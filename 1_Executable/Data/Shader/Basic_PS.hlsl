
struct BasicPixelIN
{
	float4 pos     : SV_POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

Texture2D gDiffuseMap : register(t0);
SamplerState samAnisotropicWrap : register(s0);

float4 Basic_PS(BasicPixelIN input) : SV_Target
{
#ifdef TEXTURE
	input.color = gDiffuseMap.Sample(samAnisotropicWrap, input.uv);
#endif
	input.color.a = 1.0f;

	return input.color;
}