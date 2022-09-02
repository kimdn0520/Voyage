#include "pch.h"
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsEngineManager.h"
#include "GraphicsEngineAPI.h"

Light::Light(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::LIGHT),
	_transform(gameObject->GetTransform())
{}

Light::~Light()
{}

void Light::SetLightDirection(const Vector3& direction)
{	
	_lightInfo.direction = direction;
	_direction = direction;
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int>(type);
	
	_type = type;
}

/// <summary>
/// �������� �� �Է��ϰ� LightInfo�� �׷��Ƚ��ʿ� ������ ����Ѵ�.
/// </summary>
void Light::SendLightInfo()
{	
	_lightInfo.hashcode = GetGameObject()->GetName();
	
	GraphicsEngineManager::GetInstance()->SetLightInfo(_lightInfo);
}
 
void Light::Update()
{
	switch (_type)
	{
		case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		{
			_lightInfo.direction = _direction;
		}
		break;
		case LIGHT_TYPE::POINT_LIGHT:
		{
			_lightInfo.position = _transform->GetWorldPosition();
		}
		break;
		case LIGHT_TYPE::SPOT_LIGHT:
		{
			SetLightDirection(_transform->GetLook());
			_lightInfo.position = _transform->GetWorldPosition();
		}
		break;
	}

	GraphicsEngineManager::GetInstance()->UpdateLightInfo(_lightInfo);
}
