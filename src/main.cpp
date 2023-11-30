#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "application.h"
#include "renderer/window.h"
#include "renderer/texture.h"
#include "scene/scene.h"
#include "renderer/command_buffer.h"

#include <iostream>

#include <cassert>
#include <simd/simd.h>

#include "math/matrix.h"

static constexpr size_t kInstanceRows = 20;
static constexpr size_t kInstanceColumns = 20;
static constexpr size_t kInstanceDepth = 20;
static constexpr size_t kNumInstances = (kInstanceRows * kInstanceColumns * kInstanceDepth);
static constexpr size_t MAX_FRAMES_IN_FLIGHT = 3;

namespace math
{
	constexpr simd::float3 add(const simd::float3& a, const simd::float3& b);

	constexpr simd_float4x4 makeIdentity();

	simd::float4x4 makePerspective();

	simd::float4x4 makeXRotate(float angleRadians);

	simd::float4x4 makeYRotate(float angleRadians);

	simd::float4x4 makeZRotate(float angleRadians);

	simd::float4x4 makeTranslate(const simd::float3& v);

	simd::float4x4 makeScale(const simd::float3& v);

	simd::float3x3 discardTranslation(const simd::float4x4& m);
}

class MTLRenderer
{
public:
	explicit MTLRenderer(MTL::Device* pDevice);

	~MTLRenderer();

	void buildShaders();

	void buildDepthStencilStates();

	void buildTextures();

	void buildBuffers();

	void draw(MTK::View* pView);

private:
	MTL::Device* pDevice{nullptr};
	MTL::CommandQueue* pCommandQueue{nullptr};
	MTL::Library* pShaderLibrary{nullptr};
	MTL::RenderPipelineState* pPSO{nullptr};
	MTL::DepthStencilState* pDepthStencilState{nullptr};
	MTL::Texture* pTexture{nullptr};
	MTL::Buffer* pVertexDataBuffer{nullptr};
	MTL::Buffer* pInstanceDataBuffers[MAX_FRAMES_IN_FLIGHT];
	MTL::Buffer* pCameraDataBuffers[MAX_FRAMES_IN_FLIGHT];
	MTL::Buffer* pIndexBuffer{nullptr};
	float angle;
	int frame;
	dispatch_semaphore_t semaphore;
	static int const MAX_FRAMES_IN_FLIGHT;
};

