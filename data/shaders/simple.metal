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
};



v2f vertex simple_vertex(device const VertexData* vertexData [[buffer(0)]],
					  uint vertexId [[vertex_id]],
					  uint instanceId [[instance_id]])
{
	v2f o;

	float3 a[3] = {float3(0.5, -0.5, 0), float3(-0.5, -0.5, 0), float3(0, 0.5, 0)};
	float3 c[3] = {float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0)};

	const device VertexData& vd = vertexData[vertexId];
	o.position = float4(vd.position, 1.0);
	//o.position = float4(a[vertexId % 3], 1.0);
	o.color = c[vertexId % 3];

	return o;
}

half4 fragment simple_fragment(v2f in [[stage_in]])
{
	half3 c = half3(in.color);
	return half4(c, 1.0);
}
