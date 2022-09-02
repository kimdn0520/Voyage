
cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;
};

struct BasicVertexIN
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

struct BasicVertexOut
{
	float4 pos     : SV_POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
};

BasicVertexOut Basic_VS(BasicVertexIN input)
{
	BasicVertexOut output;

	output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);

	output.color = input.color;

	output.uv = input.uv;
	
	return output;
}
