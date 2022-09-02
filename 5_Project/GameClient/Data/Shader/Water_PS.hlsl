
cbuffer cbMesh : register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint materialID;
}

cbuffer cbWater : register(b1)
{
	float4 refractionTint;
	float waterTranslation;
	float reflectRefractScale;
	float2 pad;
};

struct WaterPixelInput
{
	float4 pos : SV_POSITION;
	float3 posL : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 reflectionPos : TEXCOORD1;
	float4 refractionPos : TEXCOORD2;
};

struct PS_OUT
{
	float4 albedo : SV_Target0;
	float4 depth  : SV_Target1;
	float4 normal : SV_Target2;
	float4 pos	  : SV_Target3;
	uint materialID  : SV_Target4;
};

TextureCube reflectionTexture : register(t0);
TextureCube refractionTexture : register(t1);
Texture2D normalTexture : register(t2);

SamplerState samAnisotropicWrap : register(s0);
SamplerState samAnisotropicClamp : register(s1);
SamplerState samLinearClamp : register(s2);
SamplerState samLinearWrap : register(s3);

PS_OUT Water_PS(WaterPixelInput input) : SV_TARGET
{
	PS_OUT pout;

	float2 reflectTexCoord;
	float2 refractTexCoord;
	float4 normalMap;
	float3 normal;
	float4 reflectionColor;
	float4 refractionColor;
	float4 color;

	// 움직이는 물을 시뮬레이트하기 위해 물 수직선을 샘플링 한 위치를 이동한다.
	//input.tex.x += waterTranslation;
	input.tex.y += waterTranslation;

	// 투사 된 반사 텍스처 좌표를 계산한다. 
	reflectTexCoord.x = input.reflectionPos.x / input.reflectionPos.w / 2.0f + 0.5f;
	reflectTexCoord.y = input.reflectionPos.y / input.reflectionPos.w / 2.0f + 0.5f;

	// 투영 된 굴절 텍스처 좌표를 계산한다.
	refractTexCoord.x = input.refractionPos.x / input.refractionPos.w / 2.0f + 0.5f;
	refractTexCoord.y = input.refractionPos.y / input.refractionPos.w / 2.0f + 0.5f;


	// 노말 맵 텍스처로부터 법선을 샘플링한다.
	normalMap = normalTexture.Sample(samAnisotropicWrap, input.tex);
	//normalMap.a = 1.0f;

	//// 법선의 범위를 (0,1)에서 -1,+1로 확장
	normal = (normalMap.xyz * 2.0f) - 1.0f;

	// 텍스처 좌표 샘플링 위치를 노멀 맵 값으로 재배치해 파동 파 효과를 시뮬레이트함.
	reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
	refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);

	float3 retexcoord = float3(input.posL + (normal.xy * reflectRefractScale), 1.0f);
	float3 texcoord = float3(input.posL + (normal.xy * reflectRefractScale),0.5f);

	// 업데이트 된 텍스처 좌표를 사용하여 텍스처에서 텍스처 픽셀을 샘플링
	reflectionColor = reflectionTexture.Sample(samLinearClamp, input.posL);
	refractionColor = refractionTexture.Sample(samLinearWrap, retexcoord);

	//refractionColor = 1.0f;

	// 최종 색상의 반사 및 굴절 결과를 결합합니다.
	color = lerp(reflectionColor, refractionColor, 0.6f);
	//color = reflectionColor;
	//color = float4(normal, 1.0f) + reflectRefractScale - reflectRefractScale;

	input.normal = normalize(input.normal);

	pout.albedo = color;

	pout.normal = float4(input.normal, 1.0f);

	pout.depth = float4(input.pos.zzz, 1.0f);

	pout.pos = input.pos;

	pout.materialID = materialID;

	return pout;

}