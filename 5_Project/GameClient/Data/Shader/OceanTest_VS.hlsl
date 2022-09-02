
cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;
}

cbuffer cbReflection : register(b1)
{
	float4x4 gReflection;
}

cbuffer cbWave : register(b2)
{
	float2 WaveTime;	// 시간
}

struct OceanVertexIn
{
	float3 pos		: POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
};

struct OceanVertexOut
{
	float4 pos		: SV_POSITION;
	float3 posW			 : POSITION;
	float4 color	: COLOR;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
	float4 reflectionPos : TEXCOORD1;
};


OceanVertexOut OceanTest_VS(OceanVertexIn input)
{
	OceanVertexOut output;

	float PI = 3.141592;

	float3 gridPoint = input.pos;
	float3 tangent = float3(1, 0, 0);
	float3 binormal = float3(0, 0, 1);
	float3 p = gridPoint;

	//// 물결수 만큼
	//// dir,steepness,wavelength,... 로 이루어져 있음
	//float4 _waveA = float4(1, 1, 0.25f, 60);
	//float4 _waveB = float4(1, 0.6, 0.25, 31);
	//float4 _waveC = float4(1, 1.3, 0.25, 18);

	//// steepness
	//float waveAsteepness = _waveA.z;
	//float waveBsteepness = _waveB.z;
	//float waveCsteepness = _waveC.z;

	//// wavelength
	//float waveAlength = _waveA.w;
	//float waveBlength = _waveB.w;
	//float waveClength = _waveC.w;

	//// wave K
	//float waveAk = (2 * PI) / waveAlength;
	//float waveBk = (2 * PI) / waveBlength;
	//float waveCk = (2 * PI) / waveClength;

	//// wave C
	//float waveAc = sqrt(9.8 / waveAk);
	//float waveBc = sqrt(9.8 / waveBk);
	//float waveCc = sqrt(9.8 / waveCk);

	//// wave D
	//float2 waveAd = normalize(_waveA.xy);;
	//float2 waveBd = normalize(_waveB.xy);;
	//float2 waveCd = normalize(_waveC.xy);;

	//// wave F
	//float waveAf = waveAk * (dot(waveAd, input.pos.xz) - waveAc * WaveTime.y);
	//float waveBf = waveBk * (dot(waveBd, input.pos.xz) - waveBc * WaveTime.y);
	//float waveCf = waveCk * (dot(waveCd, input.pos.xz) - waveCc * WaveTime.y);

	//// wave A
	//float waveAa= waveAsteepness / waveAk;
	//float waveBa= waveBsteepness / waveBk;
	//float waveCa= waveCsteepness / waveCk;

	//// A
	//tangent += float3(
	//	-waveAd.x * waveAd.x * (waveAsteepness * sin(waveAf)),
	//	waveAd.x * (waveAsteepness * cos(waveAf)),
	//	-waveAd.x * waveAd.y * (waveAsteepness * sin(waveAf))
	//	);
	//binormal += float3(
	//	-waveAd.x * waveAd.y * (waveAsteepness * sin(waveAf)),
	//	waveAd.y * (waveAsteepness * cos(waveAf)),
	//	-waveAd.y * waveAd.y * (waveAsteepness * sin(waveAf))
	//	);

	//p += float3(
	//	waveAd.x * (waveAa * cos(waveAf)),
	//	waveAa * sin(waveAf),
	//	waveAd.y * (waveAa * cos(waveAf))
	//	);

	////// B
	//tangent += float3(
	//	-waveBd.x * waveBd.x * (waveBsteepness * sin(waveBf)),
	//	waveBd.x * (waveBsteepness * cos(waveBf)),
	//	-waveBd.x * waveBd.y * (waveBsteepness * sin(waveBf))
	//	);
	//binormal += float3(
	//	-waveBd.x * waveBd.y * (waveBsteepness * sin(waveBf)),
	//	waveBd.y * (waveBsteepness * cos(waveBf)),
	//	-waveBd.y * waveBd.y * (waveBsteepness * sin(waveBf))
	//	);

	//p += float3(
	//	waveBd.x * (waveBa * cos(waveBf)),
	//	waveBa * sin(waveBf),
	//	waveBd.y * (waveBa * cos(waveBf))
	//	);

	//// C
	//tangent += float3(
	//	-waveCd.x * waveCd.x * (waveCsteepness * sin(waveCf)),
	//	waveCd.x * (waveCsteepness * cos(waveCf)),
	//	-waveCd.x * waveCd.y * (waveCsteepness * sin(waveCf))
	//	);
	//binormal += float3(
	//	-waveCd.x * waveCd.y * (waveCsteepness * sin(waveCf)),
	//	waveCd.y * (waveCsteepness * cos(waveCf)),
	//	-waveCd.y * waveCd.y * (waveCsteepness * sin(waveCf))
	//	);

	//p += float3(
	//	waveCd.x * (waveCa * cos(waveCf)),
	//	waveCa * sin(waveCf),
	//	waveCd.y * (waveCa * cos(waveCf))
	//	);

	float3 normal = normalize(cross(binormal, tangent));
	//output.pos.xyz = p;

	float4 _pos = mul(float4(p, 1.0f), gWorldViewProj);
	float4x4 reflectprojworld = mul(gReflection, gProj);
	reflectprojworld = mul(gWorld, reflectprojworld);

	output.pos = _pos;
	output.normal = normal;

	//output.normal = normalize(cross(binormal,tangent));	// 이부분을 이용하여 조명에 사용하는듯 법선벡터
	output.color = input.color;
	output.tex = input.uv;

	// 반사 관련을 넘겨줌
	output.reflectionPos = mul(_pos, reflectprojworld);
	output.posW = mul(float4(input.pos, 1.0f), gWorld).xyz;

	return output;
}