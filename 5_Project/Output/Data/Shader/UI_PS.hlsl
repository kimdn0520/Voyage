
cbuffer cbUI : register(b0)
{
	bool isAlpha;
	float alpha;
};

struct UIPixelIN
{
	float4 pos     : SV_POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

Texture2D gDiffuseMap : register(t0);
SamplerState samLinearWrap : register(s3);

float4 UI_PS(UIPixelIN input) : SV_Target
{
	input.color = gDiffuseMap.Sample(samLinearWrap, input.uv);

	// HLSL�� clip() �Լ��� ���ڷ� �޴� �����Ͱ� 0���� ������ �ش� �����͸� ����Ѵ�
	// clip(color.a - 0.9f);

	if(input.color.a < 0.2f)
		discard;

	if (isAlpha)
	{
		input.color.a = alpha;
	}

	return input.color;
}