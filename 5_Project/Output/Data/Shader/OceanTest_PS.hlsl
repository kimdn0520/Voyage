#include "MaterialHeader.hlsli"

cbuffer cbMesh	:	register(b0)
{
	float4x4 gWorld;
	float4x4 gProj;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;

	uint materialID;
};

cbuffer cbOcean : register(b1)
{
	float waterTranslation;
}

cbuffer cbMaterial : register(b2)
{
	MaterialCollection gMaterials[Material_Count_ID];
}

cbuffer cbWater : register(b3)
{
	float4 refractionTint;
	float uvScale;
	float reflectRefractScale;
	float2 pad;
};

struct OceanPixelIN
{
	float4 pos			 : SV_POSITION;
	float3 posW			 : POSITION;
	float4 color		 : COLOR;
	float3 normal		: NORMAL;
	float2 tex	   		: TEXCOORD0;
	float4 reflectionPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 albedo : SV_Target0;
	float4 depth  : SV_Target1;
	float4 normal : SV_Target2;
	float4 pos	  : SV_Target3;
	uint materialID  : SV_Target4;
};

//Texture2D reflectionTexture;
Texture2D ReflectionSkyTexture : register(t0);
Texture2D ReflectionNomalMap : register(t1);
Texture2D ReflectionObjectTexture : register(t2);


SamplerState samAnisotropicClamp : register(s0);
SamplerState samAnisotropicWrap : register(s1);
SamplerState samLinearWrap : register(s2);


float2 mat2(float2 uv)
{
	float2 arr1 = float2(1.6f, 1.2f);
	float2 arr2 = float2(-1.2f, 1.6f);
	float2 answer;
	answer.x = (uv.x * arr1.x) + (uv.y * arr2.x);
	answer.y = (uv.x * arr1.y) + (uv.y * arr2.y);

	return answer;
}

float3 getSkyColor(float3 e)
{
	e.y = (max(e.y, 0.0f) * 0.8f + 0.2f) * 0.8f;
	return float3(pow(1.0f - e.y, 2.0f), 1.0f - e.y, 0.6f + (1.0f - e.y) * 0.4f) * 1.1f;

}

float specular(float3 n, float3 l, float3 e, float s) {
	float nrm = (s + 8.0) / (3.141592 * 8.0);
	return pow(max(dot(reflect(e, n), l), 0.0), s) * nrm;
}

float diffuse(float3 n, float3 l, float p) {
	return pow(dot(n, l) * 0.4 + 0.6, p);
}

float3 getSeaColor(float3 p, float3 n, float3 l, float3 eye, float3 dist) {
	float fresnel = clamp(1.0 - dot(n, -eye), 0.0, 1.0);
	fresnel = pow(fresnel, 3.0) * 0.5;

	float3 reflected = getSkyColor(reflect(eye, n));
	float3 refracted = (float3(0.0, 0.09, 0.18)) + diffuse(n, 1, 10) * (float3(0.8, 0.9, 0.7) * 0.9) * 0.12;
	// 중간에 diffuse가 들어간다(n,1,80)

	float3 color = lerp(refracted, reflected, fresnel); // 선형보간 

	float atten = max(1.0 - dot(dist, dist) * 0.001, 0.0);
	color += float3(0.8f, 0.9f, 0.6f) * (p.y - 0.6) * 0.18 * atten;

	//float3 v = float3(1.0f, 1.0f, 1.0f);
	float specularnum = specular(n, 1, eye, 60.0f);

	color += float3(specularnum, specularnum, specularnum);

	return color;
}

float hash(float2 p)
{
	float h = dot(p, float2(127.1, 311.7));
	return frac(sin(h) * 43758.5453123);
}

