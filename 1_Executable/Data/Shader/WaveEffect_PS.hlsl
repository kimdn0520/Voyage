
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

	// 3���� ���� �ٸ� �ؽ��� ��ǥ�� ����Ͽ� ������ ������ �ؽ�ó�� ���ø��Ͽ�
	// 3���� �ٸ� ������ �������� ��´�.
	noise1 = noiseTexture.Sample(samLinearWrap, input.tex1);
	noise2 = noiseTexture.Sample(samLinearWrap, input.tex2);
	noise3 = noiseTexture.Sample(samLinearWrap, input.tex3);

	// (0,1) �������� (-1,1)������ ����� �̵�
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// 3���� ���� �ٸ� �ְ� x�� y������ �� ���� ������ x�� y��ǥ�� �ְ��մϴ�.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// �� ���� �ְ� �� ������ ����� ��� �ϳ��� ������ ����� �����մϴ�.
	finalNoise = noise1 + noise2 + noise3;

	// �ְ� ������ �� ���̾ ���� ���� �Է� �ؽ��� Y��ǥ�� �ְ��մϴ�.
	// ���� ȿ������ �Ҳ��� ���� �Ÿ��� �ؽ�ó�� ���� �����̸� ������ �������ϴ�.
	// �ٵ� �츮�� ���̴ϱ�.. �� �����غ���
	perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	// ���� ȭ�� ���� �ؽ�ó�� ���ø��ϴµ� ����� �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	// �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� �� �ؽ�ó���� ������ ���ø�
	waterColor = waveTexture.Sample(samLinearClamp, noiseCoords.xy);

	alphaColor = alphaTexture.Sample(samLinearClamp, noiseCoords.xy);

	// ���� ������ �Ҿ����ϰ� �ְ� �� ���� �ؽ��� ������ ����
	waterColor.a = alphaColor;

	//if(waterColor.a<0.5f)
	//	waterColor.a=0.f;
	return waterColor;
}