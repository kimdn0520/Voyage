#include "LightHeader.hlsli"
#include "MaterialHeader.hlsli"

cbuffer cbLight : register(b0)
{
	DirectionalLight gDirLight[DIRECTION_LIGHT_COUNT];
	PointLight gPointLight[POINT_LIGHT_COUNT];
	SpotLight gSpotLight[SPOT_LIGHT_COUNT];

	float3 gEyePosW;				// CameraPos

	int gDirLightCnt;
	int gPointLightCnt;
	int gSpotLightCnt;
}

cbuffer cbMaterial : register(b1)
{
	MaterialCollection gMaterials[Material_Count_ID];
}

cbuffer cbTexture : register(b2)
{
	float4 textureInfo;
}

struct DeferredPixelIN
{
	float4 pos		   : SV_POSITION;
	float3 posW		   : POSITION;
	float4 color	   : COLOR;
	float2 uv		   : UV;
	float3 normal	   : NORMAL;
	float3 tangent	   : TANGENT;
};

Texture2D Albedo : register(t0);

Texture2D Depth : register(t1);

Texture2D Normal : register(t2);

Texture2D Position : register(t3);

Texture2D<uint> MaterialID : register(t4);

SamplerState samAnisotropicClamp : register(s0);
SamplerState samAnisotropicWrap : register(s1);
SamplerState samLinearClamp : register(s2);
SamplerState samLinearWrap : register(s3);


float4 Deferred_PS(DeferredPixelIN input) : SV_Target
{
	float4 albedo = Albedo.Sample(samLinearClamp, input.uv).xyzw;

	float4 depth = Depth.Sample(samLinearClamp, input.uv).xyzw;

	float3 normal = Normal.Sample(samLinearClamp, input.uv).xyz;

	float4 position = Position.Sample(samLinearClamp, input.uv).xyzw;

	uint materialID = (uint)MaterialID.Load(int3(input.uv.x * textureInfo.x, input.uv.y * textureInfo.y, 0));

	float3 NormalW = normalize(input.normal);

	// 초기화..
	float4 ambient = float4(depth.z - depth.z, 0.0f, 0.0f, 0.0f);			// shader reflection이 저 값들을 안쓰면 인식을 안함.. 그래서 일단 이렇게함
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 toEye = gEyePosW - position.xyz;

	// 직접적인 수치가 아니라 아이디를 불러와서 사용하게 만들었다. 
	if (!gMaterials[materialID].isUtil)
	{
		toEye = normalize(toEye);

		float4 A, D, S;

		for (int i = 0; i < gDirLightCnt; i++)
		{
			ComputeDirectionalLight(gMaterials[materialID].gMaterialAmbient, gMaterials[materialID].gMaterialDiffuse,
				gMaterials[materialID].gMaterialSpecular, gMaterials[materialID].gMaterialReflection,
				gDirLight[i], normal, toEye,
				A, D, S);


			ambient += A;
			diffuse += D;
			spec += S;
		}

		for (int i = 0; i < gPointLightCnt; i++)
		{
			ComputePointLight(gMaterials[materialID].gMaterialAmbient, gMaterials[materialID].gMaterialDiffuse,
				gMaterials[materialID].gMaterialSpecular, gMaterials[materialID].gMaterialReflection,
				gPointLight[i], position.xyz, normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		for (int i = 0; i < gSpotLightCnt; i++)
		{
			ComputeSpotLight(gMaterials[materialID].gMaterialAmbient, gMaterials[materialID].gMaterialDiffuse,
				gMaterials[materialID].gMaterialSpecular, gMaterials[materialID].gMaterialReflection,
				gSpotLight[i], position.xyz, normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		albedo = albedo * (ambient + diffuse) + spec;
		albedo.a = 1.f;
	}
	else
	{
		albedo.a = 1.f;
	}

	//if (gMaterials[materialID].isReflectionTexture)
	//{
	//	float4 normalMap;
	//	float3 reflectionNormal;
	//	float2 reflectionskyuv = input.uv;
	//	float2 reflectionobjectuv = input.uv;
	//	/*	float3 incident = -toEye;
	//	float3 reflectionVector = reflect(incident, normal);
	//	float4 reflectionColor = gCubeMap.Sample(samLinearClamp, reflectionVector);
	//	albedo += gMaterials[materialID].gMaterialReflection * reflectionColor;*/
	//	//float3 incident = -toEye;		// incident : 입사 벡터
	//	//float3 reflectionVector;
	//	////reflectionVector = reflect(incident, pin.NormalW);
	//	////reflectionVector = incident - 2.0f * dot(incident, pin.NormalW) * pin.NormalW;
	//	//reflectionVector = incident - 2.0f * dot(incident, NormalW) * NormalW;
	//	//float4 reflectionColor = gCubeMap.Sample(samLinearClamp, reflectionVector);
	//	//albedo += gMaterials[materialID].gMaterialReflection * reflectionColor;
	//	reflectionskyuv.y += waterTranslation;
	//	normalMap = ReflectionNomalMap.Sample(samAnisotropicWrap, reflectionskyuv);
	//	reflectionNormal = (normalMap.xyz * 2.0f) - 1.0f;
	//	reflectionskyuv = reflectionskyuv + (reflectionNormal.xy * reflectRefractScale);

	//	float3 retexcoord = float3(input.posW + (reflectionNormal.xy * reflectRefractScale), 1.0f);

	//	float4 reflectionColor;
	//	float4 reflectionObjectColor;

	//	reflectionColor = ReflectionSkyTexture.Sample(samLinearWrap, retexcoord);
	//	//reflectionColor = lerp(float4(ReflectionObjectTexture,1.0f),reflectionColor,0.15 );
	//	reflectionColor = ReflectionSkyTexture.Sample(samLinearWrap, reflectionskyuv);

	//	//color = lerp(float4(textureColor,1.0f), reflectionColor, 0.15f);
	//	albedo = lerp(albedo, reflectionColor, 0.2f);

	//	reflectionObjectColor = ReflectionObjectTexture.Sample(samLinearWrap, reflectionobjectuv);
	//	albedo = lerp(albedo, reflectionObjectColor, 0.2f);

	//}

	return albedo;
}