// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS

#include <memory.h>
#include <array>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <filesystem>

#include <atltrace.h>
#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <WICTextureLoader.h>
#include <Directxmath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include "DirectXTex.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

// using
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::PackedVector;
using namespace std;
using namespace SimpleMath;
namespace fs = std::filesystem;

// lib
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib") 
#pragma comment(lib, "d3dcompiler.lib")

// const
const bool FULL_SCREEN = false;

// 클라이언트에서 넘겨줌
struct WindowInfo
{
	HWND	hwnd;		// 출력 윈도우 핸들
	__int32	width;  // 너비
	__int32	height; // 높이
	bool	windowed;		// 창모드 or 전체화면
};

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

enum class OBJECT_TYPE
{
	DEFAULT,
	UTIL,
	SKY_BOX,
	WATER,
	PARTICLE,
	WaveEffect,
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

enum class ButtonStatus
{
	NORMAL,		// 일반(아무것도 아닌 상태)
	HIGHLIGHT,	// 마우스가 위에 있을 때
	PRESSED,	// 눌렸을때
	PRESSEDUP,	// 누르고 땠을때
};

struct MeshInfo
{
	size_t name;	
	DirectX::SimpleMath::Matrix worldTM;
	DirectX::SimpleMath::Matrix finalBoneListMatrix[96];
	DirectX::SimpleMath::Vector3 worldPos;
	// vector<shared_ptr<Material>> materials;
	unsigned int materialID;

	bool isSkinned = false;
	bool isBone = false;				// 본인지 아닌지
	bool isBloom = false;				// Bloom을 받는지 아닌지
	bool isParticle = false;
	OBJECT_TYPE type = OBJECT_TYPE::DEFAULT;

	int particleIdx;

};

struct ParticleType
{
	float positionX, positionY, positionZ;
	float red, green, blue;
	float velocity;
	bool isActive;
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

	DirectX::SimpleMath::Vector3 att;
	float		range;
	float		spot;
};

#endif //PCH_H
