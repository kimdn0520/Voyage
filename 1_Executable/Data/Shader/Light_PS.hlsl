#include "LightHeader.hlsli"

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
	float4 gMaterialAmbient;
	float4 gMaterialDiffuse;
	float4 gMaterialSpecular;
	float4 gMaterialReflection;

	bool isDiffuseTexture;			// hlsl에서 bool타입은 4byte 이므로 일로 구조체 넘겨줄때 BOOL로 넘겨줘야한대
	bool isNormalTexture;
	bool isSpecularTexture;
}

struct LightPixelIN
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
Texture2D gCubeMap : register(t3);
SamplerState samAnisotropicWrap : register(s0);
SamplerState samLinearWrap : register(s1);

float4 Light_PS(LightPixelIN input) : SV_Target
{
	input.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// 보간 때문에 법선이 더이상 단위벡터가 아닐 수 있으므로 다시 정규화 한다.
	input.normal = normalize(input.normal);

	input.tangent = normalize(input.tangent);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - input.posW;

	float distToEye = length(toEye);

	// normalize
	toEye /= distToEye;

	if (isDiffuseTexture)
	{
		input.color = gDiffuseMap.Sample(samAnisotropicWrap, input.uv);
	}

	// 0 <= r, g, b <= 1 을 만족하는 정규화된 성분들로 이루어진 (r,g,b)가 설정되었다. 
	// [0, 255] 구간의 정수를 255로 나누어서 [0, 1] 구간의 부동소수점 수로 만드는 작업이 된 것이다.
	// NormalSampleToWorldSpace 함수에서 단위벡터범위인 [-1, 1]로 비례 시켜준다.
	if (isNormalTexture)
	{
		float3 normalMapSample = gNormalMap.Sample(samLinearWrap, input.uv).rgb;
		input.normal = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
	}

	if (isSpecularTexture)
	{
		input.color.a = 1.0f;
	}

	input.color.a = 1.0f;

	// 초기화..
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;


	for (int i = 0; i < gDirLightCnt; i++)
	{
		ComputeDirectionalLight(gMaterialAmbient, gMaterialDiffuse, gMaterialSpecular, gMaterialReflection,
			gDirLight[i], input.normal, toEye,
			A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	for(int i = 0; i < gPointLightCnt; i++)
	{ 
		ComputePointLight(gMaterialAmbient, gMaterialDiffuse, gMaterialSpecular, gMaterialReflection,
			gPointLight[i], input.posW, input.normal, toEye,
			A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	for(int i = 0; i < gSpotLightCnt; i++)
	{ 
		ComputeSpotLight(gMaterialAmbient, gMaterialDiffuse, gMaterialSpecular, gMaterialReflection,
			gSpotLight[i], input.posW, input.normal, toEye,
			A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 litColor = input.color * (ambient + diffuse) + spec;
	litColor.a = gMaterialDiffuse.a * input.color.a;


	//litColor = float4(input.normal, 1.0f);     //빛받는거 확인용
	// 용책 참고 632쪽
	//float3 incident = -toEye;
	//float3 reflectionVector = reflect(incident, input.normal);
	//float4 reflectionColor = gCubeMap.Sample(samAnisotropicWrap, reflectionVector);
	//litColor += gMaterialReflection * reflectionColor;

	//litColor = pow(litColor, 1 / 2.2f);			// 감마컬렉션

	return litColor;
}