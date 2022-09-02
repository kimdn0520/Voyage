
cbuffer cbMesh : register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint materialID;
}

struct SkyBoxPixelIN
{
	float4 posH     : SV_POSITION;
	float3 posL     : POSITION;
};

struct PS_OUT
{
	float4 albedo : SV_Target0;
	float4 depth  : SV_Target1;
	float4 normal : SV_Target2;
	float4 pos	  : SV_Target3;
	uint materialID  : SV_Target4;
};

TextureCube gCubeMap : register(t0);
SamplerState samLinearWrap : register(s0);
 
PS_OUT SkyBox_PS(SkyBoxPixelIN input) : SV_Target
{
	PS_OUT pout;

	pout.albedo = gCubeMap.Sample(samLinearWrap, input.posL);

	pout.materialID = materialID;

	return pout;
}
