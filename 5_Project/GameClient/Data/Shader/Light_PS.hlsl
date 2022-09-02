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

	bool isDiffuseTexture;			// hlsl���� boolŸ���� 4byte �̹Ƿ� �Ϸ� ����ü �Ѱ��ٶ� BOOL�� �Ѱ�����Ѵ�
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

	// ���� ������ ������ ���̻� �������Ͱ� �ƴ� �� �����Ƿ� �ٽ� ����ȭ �Ѵ�.
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

	// 0 <= r, g, b <= 1 �� �����ϴ� ����ȭ�� ���е�� �̷���� (r,g,b)�� �����Ǿ���. 
	// [0, 255] ������ ������ 255�� ����� [0, 1] ������ �ε��Ҽ��� ���� ����� �۾��� �� ���̴�.
	// NormalSampleToWorldSpace �Լ����� �������͹����� [-1, 1]�� ��� �����ش�.
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

	// �ʱ�ȭ..
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


	//litColor = float4(input.normal, 1.0f);     //���޴°� Ȯ�ο�
	// ��å ���� 632��
	//float3 incident = -toEye;
	//float3 reflectionVector = reflect(incident, input.normal);
	//float4 reflectionColor = gCubeMap.Sample(samAnisotropicWrap, reflectionVector);
	//litColor += gMaterialReflection * reflectionColor;

	//litColor = pow(litColor, 1 / 2.2f);			// �����÷���

	return litColor;
}