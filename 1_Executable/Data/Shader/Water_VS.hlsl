
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

	// 적잘한 행렬 계산을 위해 위치 벡터를 4 단위로 변경
	input.pos.w = 1.0f;		

	// 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 계산
	output.pos = mul(input.pos, gWorldViewProj);

	// 픽셀 쉐이더의 텍스처 좌표를 저장
	output.tex = input.uv;

	// 반사 투영 세계 행렬을 만든다
	reflectProjectWorld = mul(gReflection, gProj);
	reflectProjectWorld = mul(gWorld, reflectProjectWorld);

	// reflectProjectWorld 행렬에 대한 입력 위치를 계산합니다.
	output.reflectionPos = mul(input.pos, reflectProjectWorld);

	// viewProjectWorld 행렬에 대해 입력 위치를 계산
	output.refractionPos = mul(input.pos, gWorldViewProj);

	output.normal = normalize(mul(input.normal, (float3x3)gWorldInvTranspose));

	output.color = input.color;

	output.posL = input.pos;

	return output;
}