#include <metal_stdlib>

using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

struct CameraData
{
    float4x4 viewProjectionMatrix;
};

struct InstanceData
{
    float4x4 localToWorldTransform;
};

v2f vertex axes_vertex(
    device packed_float3 const* positions [[buffer(0)]],
    device packed_float3 const* colors [[buffer(1)]],
    device CameraData const& cameraData [[buffer(3)]],
    device InstanceData const& instanceData [[buffer(4)]],
    uint vertexId [[vertex_id]],
    uint instanceId [[instance_id]])
{
    v2f o;

    device packed_float3 const& position = positions[vertexId];
    device packed_float3 const& color = colors[vertexId];

    float4x4 localToWorldMatrix = instanceData.localToWorldTransform;
    o.position = cameraData.viewProjectionMatrix * localToWorldMatrix * float4(position, 1.0);
    o.color = half3(color.x, color.y, color.z);

    return o;
}

half4 fragment axes_fragment(v2f in [[stage_in]],
                               texture2d< half, access::sample > tex [[texture(0)]])
{
    return half4(in.color, 1.0);
}
