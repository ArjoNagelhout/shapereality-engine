#include <metal_stdlib>

using namespace metal;

struct v2f
{
	float4 position [[position]];
	float3 color;
};

struct VertexData
{
	packed_float3 position;
	packed_float3 normal;
	packed_float3 color;
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
	//position.x += instanceId * 0.5f;
	o.position = cameraData.viewProjectionMatrix * position;

	o.color = vd.color;

	return o;
}

half4 fragment simple_fragment(v2f in [[stage_in]])
{
	half3 c = half3(in.color);
	return half4(c, 1.0);
}
