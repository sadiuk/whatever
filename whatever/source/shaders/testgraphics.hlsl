
struct VS_INPUT
{
	[[vk::location(0)]] float4 position : POSITION0;
};
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
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
    output.color = float4(1, 0, 0, 1);
    return output;
}