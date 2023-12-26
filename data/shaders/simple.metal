#include <metal_stdlib>

using namespace metal;

struct v2f
{
	float4 position [[position]];
	float2 texcoord;
	float3 color;
};

struct VertexData
{
	packed_float3 position;
	packed_float3 normal;
	packed_float2 uv0;
};

struct CameraData
{
	float4x4 viewProjectionMatrix;
};

v2f vertex simple_vertex(device const VertexData* vertexData [[buffer(0)]],
						 device const CameraData& cameraData [[buffer(1)]],
					  	 uint vertexId [[vertex_id]],
					  	 uint instanceId [[instance_id]])
{
	v2f o;

	const device VertexData& vd = vertexData[vertexId];
	float4 position = float4(vd.position, 1.0);
	position.z = -position.z;
	//position.x += instanceId * 0.5f;
	//float3 a[3] = {float3(-1, -1, 0), float3(-1, 0, 0), float3(0, 0, 0)};
	//o.position = float4(a[vertexId % 3], 1.0);
	o.position = cameraData.viewProjectionMatrix * position;
	o.texcoord = vd.uv0;

	return o;
}

half4 fragment simple_fragment(v2f in [[stage_in]],
							   texture2d< half, access::sample > tex [[texture(0)]])
{
	constexpr sampler s(address::repeat, filter::nearest);
	half3 texel = tex.sample(s, in.texcoord).rgb;
	//half3 c = half3(in.color);
	return half4(texel.x, texel.y, texel.z, 1.0);
}
