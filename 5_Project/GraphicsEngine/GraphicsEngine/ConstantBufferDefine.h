#pragma once
#include "SimpleMath.h"
using namespace SimpleMath;

#define DIRECTION_LIGHT_COUNT 1
#define POINT_LIGHT_COUNT 30
#define SPOT_LIGHT_COUNT 30
#define Material_Count_ID 50

//---------------------------------------------------------------------------------------------------------
// 쉐이더에 필요한 struct
//---------------------------------------------------------------------------------------------------------
struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	Vector3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	// 셰이더에 들어갈때 4차원 벡터에 하나씩 박히나봐욤 331쪽 참고
	Vector3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	Vector3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	Vector3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
	// Vector3면 빈 pad를 넣어줌
};

__declspec(align(16)) struct MaterialCollection
{
	MaterialCollection() { ZeroMemory(this, sizeof(this)); }
	 
	Vector4 gMaterialAmbient;
	Vector4 gMaterialDiffuse;
	Vector4 gMaterialSpecular;
	Vector4 gMaterialReflection;

	BOOL isDiffuseTexture;			// hlsl에서 bool타입은 4byte 이므로 일로 구조체 넘겨줄때 BOOL로 넘겨줘야한대
	BOOL isNormalTexture;
	BOOL isSpecularTexture;
	BOOL isReflectionTexture;
	BOOL isUtil;
};

//---------------------------------------------------------------------------------------------------------
// 쉐이더에 필요한 ConstantBuffer들 이름이랑 내용을 맞춰주아야한다.
//---------------------------------------------------------------------------------------------------------
__declspec(align(16)) struct cbMesh
{
	Matrix gWorld;
	Matrix gProj;
	Matrix gWorldViewProj;
	Matrix gWorldInvTranspose;

	unsigned int objectID;
};

__declspec(align(16)) struct cbLight
{
	DirectionalLight gDirLight[DIRECTION_LIGHT_COUNT];
	PointLight gPointLight[POINT_LIGHT_COUNT];
	SpotLight gSpotLight[SPOT_LIGHT_COUNT];

	Vector3 gEyePosW;	// CameraPos

	int gDirLightCnt;
	int gPointLightCnt;
	int gSpotLightCnt;
};

__declspec(align(16)) struct cbMaterial
{
	MaterialCollection gMaterials[Material_Count_ID];

	//Vector4 gMaterialAmbient;
	//Vector4 gMaterialDiffuse;
	//Vector4 gMaterialSpecular;
	//Vector4 gMaterialReflection;

	//BOOL isDiffuseTexture;			// hlsl에서 bool타입은 4byte 이므로 일로 구조체 넘겨줄때 BOOL로 넘겨줘야한대
	//BOOL isNormalTexture;
	//BOOL isSpecularTexture;
	//BOOL isUtil;
};	

__declspec(align(16)) struct cbSkinned
{
	Matrix gBoneTransforms[96];
};

__declspec(align(16)) struct cbTexture
{
	Vector4 textureInfo;
};

__declspec(align(16)) struct cbReflection
{
	Matrix gReflection;
};

__declspec(align(16)) struct cbWater
{
	Vector4 refractionTint;
	float uvScale;
	float reflectRefractScale;
	Vector2 pad;
};


__declspec(align(16)) struct cbUI
{
	BOOL isAlpha;

	float alpha;
};

__declspec(align(16)) struct cbOcean
{
	float waterTranslation;
};

__declspec(align(16)) struct cbWave
{
	Vector2 WaveTime;	// 시간
};

__declspec(align(16)) struct cbNoise
{
	float frameTime;
	Vector3 scrollSpeed;
	Vector3 scales;
	float padding;
};


__declspec(align(16)) struct cbDistortion
{
	Vector2 distortion1;
	Vector2 distortion2;
	Vector2 distortion3;
	float distortionScale;
	float distortionBias;
};

