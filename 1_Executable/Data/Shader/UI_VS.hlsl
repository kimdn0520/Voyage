
cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;
};

struct UIVertexIN
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

struct UIVertexOut
{
	float4 pos     : SV_POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

UIVertexOut UI_VS(UIVertexIN input)
{
	UIVertexOut output;

	//output.pos = float4(input.pos, 1.0f);

	output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);

	output.color = input.color;

	output.uv = input.uv;

	return output;
}