namespace math
{
	constexpr simd::float3 add(const simd::float3& a, const simd::float3& b)
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z};
	}

	constexpr simd_float4x4 makeIdentity()
	{
		using simd::float4;
		return (simd_float4x4) {(float4) {1.f, 0.f, 0.f, 0.f},
								(float4) {0.f, 1.f, 0.f, 0.f},
								(float4) {0.f, 0.f, 1.f, 0.f},
								(float4) {0.f, 0.f, 0.f, 1.f}};
	}

	simd::float4x4 makePerspective(float fovRadians, float aspect, float znear, float zfar)
	{
		using simd::float4;
		float ys = 1.f / tanf(fovRadians * 0.5f);
		float xs = ys / aspect;
		float zs = zfar / (znear - zfar);
		return simd_matrix_from_rows((float4) {xs, 0.0f, 0.0f, 0.0f},
									 (float4) {0.0f, ys, 0.0f, 0.0f},
									 (float4) {0.0f, 0.0f, zs, znear * zs},
									 (float4) {0, 0, -1, 0});
	}

	simd::float4x4 makeXRotate(float angleRadians)
	{
		using simd::float4;
		const float a = angleRadians;
		return simd_matrix_from_rows((float4) {1.0f, 0.0f, 0.0f, 0.0f},
									 (float4) {0.0f, cosf(a), sinf(a), 0.0f},
									 (float4) {0.0f, -sinf(a), cosf(a), 0.0f},
									 (float4) {0.0f, 0.0f, 0.0f, 1.0f});
	}

	simd::float4x4 makeYRotate(float angleRadians)
	{
		using simd::float4;
		const float a = angleRadians;
		return simd_matrix_from_rows((float4) {cosf(a), 0.0f, sinf(a), 0.0f},
									 (float4) {0.0f, 1.0f, 0.0f, 0.0f},
									 (float4) {-sinf(a), 0.0f, cosf(a), 0.0f},
									 (float4) {0.0f, 0.0f, 0.0f, 1.0f});
	}

	simd::float4x4 makeZRotate(float angleRadians)
	{
		using simd::float4;
		const float a = angleRadians;
		return simd_matrix_from_rows((float4) {cosf(a), sinf(a), 0.0f, 0.0f},
									 (float4) {-sinf(a), cosf(a), 0.0f, 0.0f},
									 (float4) {0.0f, 0.0f, 1.0f, 0.0f},
									 (float4) {0.0f, 0.0f, 0.0f, 1.0f});
	}

	simd::float4x4 makeTranslate(const simd::float3& v)
	{
		using simd::float4;
		const float4 col0 = {1.0f, 0.0f, 0.0f, 0.0f};
		const float4 col1 = {0.0f, 1.0f, 0.0f, 0.0f};
		const float4 col2 = {0.0f, 0.0f, 1.0f, 0.0f};
		const float4 col3 = {v.x, v.y, v.z, 1.0f};
		return simd_matrix(col0, col1, col2, col3);
	}

	simd::float4x4 makeScale(const simd::float3& v)
	{
		using simd::float4;
		return simd_matrix((float4) {v.x, 0, 0, 0},
						   (float4) {0, v.y, 0, 0},
						   (float4) {0, 0, v.z, 0},
						   (float4) {0, 0, 0, 1.0});
	}

	simd::float3x3 discardTranslation(const simd::float4x4& m)
	{
		return simd_matrix(m.columns[0].xyz, m.columns[1].xyz, m.columns[2].xyz);
	}

}

const int MTLRenderer::MAX_FRAMES_IN_FLIGHT = 3;

MTLRenderer::MTLRenderer(MTL::Device* pDevice)
	: pDevice(pDevice->retain()), angle(0.f), frame(0)
{
	pCommandQueue = pDevice->newCommandQueue();
	buildShaders();
	buildDepthStencilStates();
	buildTextures();
	buildBuffers();

	semaphore = dispatch_semaphore_create(MTLRenderer::MAX_FRAMES_IN_FLIGHT);
}

MTLRenderer::~MTLRenderer()
{
	pTexture->release();
	pShaderLibrary->release();
	pDepthStencilState->release();
	pVertexDataBuffer->release();
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		pInstanceDataBuffers[i]->release();
	}
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		pCameraDataBuffers[i]->release();
	}
	pIndexBuffer->release();
	pPSO->release();
	pCommandQueue->release();
	pDevice->release();
}

namespace shader_types
{
	struct VertexData
	{
		simd::float3 position;
		simd::float3 normal;
		simd::float2 texcoord;
	};

	struct InstanceData
	{
		simd::float4x4 instanceTransform;
		simd::float3x3 instanceNormalTransform;
		simd::float4 instanceColor;
	};

	struct CameraData
	{
		simd::float4x4 perspectiveTransform;
		simd::float4x4 worldTransform;
		simd::float3x3 worldNormalTransform;
	};
}

