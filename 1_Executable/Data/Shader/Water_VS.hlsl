
cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint objectID;
};

cbuffer cbReflection : register(b1)
{
	float4x4 gReflection;
}

struct WaterVertexIN
{ 
	float4 pos		: POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};

struct WaterVertexOut
{
	float4 pos : SV_POSITION;
	float3 posL : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 reflectionPos : TEXCOORD1;
	float4 refractionPos : TEXCOORD2;
};


WaterVertexOut Water_VS(WaterVertexIN input)
{
	WaterVertexOut output;

	matrix reflectProjectWorld;
	matrix viewProjectWorld;

	// ������ ��� ����� ���� ��ġ ���͸� 4 ������ ����
	input.pos.w = 1.0f;		

	// ����, �� �� ���� ��Ŀ� ���� ������ ��ġ�� ���
	output.pos = mul(input.pos, gWorldViewProj);

	// �ȼ� ���̴��� �ؽ�ó ��ǥ�� ����
	output.tex = input.uv;

	// �ݻ� ���� ���� ����� �����
	reflectProjectWorld = mul(gReflection, gProj);
	reflectProjectWorld = mul(gWorld, reflectProjectWorld);

	// reflectProjectWorld ��Ŀ� ���� �Է� ��ġ�� ����մϴ�.
	output.reflectionPos = mul(input.pos, reflectProjectWorld);

	// viewProjectWorld ��Ŀ� ���� �Է� ��ġ�� ���
	output.refractionPos = mul(input.pos, gWorldViewProj);

	output.normal = normalize(mul(input.normal, (float3x3)gWorldInvTranspose));

	output.color = input.color;

	output.posL = input.pos;

	return output;
}