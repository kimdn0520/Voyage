
struct NormalPixelIN
{
	float4 pos     : SV_POSITION;
	float3 posW	   : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};

float4 Normal_PS(NormalPixelIN input) : SV_Target
{
	input.normal = normalize(input.normal);

	return float4(input.normal, 1.0f);
}