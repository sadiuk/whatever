#pragma pack_matrix(row_major)

struct VS_INPUT
{
	[[vk::location(0)]] float3 normal : NORMAL;
	[[vk::location(1)]] float3 position : POSITION;
	[[vk::location(2)]] float4 tangent : TANGENT;
	[[vk::location(3)]] float2 texcoord : TEXCOORD0;
};

cbuffer Camera : register(b0, space0)
{
    float4x4 viewMatrix; 
    float4x4 projectionMatrix;
    float4x4 viewProjMatrix;
};

struct MaterialParamEntry
{
    float4 baseColorRGBMetallicA;
    float4 emissiveRGBRoughnessA;
};
StructuredBuffer<MaterialParamEntry> materialParams : register(t0, space1);

Texture2D matTextures[] : register(t2, space1);
SamplerState matSampler : register(s1, space1);

struct PushConstants
{
    uint diffuseTexIndex;
    uint normalTexIndex;
    uint metalicRoughnessTexIndex;
    uint emissiveTexIndex;
};

[[vk::push_constant]]
ConstantBuffer<PushConstants> pc;

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
    float4 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    //if (viewMatrix[0][0]  == 34.5)
    //    output.pos = float4(0, 0, 0, 0);
    float4 viewPos = mul(float4(input.position, 1), viewMatrix);
    output.pos = mul(viewPos, projectionMatrix);
    output.color = float4(input.normal, 1);
    output.uv = input.texcoord;
    output.normal = input.normal;
    output.tangent = input.tangent;
	return output;
}


struct PS_OUTPUT
{
	float4 color : SV_Target0;
};

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;
    output.color = input.color;
    int textureIndex = pc.diffuseTexIndex;
    Texture2D diffuseTexture = matTextures[textureIndex];
    float4 albedo = diffuseTexture.Sample(matSampler, input.uv);
    
    float3 geomNormal = normalize(input.normal);
    float3 geomTangent = normalize(input.tangent.xyz);
    Texture2D normalTexture = matTextures[pc.normalTexIndex];

    float3 bitangent = input.tangent.w * cross(geomNormal, geomTangent);
    float3x3 TBN = float3x3(
    bitangent.x, bitangent.y, bitangent.z,
    geomTangent.x, geomTangent.y, geomTangent.z,
    geomNormal.x, geomNormal.y, geomNormal.z
    );

    float3 normal = (normalTexture.Sample(matSampler, input.uv).xyz - (float3)0.5) * 2.0;
    normal = mul(normal, TBN);

    float3 L = normalize(float3(0.1, 0.1, 1));
    float NdotL = dot(normal, L);
    output.color = float4(NdotL * albedo.rgb, albedo.a);
    return output;
}