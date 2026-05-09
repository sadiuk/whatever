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

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
    float4 color : TEXCOORD0;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    //if (viewMatrix[0][0]  == 34.5)
    //    output.pos = float4(0, 0, 0, 0);
    float4 viewPos = mul(float4(input.position, 1), viewMatrix);
    output.pos = mul(viewPos, projectionMatrix);
    output.color = float4(input.normal, 1);
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
    return output;
}