#ifndef BORED_ENGINE_GRAPHICS_H
#define BORED_ENGINE_GRAPHICS_H

#include <string>
#include <unordered_map>

namespace graphics
{
	class IDevice;

	enum class GraphicsBackend : int
	{
		None = 0,
		Metal,
		Vulkan,
		OpenGL,
		WebGPU
	};
	constexpr static char const* graphicsBackendStrings[]{
		"None",
		"Metal",
		"Vulkan",
		"OpenGL",
		"WebGPU"
	};
	constexpr char const* toString(GraphicsBackend value)
	{
		return graphicsBackendStrings[static_cast<int>(value)];
	}

	// main entrypoint for creating a renderer
	[[nodiscard]] std::unique_ptr<IDevice> createDevice(GraphicsBackend backend);

	// a simple color
	struct Color final
	{
		explicit Color(float r, float g, float b, float a);
		~Color();

		float r;
		float g;
		float b;
		float a;

		static Color clear;

		// conversion
	};

	enum class WindingOrder
	{
		Clockwise,
		CounterClockwise
	};

	enum class CullMode
	{
		None,
		Front,
		Back
	};

	// The geometric primitive type for drawing commands.
	enum class PrimitiveType
	{
		// Rasterize a point at each vertex. The vertex shader must provide [[point_size]],
		// or the point size is undefined.
		Point,

		// Rasterize a line between each separate pair of vertices, resulting in a series of
		// unconnected lines. If there are an odd number of vertices, the last vertex is ignored.
		Line,

		// Rasterize a line between each pair of adjacent vertices, resulting in a series of
		// connected lines (also called a polyline).
		LineStrip,

		// For every separate set of three vertices, rasterize a triangle. If the number of
		// vertices is not a multiple of three, either one or two vertices is ignored.
		Triangle,

		// For every three adjacent vertices, rasterize a triangle.
		TriangleStrip
	};

	enum class CompareFunction
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	struct Viewport
	{
		float originX;
		float originY;
		float width;
		float height;
		float zNear;
		float zFar;
	};

	struct Range
	{
		unsigned int offset; // index where to start
		unsigned int length; // how many item indices in the range
	};

	enum class PixelFormat
	{
		Undefined,
		A8Unorm,
		R8Unorm,
		R8Unorm_sRGB,
		R8Snorm,
		R8Uint,
		R8Sint,
		R16Unorm,
		R16Snorm,
		R16Uint,
		R16Sint,
		R16Float,
		RG8Unorm,
		RG8Unorm_sRGB,
		RG8Snorm,
		RG8Uint,
		RG8Sint,
		B5G6R5Unorm,
		A1BGR5Unorm,
		ABGR4Unorm,
		BGR5A1Unorm,
		R32Uint,
		R32Sint,
		R32Float,
		RG16Unorm,
		RG16Snorm,
		RG16Uint,
		RG16Sint,
		RG16Float,
		RGBA8Unorm,
		RGBA8Unorm_sRGB,
		RGBA8Snorm,
		RGBA8Uint,
		RGBA8Sint,
		BGRA8Unorm,
		BGRA8Unorm_sRGB,
		RGB10A2Unorm,
		RGB10A2Uint,
		RG11B10Float,
		RGB9E5Float,
		BGR10A2Unorm,
		RG32Uint,
		RG32Sint,
		RG32Float,
		RGBA16Unorm,
		RGBA16Snorm,
		RGBA16Uint,
		RGBA16Sint,
		RGBA16Float,
		RGBA32Uint,
		RGBA32Sint,
		RGBA32Float,
		BC1_RGBA,
		BC1_RGBA_sRGB,
		BC2_RGBA,
		BC2_RGBA_sRGB,
		BC3_RGBA,
		BC3_RGBA_sRGB,
		BC4_RUnorm,
		BC4_RSnorm,
		BC5_RGUnorm,
		BC5_RGSnorm,
		BC6H_RGBFloat,
		BC6H_RGBUfloat,
		BC7_RGBAUnorm,
		BC7_RGBAUnorm_sRGB,
		PVRTC_RGB_2BPP,
		PVRTC_RGB_2BPP_sRGB,
		PVRTC_RGB_4BPP,
		PVRTC_RGB_4BPP_sRGB,
		PVRTC_RGBA_2BPP,
		PVRTC_RGBA_2BPP_sRGB,
		PVRTC_RGBA_4BPP,
		PVRTC_RGBA_4BPP_sRGB,
		EAC_R11Unorm,
		EAC_R11Snorm,
		EAC_RG11Unorm,
		EAC_RG11Snorm,
		EAC_RGBA8,
		EAC_RGBA8_sRGB,
		ETC2_RGB8,
		ETC2_RGB8_sRGB,
		ETC2_RGB8A1,
		ETC2_RGB8A1_sRGB,
		ASTC_4x4_sRGB,
		ASTC_5x4_sRGB,
		ASTC_5x5_sRGB,
		ASTC_6x5_sRGB,
		ASTC_6x6_sRGB,
		ASTC_8x5_sRGB,
		ASTC_8x6_sRGB,
		ASTC_8x8_sRGB,
		ASTC_10x5_sRGB,
		ASTC_10x6_sRGB,
		ASTC_10x8_sRGB,
		ASTC_10x10_sRGB,
		ASTC_12x10_sRGB,
		ASTC_12x12_sRGB,
		ASTC_4x4_LDR,
		ASTC_5x4_LDR,
		ASTC_5x5_LDR,
		ASTC_6x5_LDR,
		ASTC_6x6_LDR,
		ASTC_8x5_LDR,
		ASTC_8x6_LDR,
		ASTC_8x8_LDR,
		ASTC_10x5_LDR,
		ASTC_10x6_LDR,
		ASTC_10x8_LDR,
		ASTC_10x10_LDR,
		ASTC_12x10_LDR,
		ASTC_12x12_LDR,
		ASTC_4x4_HDR,
		ASTC_5x4_HDR,
		ASTC_5x5_HDR,
		ASTC_6x5_HDR,
		ASTC_6x6_HDR,
		ASTC_8x5_HDR,
		ASTC_8x6_HDR,
		ASTC_8x8_HDR,
		ASTC_10x5_HDR,
		ASTC_10x6_HDR,
		ASTC_10x8_HDR,
		ASTC_10x10_HDR,
		ASTC_12x10_HDR,
		ASTC_12x12_HDR,
		GBGR422,
		BGRG422,
		Depth16Unorm,
		Depth32Float,
		Stencil8,
		Depth24Unorm_Stencil8,
		Depth32Float_Stencil8,
		X32_Stencil8,
		X24_Stencil8,
		BGRA10_XR,
		BGRA10_XR_sRGB,
		BGR10_XR,
		BGR10_XR_sRGB,
	};
}

#endif //BORED_ENGINE_GRAPHICS_H