#include "LightHeader.hlsli"

cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint materialID;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[96];
};

#ifdef Skinned
struct LightVertexIn
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;

	float4 weight1 : WEIGHT;
	float4 weight2 : WEIGHTS;

	uint4 boneIndex1 : BONEINDEX;
	uint4 boneIndex2 : BONEINDEXES;
};
#else
struct LightVertexIn
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};
#endif

struct LightVertexOut
{
	float4 pos     : SV_POSITION;
	float3 posW	   : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};

LightVertexOut Light_VS(LightVertexIn input)
{
	LightVertexOut output;

#ifdef Skinned
	float3 pos = float3(0.0f, 0.0f, 0.0f);
	float3 normal = float3(0.0f, 0.0f, 0.0f);

	float totalWeight = (float) 0.f;

	for (int k = 0; k < 4; k++)
	{
		totalWeight += input.weight1[k];

		totalWeight += input.weight2[k];
	}

	for (int i = 0; i < 4; i++)
	{
		// �������� �������� ���ٰ� �����ϹǷ�, �븻���� ���� ������� ��ġ����� �ʿ����.
		pos += (input.weight1[i] / totalWeight) * mul(float4(input.pos, 1.0f), gBoneTransforms[input.boneIndex1[i]]).xyz;
		normal += (input.weight1[i] / totalWeight) * mul(input.normal, (float3x3)gBoneTransforms[input.boneIndex1[i]]);
	}

	for (int j = 0; j < 4; j++)
	{
		// �������� �������� ���ٰ� �����ϹǷ�, �븻���� ���� ������� ��ġ����� �ʿ����.
		pos += (input.weight2[j] / totalWeight) * mul(float4(input.pos, 1.0f), gBoneTransforms[input.boneIndex2[j]]).xyz;
		normal += (input.weight2[j] / totalWeight) * mul(input.normal, (float3x3)gBoneTransforms[input.boneIndex2[j]]);
	}

	output.posW = mul(float4(pos, 1.0f), gWorld).xyz;

	output.pos = mul(float4(pos, 1.0f), gWorldViewProj);

	// �������Ϳ� �������Ͱ� ������ �������Ǵ� ���� ���� ��ȯ����� ����ġ����� �븻(��������)�� �����ش�.
	output.normal = normalize(mul(normal, (float3x3)gWorldInvTranspose));
#else
	output.posW = mul(float4(input.pos, 1.0f), gWorld).xyz;

	output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);

	output.normal = normalize(mul(input.normal, (float3x3)gWorldInvTranspose));
#endif

	output.uv = input.uv;

	// output.ź��Ʈ���� = mul(ź��Ʈ����, gWorld) ����� �ٲ���
	output.tangent = mul(input.tangent, (float3x3)gWorld);

	output.color = input.color;

	return output;
}