float noise(float2 p)
{
	float2 i = floor(p);
	float2 f = frac(p);
	float2 u = f * f * (3.0 - 2.0 * f);
	return -1.0 + 2.0 * lerp(lerp(hash(i + float2(0.0f, 0.0f)),
		hash(i + float2(1.0, 0.0)), u.x),
		lerp(hash(i + float2(0.0f, 1.0f)),
			hash(i + float2(1.0f, 1.0f)), u.x), u.y);
}

float3x3 fromEuler(float3 angle)
{
	float2 a1 = float2(sin(angle.x), cos(angle.x));
	float2 a2 = float2(sin(angle.y), cos(angle.y));
	float2 a3 = float2(sin(angle.z), cos(angle.z));

	float3x3 mat;
	mat[0] = float3(a1.y * a3.y + a1.x * a2.x * a3.x, a1.y * a2.x * a3.x + a3.y * a1.x, -a2.y * a3.x);
	mat[1] = float3(-a2.y * a1.x, a1.y * a2.y, a2.x);
	mat[2] = float3(a3.y * a1.x * a2.x + a1.y * a3.x, a1.x * a3.x - a1.y * a3.y * a2.x, a2.y * a3.y);
	return mat;
}

float sea_octave(float2 uv, float choppy) {
	uv += noise(uv);
	float2 wv = 1.0 - abs(sin(uv));
	float2 swv = abs(cos(uv));
	wv = lerp(wv, swv, wv);
	return pow(1.0 - pow(wv.x * wv.y, 0.65), choppy);
}

float map(float3 p) {
	float freq = 0.16;
	float amp = 0.6;
	float choppy = 4.0;
	float2 uv = p.xz; uv.x *= 0.75;

	float d, h = 0.0;
	for (int i = 0; i < 3; i++) {
		d = sea_octave((uv + (1.0 + waterTranslation * 0.8)) * freq, choppy);
		d += sea_octave((uv - 0.8) * freq, choppy);
		h += d * amp;
		uv = mat2(uv);
		freq *= 1.9;
		amp *= 0.22;
		choppy = lerp(choppy, 1.0, 0.2);
	}
	return p.y - h;
}

float heightMapTracing(float3 ori, float3 dir, float3 p) {
	float tm = 0.0;
	float tx = 1000.0;
	float hx = map(ori + dir * tx);
	if (hx > 0.0) {
		p = ori + dir * tx;
		return tx;
	}
	float hm = map(ori + dir * tm);
	float tmid = 0.0;
	for (int i = 0; i < 8; i++) {
		tmid = lerp(tm, tx, hm / (hm - hx));
		p = ori + dir * tmid;
		float hmid = map(p);
		if (hmid < 0.0) {
			tx = tmid;
			hx = hmid;
		}
		else {
			tm = tmid;
			hm = hmid;
		}
	}
	return tmid;
}

float map_detailed(float3 p) {
	float freq = 0.16;
	float amp = 0.6;
	float choppy = 4.0;
	float2 uv = p.xz; uv.x *= 0.75;

	float d, h = 0.0;
	for (int i = 0; i < 5; i++) {
		d = sea_octave((uv + (1.0 + waterTranslation * 0.8)) * freq, choppy);
		d += sea_octave((uv - (1.0 + waterTranslation * 0.8)) * freq, choppy);
		h += d * amp;
		uv = mat2(uv); freq *= 1.9; amp *= 0.22;
		choppy = lerp(choppy, 1.0, 0.2);
	}
	return p.y - h;
}

// tracing
float3 getNormal(float3 p, float eps) {
	float3 n;
	n.y = map_detailed(p);
	n.x = map_detailed(float3(p.x + eps, p.y, p.z)) - n.y;
	n.z = map_detailed(float3(p.x, p.y, p.z + eps)) - n.y;
	n.y = eps;
	return normalize(n);
}