void MTLRenderer::buildShaders()
{
	using NS::StringEncoding::UTF8StringEncoding;

	const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            float3 normal;
            half3 color;
            float2 texcoord;
        };

        struct VertexData
        {
            float3 position;
            float3 normal;
            float2 texcoord;
        };

        struct InstanceData
        {
            float4x4 instanceTransform;
            float3x3 instanceNormalTransform;
            float4 instanceColor;
        };

        struct CameraData
        {
            float4x4 perspectiveTransform;
            float4x4 worldTransform;
            float3x3 worldNormalTransform;
        };

        v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
                               device const InstanceData* instanceData [[buffer(1)]],
                               device const CameraData& cameraData [[buffer(2)]],
                               uint vertexId [[vertex_id]],
                               uint instanceId [[instance_id]] )
        {
            v2f o;

            const device VertexData& vd = vertexData[ vertexId ];
            float4 pos = float4( vd.position, 1.0 );
            pos = instanceData[ instanceId ].instanceTransform * pos;
            pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
            o.position = pos;

            float3 normal = instanceData[ instanceId ].instanceNormalTransform * vd.normal;
            normal = cameraData.worldNormalTransform * normal;
            o.normal = normal;

            o.texcoord = vd.texcoord.xy;

            o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
            return o;
        }

        half4 fragment fragmentMain( v2f in [[stage_in]], texture2d< half, access::sample > tex [[texture(0)]] )
        {
            constexpr sampler s( address::repeat, filter::linear );
            half3 texel = tex.sample( s, in.texcoord ).rgb;

            // assume light coming from (front-top-right)
            float3 l = normalize(float3( 1.0, 1.0, 0.8 ));
            float3 n = normalize( in.normal );

            half ndotl = half( saturate( dot( n, l ) ) );

            half3 illum = (in.color * texel * 0.1) + (in.color * texel * ndotl);
            return half4( illum, 1.0 );
        }
    )";

	NS::Error* pError = nullptr;
	MTL::Library* pLibrary = pDevice->newLibrary(NS::String::string(shaderSrc, UTF8StringEncoding), nullptr,
												 &pError);
	if (!pLibrary)
	{
		__builtin_printf("%s", pError->localizedDescription()->utf8String());
		assert(false);
	}

	MTL::Function* pVertexFn = pLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
	MTL::Function* pFragFn = pLibrary->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

	MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
	pDesc->setVertexFunction(pVertexFn);
	pDesc->setFragmentFunction(pFragFn);
	pDesc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	pDesc->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth16Unorm);

	pPSO = pDevice->newRenderPipelineState(pDesc, &pError);
	if (!pPSO)
	{
		__builtin_printf("%s", pError->localizedDescription()->utf8String());
		assert(false);
	}

	pVertexFn->release();
	pFragFn->release();
	pDesc->release();
	pShaderLibrary = pLibrary;
}

void MTLRenderer::buildDepthStencilStates()
{
	MTL::DepthStencilDescriptor* pDsDesc = MTL::DepthStencilDescriptor::alloc()->init();
	pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
	pDsDesc->setDepthWriteEnabled(true);

	pDepthStencilState = pDevice->newDepthStencilState(pDsDesc);

	pDsDesc->release();
}

void MTLRenderer::buildTextures()
{
	const uint32_t tw = 128;
	const uint32_t th = 128;

	MTL::TextureDescriptor* pTextureDesc = MTL::TextureDescriptor::alloc()->init();
	pTextureDesc->setWidth(tw);
	pTextureDesc->setHeight(th);
	pTextureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
	pTextureDesc->setTextureType(MTL::TextureType2D);
	pTextureDesc->setStorageMode(MTL::StorageModeManaged);
	pTextureDesc->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead);

	MTL::Texture* pTexture = pDevice->newTexture(pTextureDesc);
	this->pTexture = pTexture;

	auto* pTextureData = (uint8_t*) alloca(tw * th * 4);
	for (size_t y = 0; y < th; ++y)
	{
		for (size_t x = 0; x < tw; ++x)
		{
			bool isWhite = (x ^ y) & 0b1000000;
			uint8_t c = isWhite ? 0xFF : 0xA;

			size_t i = y * tw + x;

			pTextureData[i * 4 + 0] = c;
			pTextureData[i * 4 + 1] = c;
			pTextureData[i * 4 + 2] = c;
			pTextureData[i * 4 + 3] = 0xFF;
		}
	}

	pTexture->replaceRegion(MTL::Region(0, 0, 0, tw, th, 1), 0, pTextureData, tw * 4);

	pTextureDesc->release();
}

