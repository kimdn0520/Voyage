
cbuffer cbDistortion	: register(b0)
{
	float2 distortion1;
	float2 distortion2;
	float2 distortion3;
	float distortionScale;
	float distortionBias;
};


struct WaveVertexInput
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float2 tex1		: TEXCOORD1;
	float2 tex2		: TEXCOORD2;
	float2 tex3		: TEXCOORD3;
};

Texture2D waveTexture : register(t0);
Texture2D noiseTexture : register(t1);
Texture2D alphaTexture : register(t2);

SamplerState samAnisotropicWrap : register(s0);
SamplerState samAnisotropicClamp : register(s1);
SamplerState samLinearClamp : register(s2);
SamplerState samLinearWrap : register(s3);

float4 WaveEffect_PS(WaveVertexInput input) : SV_TARGET
{
	float4 noise1;
	float4 noise2;
	float4 noise3;
	float4 finalNoise;
	float perturb;
	float2 noiseCoords;
	float4 waterColor;
	float4 alphaColor;

	// 3개의 서로 다른 텍스쳐 좌표를 사용하여 동일한 노이즈 텍스처를 샘플링하여
	// 3개의 다른 노이즈 스케일을 얻는다.
	noise1 = noiseTexture.Sample(samLinearWrap, input.tex1);
	noise2 = noiseTexture.Sample(samLinearWrap, input.tex2);
	noise3 = noiseTexture.Sample(samLinearWrap, input.tex3);

	// (0,1) 범위에서 (-1,1)범위로 노이즈를 이동
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// 3개의 서로 다른 왜곡 x와 y값으로 세 개의 노이즈 x와 y좌표를 왜곡합니다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// 세 가지 왜곡 된 노이즈 결과를 모두 하나의 노이즈 결과로 결합합니다.
	finalNoise = noise1 + noise2 + noise3;

	// 왜곡 스케일 및 바이어스 값에 의해 입력 텍스쳐 Y좌표를 왜곡합니다.
	// 위쪽 효과에서 불꽃이 깜박 거리는 텍스처를 위로 움직이면 섭동이 강해집니다.
	// 근데 우리는 물이니깐.. 뭐 적용해보자
	perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	// 이제 화재 색상 텍스처를 샘플링하는데 사용할 교란되고 왜곡 된 텍스처 샘플링 좌표를 만듬
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	// 섭동되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 물 텍스처에서 색상을 샘플링
	waterColor = waveTexture.Sample(samLinearClamp, noiseCoords.xy);

	alphaColor = alphaTexture.Sample(samLinearClamp, noiseCoords.xy);

	// 알파 블렌딩을 불안정하고 왜곡 된 알파 텍스쳐 값으로 설정
	waterColor.a = alphaColor;

	//if(waterColor.a<0.5f)
	//	waterColor.a=0.f;
	return waterColor;
}