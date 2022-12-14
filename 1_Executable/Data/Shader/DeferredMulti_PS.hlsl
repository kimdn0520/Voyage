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


struct DeferredMultiIN
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

struct PS_OUT
{
	float4 albedo : SV_Target0;
	float4 depth  : SV_Target1;
	float4 normal : SV_Target2;
	float4 pos	  : SV_Target3;
	uint materialID  : SV_Target4;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samAnisotropicWrap : register(s0);
SamplerState samLinearWrap : register(s1);

PS_OUT DeferredMulti_PS(DeferredMultiIN input) : SV_Target
{
	// gMaterial[materialID]->isDiffuseTexture 이렇게..?
	if (gMaterials[materialID].isDiffuseTexture)
	{
		input.color = gDiffuseMap.Sample(samAnisotropicWrap, input.uv);
	}

	// 0 <= r, g, b <= 1 을 만족하는 정규화된 성분들로 이루어진 (r,g,b)가 설정되었다. 
	// [0, 255] 구간의 정수를 255로 나누어서 [0, 1] 구간의 부동소수점 수로 만드는 작업이 된 것이다.
	// NormalSampleToWorldSpace 함수에서 단위벡터범위인 [-1, 1]로 비례 시켜준다.
	if (gMaterials[materialID].isNormalTexture)
	{
		float3 normalMapSample = gNormalMap.Sample(samLinearWrap, input.uv).rgb;
		input.normal = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
	}

	if (gMaterials[materialID].isSpecularTexture)
	{
		// 일단 임시
		input.color.a = 1.0f;
	}

	

	input.color.a = 1.0f;

	PS_OUT pout;

	input.normal = normalize(input.normal);

	pout.albedo = input.color;

	pout.depth = float4(input.pos.zzz, 1.0f);

	pout.normal = float4(input.normal, 1.0f);

	pout.pos = float4(input.posW, 1.0f);

	pout.materialID = materialID;

	return pout;
}