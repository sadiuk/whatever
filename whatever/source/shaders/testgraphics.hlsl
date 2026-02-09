#pragma pack_matrix(row_major)

struct VS_INPUT
{
	[[vk::location(0)]] float4 position : POSITION0;
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
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    if (viewMatrix[0][0]  == 34.5)
        output.pos = float4(0, 0, 0, 0);
	output.pos = input.position;
	return output;
}


struct PS_OUTPUT
{
	float4 color : SV_Target0;
};

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;
    output.color = float4(1, 0,0.8, 1);
    return output;
}