
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

	// z/w = 1�� �ǵ��� (�� �ϴ� ���� �׻� �� ��鿡 �ֵ���) z = w�� �����Ѵ�.
	output.posH = mul(float4(input.pos, 1.0f), gWorldViewProj).xyww;

	output.posL = input.pos;

	return output;
}
