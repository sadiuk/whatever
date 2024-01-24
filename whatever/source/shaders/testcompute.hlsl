RWTexture2D<float> tex : register(u1);

[numthreads(4, 4, 4)]
void CS(uint3 dtid : SV_DispatchThreadID)
{
	tex[dtid.xy] = 1.;
}