

struct SkyBoxPixelIN
{
	float4 posH     : SV_POSITION;
	float3 posL     : POSITION;
};

TextureCube gCubeMap : register(t0);
SamplerState samLinearWrap : register(s0);

float4 WaterPassSky_PS(SkyBoxPixelIN input) : SV_Target
{
	return gCubeMap.Sample(samLinearWrap, input.posL);
}