void MTLRenderer::buildBuffers()
{
	using simd::float2;
	using simd::float3;

	const float s = 0.5f;

	shader_types::VertexData verts[] = {
		//                                         Texture
		//   Positions           Normals         Coordinates
		{{-s, -s, +s}, {0.f,  0.f,  1.f},  {0.f, 1.f}},
		{{+s, -s, +s}, {0.f,  0.f,  1.f},  {1.f, 1.f}},
		{{+s, +s, +s}, {0.f,  0.f,  1.f},  {1.f, 0.f}},
		{{-s, +s, +s}, {0.f,  0.f,  1.f},  {0.f, 0.f}},

		{{+s, -s, +s}, {1.f,  0.f,  0.f},  {0.f, 1.f}},
		{{+s, -s, -s}, {1.f,  0.f,  0.f},  {1.f, 1.f}},
		{{+s, +s, -s}, {1.f,  0.f,  0.f},  {1.f, 0.f}},
		{{+s, +s, +s}, {1.f,  0.f,  0.f},  {0.f, 0.f}},

		{{+s, -s, -s}, {0.f,  0.f,  -1.f}, {0.f, 1.f}},
		{{-s, -s, -s}, {0.f,  0.f,  -1.f}, {1.f, 1.f}},
		{{-s, +s, -s}, {0.f,  0.f,  -1.f}, {1.f, 0.f}},
		{{+s, +s, -s}, {0.f,  0.f,  -1.f}, {0.f, 0.f}},

		{{-s, -s, -s}, {-1.f, 0.f,  0.f},  {0.f, 1.f}},
		{{-s, -s, +s}, {-1.f, 0.f,  0.f},  {1.f, 1.f}},
		{{-s, +s, +s}, {-1.f, 0.f,  0.f},  {1.f, 0.f}},
		{{-s, +s, -s}, {-1.f, 0.f,  0.f},  {0.f, 0.f}},

		{{-s, +s, +s}, {0.f,  1.f,  0.f},  {0.f, 1.f}},
		{{+s, +s, +s}, {0.f,  1.f,  0.f},  {1.f, 1.f}},
		{{+s, +s, -s}, {0.f,  1.f,  0.f},  {1.f, 0.f}},
		{{-s, +s, -s}, {0.f,  1.f,  0.f},  {0.f, 0.f}},

		{{-s, -s, -s}, {0.f,  -1.f, 0.f},  {0.f, 1.f}},
		{{+s, -s, -s}, {0.f,  -1.f, 0.f},  {1.f, 1.f}},
		{{+s, -s, +s}, {0.f,  -1.f, 0.f},  {1.f, 0.f}},
		{{-s, -s, +s}, {0.f,  -1.f, 0.f},  {0.f, 0.f}}
	};

	uint16_t indices[] = {
		0, 1, 2, 2, 3, 0, /* front */
		4, 5, 6, 6, 7, 4, /* right */
		8, 9, 10, 10, 11, 8, /* back */
		12, 13, 14, 14, 15, 12, /* left */
		16, 17, 18, 18, 19, 16, /* top */
		20, 21, 22, 22, 23, 20, /* bottom */
	};

	const size_t vertexDataSize = sizeof(verts);
	const size_t indexDataSize = sizeof(indices);

	MTL::Buffer* pVertexBuffer = pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
	MTL::Buffer* pIndexBuffer = pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

	pVertexDataBuffer = pVertexBuffer;
	this->pIndexBuffer = pIndexBuffer;

	memcpy(pVertexDataBuffer->contents(), verts, vertexDataSize);
	memcpy(pIndexBuffer->contents(), indices, indexDataSize);

	pVertexDataBuffer->didModifyRange(NS::Range::Make(0, pVertexDataBuffer->length()));
	pIndexBuffer->didModifyRange(NS::Range::Make(0, pIndexBuffer->length()));

	const size_t instanceDataSize = MAX_FRAMES_IN_FLIGHT * kNumInstances * sizeof(shader_types::InstanceData);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		pInstanceDataBuffers[i] = pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
	}

	const size_t cameraDataSize = MAX_FRAMES_IN_FLIGHT * sizeof(shader_types::CameraData);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		pCameraDataBuffers[i] = pDevice->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged);
	}
}

