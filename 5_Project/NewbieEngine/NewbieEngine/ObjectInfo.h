#pragma once
#include "NewbieEngineDLL.h"
#include <SimpleMath.h>

//---------------------------------------------------------------------------------------------------------
// MeshRenderInfo
//---------------------------------------------------------------------------------------------------------
enum class RasterState
{
	WIRE,
	SOLID,
};

enum class TOPOLOGY {
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
};

//---------------------------------------------------------------------------------------------------------
// MeshInfo
//---------------------------------------------------------------------------------------------------------

enum class MeshName
{
	CubeMesh,
	SphereMesh,
	AxisMesh,
	GridMesh,
	ScreenMesh,
	RectMesh,
	WireRectMesh,
	WireGreenCubeMesh,
	WireRedCubeMesh,
	WireGreenSphereMesh,
	WireRedSphereMesh,
	CircleMesh,
	OceanMesh,
	ParticleMesh,
	WaveMesh,
};

struct KeyFrameInfo
{
	float 		time;
	Vector3		scale;
	Vector3		rotation;
	Vector3		translate;
};

struct BoneInfo
{
	size_t		boneName;
};

struct Animation_p
{
	int			tick;
	Vector4		pos;
};

struct Animation_r
{
	int			tick;
	Vector4		rotQT;		// 상대변량이기 때문에 이전 값들과 누적해서 가지고 있는다 (즉 지금까지의 절대량으로 변환한다)
};

struct Animation_s
{
	int			time;
	Vector3		scale;
	Vector3		scaleaxis;
	float		scaleaxisang;
	//Quaternion	m_scalerotQT_accumulation;
};

struct AnimationClipInfo
{
	std::size_t		nodename;
	std::size_t		animName;
	std::vector<std::shared_ptr<Animation_p>>		position;
	std::vector<std::shared_ptr<Animation_r>>		rotation;
	std::vector<std::shared_ptr<Animation_s>>		scale;

	// 애니메이션 firstframe
	int firstFrame;

	// 애니메이션 총 프레임 수
	int totalFrame;

	// 애니메이션 초당 프레임 수
	int frameSpeed;

	// 프레임당 몇 Tick이 지나가는지
	int ticksPosPerFrame;

	// 프레임당 몇 Tick이 지나가는지
	int ticksRotPerFrame;
};

struct MaterialInfo
{
	bool isDiffuse = false;
	bool isNormal = false;
	bool isSpecular = false;
	bool isReflection = false;
	bool isUtil = false;

	wstring diffuseTexture;
	wstring normalTexture;
	wstring specularTexture;
	wstring cubeMapTexture;

	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 diffuse;
	DirectX::SimpleMath::Vector4 specular;
	DirectX::SimpleMath::Vector4 reflection;
};


struct ParticleType
{
	float positionX, positionY, positionZ;
	float red, green, blue;
	float velocity;
	bool isActive;
};

enum class OBJECT_TYPE
{
	DEFAULT,
	UTIL,
	SKY_BOX,
	WATER,
	PARTICLE,
	WaveEffect
};

enum class ColliderType
{
	Box,
	Sphere,
};

enum class UIType
{
	Panel,
	Button,
};

enum class BodyType
{
	Dynamic,
	Kinematic,
	Static,
};

enum class ButtonStatus
{
	NORMAL,		// 일반(아무것도 아닌 상태)
	HIGHLIGHT,	// 마우스가 위에 있을 때
	PRESSED,	// 눌렸을때
	PRESSEDUP,	// 누르고 땠을때
};

struct MeshInfo
{
	size_t name;									// 어떤 메시를 쓸지 그래픽스에게 알려줌
	DirectX::SimpleMath::Matrix worldTM;
	DirectX::SimpleMath::Matrix finalBoneListMatrix[96];
	DirectX::SimpleMath::Vector3 worldPos;

	// vector<shared_ptr<Material>> materials;
	unsigned int materialID;

	bool isSkinned = false;						// 본들을 가지고 있는 메시인지..
	bool isBone = false;						// 본인지 아닌지
	bool isBloom = false;						// Bloom을 받는지 아닌지
	bool isParticle = false;
	OBJECT_TYPE type = OBJECT_TYPE::DEFAULT;

	int particleIdx;

};

struct TextInfo
{
	int x;
	int y;
	string text;
	Vector4 color;
	float scale;
};

struct UIInfo
{
	UIType uiType;

	int sortLayer;

	DirectX::SimpleMath::Matrix worldTM;

	float left_Offset;
	float right_Offset;
	float top_Offset;
	float bottom_Offset;
	float width;
	float height;

	ButtonStatus status;
	wstring normalImage;
	wstring highLightImage;
	wstring pressedImage;

	wstring panelImage;
	float alpha;
	bool isAlpha;
};

struct ColDebugInfo
{
	ColliderType type;

	bool isCol;

	DirectX::SimpleMath::Matrix worldTM;
};

struct CameraInfo
{
	DirectX::SimpleMath::Vector3 worldPos;

	DirectX::SimpleMath::Matrix viewTM;
	DirectX::SimpleMath::Matrix projTM;
	DirectX::SimpleMath::Matrix orthoProjTM;
	DirectX::SimpleMath::Matrix reflectionTM;
	DirectX::SimpleMath::Vector3 worldRotation;
	DirectX::SimpleMath::Vector3 right;
	DirectX::SimpleMath::Vector3 up;
	DirectX::SimpleMath::Vector3 look;

};
//
//struct CubeMapCamera
//{
//	CameraInfo cameraInfo[6];
//};

struct LightInfo
{
	int 		lightType;
	size_t		hashcode;

	//color
	DirectX::SimpleMath::Vector4	diffuse;
	DirectX::SimpleMath::Vector4	ambient;
	DirectX::SimpleMath::Vector4	specular;

	DirectX::SimpleMath::Vector3	position;
	DirectX::SimpleMath::Vector3	direction;

	DirectX::SimpleMath::Vector3	att;
	float		range;
	float		spot;
};

//---------------------------------------------------------------------------------------------------------
// VertexInfo
//---------------------------------------------------------------------------------------------------------
struct GraphicsVertex
{
	GraphicsVertex()
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0;
		}
	}
	GraphicsVertex(DirectX::SimpleMath::Vector3 paPos,
		DirectX::SimpleMath::Vector4 paColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		DirectX::SimpleMath::Vector2 paUV = { 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paNormal = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paTangent = { 0.f, 0.f, 0.f }) 
	{
		pos = paPos; color = paColor; uv = paUV; normal = paNormal; tangent = paTangent;
		
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0; 
		}
	}

	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector3 tangent;

	// Skinned Data
	float weights[8];
	unsigned __int32 boneIndices[8];
};