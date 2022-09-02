#include "LightHeader.hlsli"
#include "MaterialHeader.hlsli"

cbuffer cbMesh : register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint materialID;
}

cbuffer cbMaterial : register(b1)
{
	MaterialCollection gMaterials[Material_Count_ID];
}

struct WaterPassDefalultIN
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samAnisotropicWrap : register(s0);
SamplerState samLinearWrap : register(s1);

float4 WaterPassDefault_PS(WaterPassDefalultIN input) : SV_Target
{
	input.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// ���� ������ ������ ���̻� �������Ͱ� �ƴ� �� �����Ƿ� �ٽ� ����ȭ �Ѵ�.
	input.normal = normalize(input.normal);

	input.tangent = normalize(input.tangent);

	if (gMaterials[materialID].isDiffuseTexture)
	{
		input.color = gDiffuseMap.Sample(samAnisotropicWrap, input.uv);
	}

	if (gMaterials[materialID].isNormalTexture)
	{
		float3 normalMapSample = gNormalMap.Sample(samLinearWrap, input.uv).rgb;
		input.normal = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
	}

	if (gMaterials[materialID].isSpecularTexture)
	{
		// �ϴ� �ӽ�
		input.color.a = 1.0f;
	}

	input.color.a = 1.0f;

	//litColor = pow(litColor, 1 / 2.2f);			// �����÷���

	return input.color;
}