
struct VS_INPUT
{
	uint vertexId : SV_VertexID;
};
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};

static float4 positions[3] =
{
    float4(-0.5, -0.5, 0, 1),
	float4(0, 0.5, 0, 1),
	float4(0.5, -0.5, 0, 1)
};
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = positions[input.vertexId];
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