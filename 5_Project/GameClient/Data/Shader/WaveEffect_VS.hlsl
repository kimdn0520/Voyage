cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint objectID;
};

cbuffer cbNoise : register(b1)
{
	float frameTime;
	float3 scrollSpeeds;
	float3 scales;
	float padding;
};

struct WaveVertexIN
{
	float3 pos		: POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
};

struct WaveVertexOut
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float2 tex1		: TEXCOORD1;
	float2 tex2		: TEXCOORD2;
	float2 tex3		: TEXCOORD3;
};

WaveVertexOut WaveEffect_VS(WaveVertexIN input) 
{
	WaveVertexOut output;

	output.pos = mul(float4(input.pos,1.0), gWorldViewProj);

	output.tex = input.uv;
	// ù ��° ������ �� ���� ��ũ�Ѹ� �ӵ����� ����Ͽ� ù ��° 
	// ������ �ؽ�ó�� ���� �ؽ�ó ��ǥ�� ���
	output.tex1 = (input.uv * scales.x);
	output.tex1.y = output.tex1.y + (frameTime *scrollSpeeds.x);

	output.tex2 = (input.uv * scales.y);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	output.tex3 = (input.uv * scales.z);
	output.tex3.y = output.tex3.y + (frameTime * scrollSpeeds.z);

	return output;
}