void MTLRenderer::draw(MTK::View* pView)
{
	using simd::float3;
	using simd::float4;
	using simd::float4x4;

	NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

	frame = (frame + 1) % MTLRenderer::MAX_FRAMES_IN_FLIGHT;
	MTL::Buffer* pInstanceDataBuffer = pInstanceDataBuffers[frame];

	MTL::CommandBuffer* pCmd = pCommandQueue->commandBuffer();
	dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
	MTLRenderer* pRenderer = this;
	pCmd->addCompletedHandler(^void(MTL::CommandBuffer* pCmd) {
		dispatch_semaphore_signal(pRenderer->semaphore);
	});

	angle += 0.01f;

	const float scl = 0.1f;
	auto* pInstanceData = reinterpret_cast< shader_types::InstanceData*>( pInstanceDataBuffer->contents());

	float3 objectPosition = {0.f, 0.f, -10.f};

	float4x4 rt = math::makeTranslate(objectPosition);
	float4x4 rr1 = math::makeYRotate(-angle);
	float4x4 rr0 = math::makeXRotate(angle * 0.f);
	float4x4 rtInv = math::makeTranslate({-objectPosition.x, -objectPosition.y, -objectPosition.z});
	float4x4 fullObjectRot = rt * rr1 * rr0 * rtInv;

	size_t ix = 0;
	size_t iy = 0;
	size_t iz = 0;
	for (size_t i = 0; i < kNumInstances; ++i)
	{
		if (ix == kInstanceRows)
		{
			ix = 0;
			iy += 1;
		}
		if (iy == kInstanceRows)
		{
			iy = 0;
			iz += 1;
		}

		float4x4 scale = math::makeScale((float3) {scl, scl, scl});
		float4x4 zrot = math::makeZRotate(angle * sinf((float) ix));
		float4x4 yrot = math::makeYRotate(angle * cosf((float) iy));

		float x = ((float) ix - (float) kInstanceRows / 2.f) * (2.f * scl) + scl;
		float y = ((float) iy - (float) kInstanceColumns / 2.f) * (2.f * scl) + scl;
		float z = ((float) iz - (float) kInstanceDepth / 2.f) * (2.f * scl);
		float4x4 translate = math::makeTranslate(math::add(objectPosition, {x, y, z}));

		pInstanceData[i].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;
		pInstanceData[i].instanceNormalTransform = math::discardTranslation(pInstanceData[i].instanceTransform);

		float iDivNumInstances = i / (float) kNumInstances;
		float r = iDivNumInstances;
		float g = 1.0f - r;
		float b = sinf(M_PI * 2.0f * iDivNumInstances);
		pInstanceData[i].instanceColor = (float4) {r, g, b, 1.0f};

		ix += 1;
	}
	pInstanceDataBuffer->didModifyRange(NS::Range::Make(0, pInstanceDataBuffer->length()));

	// Update camera state:

	MTL::Buffer* pCameraDataBuffer = pCameraDataBuffers[frame];
	auto* pCameraData = reinterpret_cast< shader_types::CameraData*>( pCameraDataBuffer->contents());
	pCameraData->perspectiveTransform = math::makePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
	pCameraData->worldTransform = math::makeIdentity();
	pCameraData->worldNormalTransform = math::discardTranslation(pCameraData->worldTransform);
	pCameraDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(shader_types::CameraData)));

	// Begin render pass:

	MTL::RenderPassDescriptor* pRpd = pView->currentRenderPassDescriptor();
	MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder(pRpd);

	pEnc->setRenderPipelineState(pPSO);
	pEnc->setDepthStencilState(pDepthStencilState);

	pEnc->setVertexBuffer(pVertexDataBuffer, /* offset */ 0, /* index */ 0);
	pEnc->setVertexBuffer(pInstanceDataBuffer, /* offset */ 0, /* index */ 1);
	pEnc->setVertexBuffer(pCameraDataBuffer, /* offset */ 0, /* index */ 2);

	pEnc->setFragmentTexture(pTexture, /* index */ 0);

	pEnc->setCullMode(MTL::CullModeBack);
	pEnc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

	pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
								6 * 6, MTL::IndexType::IndexTypeUInt16,
								pIndexBuffer,
								0,
								kNumInstances);

	pEnc->endEncoding();
	pCmd->presentDrawable(pView->currentDrawable());
	pCmd->commit();

	pPool->release();
}

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public renderer::IWindowDelegate
{
public:
	explicit App()
	{
		pScene = std::make_unique<scene::Scene>();
		pTexture = renderer::create<renderer::Texture>(1024, 1024, renderer::TextureFormat::RGBA8Unorm_sRGB);
	}

	~App()
	{
		pScene.reset();
	}

	void applicationDidFinishLaunching() override
	{
		std::cout << "we finished launching, happy" << std::endl;
		math::Matrix<4, 4> m{
			{{
				 {{1, 2, 3, 4}},
				 {{5, 6, 7, 8}},
				 {{9, 10, 11, 12}},
				 {{4, 3, 2, 1}}
			}}
		};

		math::Vector<3> ba{{1, 2, 3}};

		auto baz = ba;

		auto biez = baz * 10;
		auto booz = biez / 10;

		auto dae = math::vec3::dot(biez, booz);
		auto cross = math::vec3::cross(math::vec3{{3, -4, 6}}, math::vec3{{15, 23, 54}});

		auto distance = math::vec3::distance(biez, cross);

		auto maxi = math::vec3::max(biez, booz);

		auto kaba = math::vec2{{234234, 1111}};
		auto baba = static_cast<math::vec3>(kaba);
		auto aaaa = static_cast<math::vec4>(kaba);
		auto idie = static_cast<math::vec2>(aaaa);

		std::cout << "ba: " << ba.toString() << std::endl;
		std::cout << "baz: " << baz.toString() << std::endl;
		std::cout << "biez: " << biez.toString() << std::endl;
		std::cout << "booz: " << booz.toString() << std::endl;
		std::cout << "dae: " << dae << std::endl;
		std::cout << "cross: " << cross.toString() << std::endl;
		std::cout << "distance: " << distance << std::endl;

		std::cout << "kaba: " << kaba.toString() << std::endl;
		std::cout << "baba: " << baba.toString() << std::endl;
		std::cout << "aaaa: " << aaaa.toString() << std::endl;
		std::cout << "idie: " << idie.toString() << std::endl;

		std::cout << "matrix wee:" << m.toString() << std::endl;
	}

	void render(renderer::Window* window) override
	{
		//std::cout << "sir, you gotta render a new frame" << std::endl;
		pScene->render();

		std::unique_ptr<renderer::CommandBuffer> pCmd = renderer::create<renderer::CommandBuffer>();
		pCmd->blit();
	}

private:
	std::unique_ptr<scene::Scene> pScene;
	std::unique_ptr<renderer::Texture> pTexture;
};

int main( int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	// set renderer backend
	renderer::Renderer* renderer = application.getRenderer();
	renderer->setRendererBackendType(renderer::RendererBackendType::Metal);

	// add window
	std::unique_ptr<renderer::Window> newWindow = renderer::create<renderer::Window>(0, 600, 500, 400);
	newWindow->setTitle("heyo it's a window");
	newWindow->setMinSize(300, 100);
	newWindow->setSize(900, 700);
	newWindow->setDelegate(&app);

	std::unique_ptr<renderer::Window> newWindow2 = std::make_unique<renderer::Window>(0, 200, 500, 500);
	newWindow2->registerObject();
	newWindow2->setTitle("heyo it's a window");
	newWindow2->setMinSize(300, 100);
	newWindow2->setSize(900, 700);
	newWindow2->setDelegate(&app);

	// run application
	application.run();

	return 0;
}