#pragma once
#include "AlignedAllocationPolicy.h"
#include "NewbieEngineDLL.h"
#include "Component.h"
#include "ObjectInfo.h"

/// <summary>
/// Ambient : 광원이 방출하는 주변광의 양
/// Diffuse : 광원이 방출하는 분산광의 양(확산광)
/// Specular : 광원이 방출하는 반영광의 양(반사광)
/// Direction : 빛의 방향
/// Position : 광원의 위치
/// Range : 빛의 범위, 광원과의 거리가 이 범위보다 큰 점은 빛을 받지 않음
/// Attenuation : 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 세 개의
///				  감쇠 상수들을(a0, a1, a2)의 형태로 저장한다.
/// Spot : 점적광 계산에서 점적광 원뿔을 제어하는데 쓰이는 지수
/// Pad : 필요하다면 빛들의 배열을 설정할 수 있도록 마지막에 float 하나의 자리를 하나 마련해 둔다.
/// </summary>

class Transform;

enum class LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};
	
class Light : public Component, public AlignedAllocationPolicy<16>
{
public:
	NewbieEngine_DLL Light(shared_ptr<GameObject> gameObject);
	virtual ~Light();

private:
	LIGHT_TYPE _type = LIGHT_TYPE::DIRECTIONAL_LIGHT;

	LightInfo _lightInfo = {};

	shared_ptr<Transform> _transform;

	Vector3 _direction;

public:
	NewbieEngine_DLL void SetLightType(LIGHT_TYPE type);
	
	NewbieEngine_DLL void SetLightDirection(const Vector3& direction);
	
	NewbieEngine_DLL void SetDiffuse(const Vector4& diffuse) { _lightInfo.diffuse = diffuse; }
	NewbieEngine_DLL void SetAmbient(const Vector4& ambient) { _lightInfo.ambient = ambient; }
	NewbieEngine_DLL void SetSpecular(const Vector4& specular) { _lightInfo.specular = specular; }
	NewbieEngine_DLL void SetATT(const Vector3& att) { _lightInfo.att = att; }
	NewbieEngine_DLL void SetRange(float range) { _lightInfo.range = range; }
	NewbieEngine_DLL void SetSpot(float spot) { _lightInfo.spot = spot; }

	NewbieEngine_DLL void SendLightInfo();

	void Update() override;
};

