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

v2f vertex simple_vertex(device const VertexData* vertexData [[buffer(0)]],
					  uint vertexId [[vertex_id]],
					  uint instanceId [[instance_id]])
{
	v2f o;

	const device VertexData& vd = vertexData[vertexId];
	o.position = float4(vd.position, 1.0);
	o.position.x += instanceId / 10.0f;
	o.color = vd.color;

	return o;
}

half4 fragment simple_fragment(v2f in [[stage_in]])
{
	half3 c = half3(in.color);
	return half4(c, 1.0);
}
