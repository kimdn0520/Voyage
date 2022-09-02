#include "pch.h"
#include "Physics.h"
#include "NoobRay.h"
#include "RaycastHit.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "GraphicsEngineManager.h"

Physics::Physics()
{}

Physics::~Physics()
{}

bool Physics::RayCast(NoobRay ray, RaycastHit raycastHit, float distance)
{
	// ������� ����ī�޶� �����´�.
	shared_ptr<Camera> camera = SceneManager::GetInstance()->GetActiveScene()->GetMainCamera();

	// ������� �ݶ��̴� ������Ʈ�� �����´�.
	auto& gameObjects = SceneManager::GetInstance()->GetActiveScene()->GetColliders();

	// �������� width, height�� �����´�.
	float width = static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().width);
	float height = static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().height);

	// ViewSpace������ Ray ����

	return false;
}
