
cbuffer cbMesh : register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint materialID;
};

struct ParticleIN
{
	float4 pos     : SV_POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

SamplerState samAnisotropicWrap : register(s0);
//texture2D gDiffuseMap : register(t0);

struct PS_OUT
{
	float4 albedo : SV_Target0;
	float4 depth  : SV_Target1;
	float4 normal : SV_Target2;
	float4 pos	  : SV_Target3;
	uint materialID  : SV_Target4;
};

PS_OUT Particle_PS(ParticleIN input) : SV_TARGET
{
	PS_OUT pout;

	float4 textureColor;
	float4 finalColor;

	//textureColor = gDiffuseMap.Sample(samAnisotropicWrap, input.uv);
	input.color.a = 1.0f;

	pout.albedo =  input.color;

	pout.materialID = materialID;

	return pout;
}