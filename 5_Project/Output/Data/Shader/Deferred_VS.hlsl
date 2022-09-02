
struct DeferredVertexIn
{
	float3 pos     : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};

struct DeferredVertexOut
{
	float4 pos     : SV_POSITION;
	float3 posW	   : POSITION;
	float4 color   : COLOR;
	float2 uv	   : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
};

DeferredVertexOut Deferred_VS(DeferredVertexIn input)
{
	DeferredVertexOut output;
	
	output.pos = float4(input.pos, 1.0f).xyww;

	output.uv = input.uv;

	return output;
}

