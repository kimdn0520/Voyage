#include "pch.h"
#include "PhysicsManager.h"
#include "ColliderBase.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsEngineManager.h"

shared_ptr<PhysicsManager> PhysicsManager::physicsManager = nullptr;

shared_ptr<PhysicsManager> PhysicsManager::GetInstance()
{
	if (physicsManager == nullptr)
		physicsManager = make_shared<PhysicsManager>();

	return physicsManager;
}

void PhysicsManager::ColStateUpdate(std::vector<std::shared_ptr<ColliderBase>>& colliders)
{
	// 느리면 바꾸자
	for (auto& collider : colliders)
	{
		collider->_preColList = collider->_curColList;
		
		collider->ClearCurList();
	}
}

void PhysicsManager::CheckCollision(std::vector<std::shared_ptr<ColliderBase>>& colliders)
{
	// 비교를 빠르게 하자
	// 1. size를 매번 계산하는건 좋지 않다
	// 2. 반대로 탐색해야 빠르다(조금 더..) 변수 비교보단 상수에 접근하는 것이 시간적으로 빠르다 (차이가 크진 않다)
	int colliderSize = colliders.size();

	for (int i = colliderSize - 1; i > 0; i--)        
	{
		for (int j = i - 1; j >= 0; j--)
		{
			if (colliders[i]->Intersetcs(colliders[j]))
			{
				colliders[i]->_curColList.push_back(colliders[j]);
				colliders[j]->_curColList.push_back(colliders[i]);
			}
		}
	}
}

void PhysicsManager::CollisionEvent(std::vector<std::shared_ptr<ColliderBase>>& colliders)
{
	for (auto& collider : colliders)
	{
		if(collider == nullptr)
			continue;

		// 이 콜라이더와 지난번에 충돌한 콜라이더들 
		std::vector<std::shared_ptr<ColliderBase>>& preColList = collider->_preColList;

		// 이 콜라이더와 지금 충돌한 콜라이더들
		std::vector<std::shared_ptr<ColliderBase>>& curColList = collider->_curColList;

		std::shared_ptr<GameObject> gameObject = collider->GetGameObject();

		for (auto& curCollider : curColList)
		{
			// 현재에 있는데 과거에 없으면 Enter
			// 현재에 있는데 과거에 있으면 Stay
			std::find(preColList.begin(), preColList.end(), curCollider) != preColList.end()
				? gameObject->OnTriggerStay(curCollider)
				: gameObject->OnTriggerEnter(curCollider);

			// 현재에 있으면 collider는 충돌 상태
			if (collider != nullptr)
				collider->SetIsCol(true);
		}

		// 현재에 없는데 => 과거에 있으면 Exit, 없으면 넘어가.
		for (auto& preCollider : preColList)
		{
			if (std::find(curColList.begin(), curColList.end(), preCollider) == curColList.end())
			{
				gameObject->OnTriggerExit(preCollider);
				
				if (collider != nullptr)
					collider->SetIsCol(false);
			}
		}
	}
}

void PhysicsManager::Update(std::vector<std::shared_ptr<ColliderBase>>& colliders)
{
	ColStateUpdate(colliders);

	CheckCollision(colliders);

	CollisionEvent(colliders);

	// 충돌이 되었는지 확인하기 위하여 wire로 collider 상태를 그려주기위해 coldebug 정보를 그래픽스에 보내준다. 
	for (auto& collider : colliders)
	{
		std::shared_ptr<BoxCollider> boxCollider = std::dynamic_pointer_cast<BoxCollider>(collider);
		std::shared_ptr<SphereCollider> sphereCollider = std::dynamic_pointer_cast<SphereCollider>(collider);
		shared_ptr<ColDebugInfo> colDebugInfo = make_shared<ColDebugInfo>();

		if (boxCollider != nullptr)
		{
			colDebugInfo->type = ColliderType::Box;
			colDebugInfo->isCol = collider->_isCol;
			
			Vector3 side = boxCollider->GetSide();
			Vector3 position = boxCollider->GetCenterPos();

			Matrix scaleMatrix = XMMatrixScaling(side.x, side.y, side.z);

			Matrix rotationMatrix = XMMatrixRotationQuaternion(boxCollider->_transform->EulerToQuat(boxCollider->_transform->GetWorldRotation()));

			Matrix positionMatrix = XMMatrixTranslation(position.x, position.y, position.z);
			
			colDebugInfo->worldTM = scaleMatrix * rotationMatrix * positionMatrix;
		}

		if (sphereCollider != nullptr)
		{
			colDebugInfo->type = ColliderType::Sphere;
			colDebugInfo->isCol = collider->_isCol;

			Vector3 scale = Vector3(sphereCollider->GetRadius() * 2, sphereCollider->GetRadius() * 2, sphereCollider->GetRadius() * 2);
			Vector3 position = sphereCollider->GetCenterPos();

			Matrix scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

			Matrix rotationMatrix = XMMatrixRotationQuaternion(sphereCollider->_transform->EulerToQuat(sphereCollider->_transform->GetWorldRotation()));

			Matrix positionMatrix = XMMatrixTranslation(position.x, position.y, position.z);

			colDebugInfo->worldTM = scaleMatrix * rotationMatrix * positionMatrix;
		}

		GraphicsEngineManager::GetInstance()->SetColDebugInfo(colDebugInfo);
	}
}
