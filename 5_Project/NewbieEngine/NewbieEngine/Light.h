#pragma once
#include "AlignedAllocationPolicy.h"
#include "NewbieEngineDLL.h"
#include "Component.h"
#include "ObjectInfo.h"

/// <summary>
/// Ambient : ������ �����ϴ� �ֺ����� ��
/// Diffuse : ������ �����ϴ� �л걤�� ��(Ȯ�걤)
/// Specular : ������ �����ϴ� �ݿ����� ��(�ݻ籤)
/// Direction : ���� ����
/// Position : ������ ��ġ
/// Range : ���� ����, �������� �Ÿ��� �� �������� ū ���� ���� ���� ����
/// Attenuation : ���� ���Ⱑ �Ÿ��� ���� �����ϴ� ����� �����ϴ� �� ����
///				  ���� �������(a0, a1, a2)�� ���·� �����Ѵ�.
/// Spot : ������ ��꿡�� ������ ������ �����ϴµ� ���̴� ����
/// Pad : �ʿ��ϴٸ� ������ �迭�� ������ �� �ֵ��� �������� float �ϳ��� �ڸ��� �ϳ� ������ �д�.
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

