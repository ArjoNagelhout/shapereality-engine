#include <metal_stdlib>

using namespace metal;

struct v2f
{
    float4 position [[position]];
    float2 texcoord;
    float3 color;
};

struct CameraData
{
    float4x4 viewProjectionMatrix;
};

struct InstanceData
{
    float4x4 localToWorldTransform;
};

v2f vertex new_color_vertex(
    //device packed_float3 const* normals [[buffer(0)]],
    device packed_float3 const* positions [[buffer(0)]],
    device packed_float3 const* colors [[buffer(1)]],
    //device packed_float2 const* uv0s [[buffer(2)]],
    device CameraData const& cameraData [[buffer(3)]],
    device InstanceData const& instanceData [[buffer(4)]],
    uint vertexId [[vertex_id]],
    uint instanceId [[instance_id]])
{
    v2f o;

    device packed_float3 const& position = positions[vertexId];
    device packed_float3 const& color = colors[vertexId];
//    device packed_float3 const& normal = normals[vertexId];
//    device packed_float2 const& uv0 = uv0s[vertexId];

    //float4x4 localToWorldMatrix = instanceData.localToWorldTransform;
//    o.position = float4(position.x, position.y, 0.0, 1.0);
//    o.position = cameraData.viewProjectionMatrix * localToWorldMatrix * float4(position, 1.0);
//    o.texcoord = uv0;

    //float3 a[3] = {float3(0.5, -0.5, 0), float3(-0.5, -0.5, 0), float3(0, 0.5, 0)};
    //float3 c[3] = {float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0)};
    //o.position = float4(a[vertexId % 3], 1.0);
    o.position = float4(position, 1.0);
    o.color = color;

    return o;
}

half4 fragment new_color_fragment(v2f in [[stage_in]],
                               texture2d< half, access::sample > tex [[texture(0)]])
{
    //constexpr sampler s(address::repeat, filter::nearest);
    //half3 texel = tex.sample(s, in.texcoord).rgb;
    //half3 texel = half3(0.0, 0.0, 0.0);
    half3 c = half3(in.color);
    return half4(c, 1.0);
    //return half4(texel.x, texel.y, texel.z, 1.0);
}
