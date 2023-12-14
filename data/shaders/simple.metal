#include <metal_stdlib>

using namespace metal;

struct v2f
{
	float4 position [[position]];
};

struct VertexData
{
	float3 position;
};

v2f vertex simple_vertex(device const VertexData* vertexData [[buffer(0)]],
					  uint vertexId [[vertex_id]],
					  uint instanceId [[instance_id]])
{
	v2f o;

	const device VertexData& vd = vertexData[vertexId];
	o.position = float4(vd.position, 1.0);

	return o;
}

half4 fragment simple_fragment(v2f in [[stage_in]])
{
	return half4(1.0, 0.5, 0.5, 1.0);
}