float3 getPixel(float2 coord, float time)
{
	float3 iResolution = float3(1.f, 5.0f, 0.0f);

	float2 uv = coord / iResolution.xy;
	uv = uv * 2.0 - 1.0;
	uv.x *= iResolution.x / iResolution.y;

	// ray
	float3 ang = float3(sin(time * 3.0) * 0.1, sin(time) * 0.2 + 0.3, time);
	float3 ori = float3(0.0, 3.5, time * 5.0);
	float3 dir = normalize(float3(uv.xy, -2.0)); dir.z += length(uv) * 0.14;
	float3 normaldir = normalize(dir);
	float3x3 eulerout = fromEuler(ang);
	dir.x = (normaldir.x * eulerout[0][0]) + (normaldir.y * eulerout[1][0]) + (normaldir.z * eulerout[2][0]);
	dir.y = (normaldir.x * eulerout[0][1]) + (normaldir.y * eulerout[1][1]) + (normaldir.z * eulerout[2][1]);
	dir.z = (normaldir.x * eulerout[0][2]) + (normaldir.y * eulerout[1][2]) + (normaldir.z * eulerout[2][2]);
	//dir = normalize(dir) * fromEuler(ang);

	// tracing
	float3 p;
	//    heightMapTracing(ori, dir, p);
	float tm = 0.0;
	float tx = 1000.0;
	float hx = map(ori + dir * tx);
	if (hx > 0.0) {
		p = ori + dir * tx;
	}
	float hm = map(ori + dir * tm);
	float tmid = 0.0;
	for (int i = 0; i < 8; i++) {
		tmid = lerp(tm, tx, hm / (hm - hx));
		p = ori + dir * tmid;
		float hmid = map(p);
		if (hmid < 0.0) {
			tx = tmid;
			hx = hmid;
		}
		else {
			tm = tmid;
			hm = hmid;
		}
	}
	//
	float3 dist = p - ori;
	float3 n = getNormal(p, dot(dist, dist) * (0.1 / 30.f));
	float3 light = normalize(float3(0.0, 1.0, 0.8));

	// color
	return lerp(
		getSkyColor(dir),
		getSeaColor(p, n, light, dir, dist),
		pow(smoothstep(0.0, -0.02, dir.y), 0.2));
}

PS_OUT OceanTest_PS(OceanPixelIN input) : SV_TARGET
{
	PS_OUT pout;

	float2 RotationUV = input.tex;
	RotationUV.x = 1 - input.tex.x;

	float3 color = getPixel(input.tex, 1.0f);

	if (gMaterials[materialID].isReflectionTexture)
	{
		float4 normalMap;
		float3 reflectionNormal;
		float2 reflectionskyuv = input.tex;
		float2 reflectionobjectuv = RotationUV;
		reflectionskyuv.x += uvScale;
	/*	reflectionobjectuv.x += 15;*/
		normalMap = ReflectionNomalMap.Sample(samAnisotropicWrap, reflectionskyuv);
		reflectionNormal = (normalMap.xyz * 2.0f) - 1.0f;
		reflectionskyuv = reflectionskyuv + (reflectionNormal.xy * reflectRefractScale);
		reflectionobjectuv = reflectionobjectuv + (reflectionNormal.xy * 0.01f);	// 여기서 그림자 일렁이는 수치를 조절가능

		float3 retexcoord = float3(input.posW + (reflectionNormal.xy * reflectRefractScale), 1.0f);

		float4 reflectionColor;
		float4 reflectionObjectColor;

		reflectionColor = ReflectionSkyTexture.Sample(samLinearWrap, retexcoord);
		reflectionColor = ReflectionSkyTexture.Sample(samLinearWrap, reflectionskyuv);

		color = lerp(color, reflectionColor, 0.25f);

		reflectionObjectColor = ReflectionObjectTexture.Sample(samLinearWrap, reflectionobjectuv);
		color = lerp(color, reflectionObjectColor, 0.2f);
	}

	pout.albedo = float4(color,1.0f);

	pout.normal = float4(input.normal,1.f);

	pout.depth = float4(input.pos.zzz, 5.0f);

	pout.pos = input.pos;

	pout.materialID = materialID;

	return pout;
}