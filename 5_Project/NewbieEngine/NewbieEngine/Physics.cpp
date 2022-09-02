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
	// 현재씬의 메인카메라를 가져온다.
	shared_ptr<Camera> camera = SceneManager::GetInstance()->GetActiveScene()->GetMainCamera();

	// 현재씬의 콜라이더 오브젝트를 가져온다.
	auto& gameObjects = SceneManager::GetInstance()->GetActiveScene()->GetColliders();

	// 윈도우의 width, height를 가져온다.
	float width = static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().width);
	float height = static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().height);

	// ViewSpace에서의 Ray 정의

	return false;
}
