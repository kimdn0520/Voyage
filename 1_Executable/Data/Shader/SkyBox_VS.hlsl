
cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;
};

struct SkyBoxVertexIN
{
	float3 pos		: POSITION;
	float4 color	: COLOR;
	float2 uv		: UV;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};

struct SkyBoxVertexOut
{
	float4 posH     : SV_POSITION;
	float3 posL     : POSITION;
};

SkyBoxVertexOut SkyBox_VS(SkyBoxVertexIN input)
{
	SkyBoxVertexOut output;

	// z/w = 1이 되도록 (즉 하늘 돔이 항상 먼 평면에 있도록) z = w로 설정한다.
	output.posH = mul(float4(input.pos, 1.0f), gWorldViewProj).xyww;

	output.posL = input.pos;

	return output;
}
