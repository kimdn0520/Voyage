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
	// ������ �ٲ���
	for (auto& collider : colliders)
	{
		collider->_preColList = collider->_curColList;
		
		collider->ClearCurList();
	}
}

void PhysicsManager::CheckCollision(std::vector<std::shared_ptr<ColliderBase>>& colliders)
{
	// �񱳸� ������ ����
	// 1. size�� �Ź� ����ϴ°� ���� �ʴ�
	// 2. �ݴ�� Ž���ؾ� ������(���� ��..) ���� �񱳺��� ����� �����ϴ� ���� �ð������� ������ (���̰� ũ�� �ʴ�)
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

		// �� �ݶ��̴��� �������� �浹�� �ݶ��̴��� 
		std::vector<std::shared_ptr<ColliderBase>>& preColList = collider->_preColList;

		// �� �ݶ��̴��� ���� �浹�� �ݶ��̴���
		std::vector<std::shared_ptr<ColliderBase>>& curColList = collider->_curColList;

		std::shared_ptr<GameObject> gameObject = collider->GetGameObject();

		for (auto& curCollider : curColList)
		{
			// ���翡 �ִµ� ���ſ� ������ Enter
			// ���翡 �ִµ� ���ſ� ������ Stay
			std::find(preColList.begin(), preColList.end(), curCollider) != preColList.end()
				? gameObject->OnTriggerStay(curCollider)
				: gameObject->OnTriggerEnter(curCollider);

			// ���翡 ������ collider�� �浹 ����
			if (collider != nullptr)
				collider->SetIsCol(true);
		}

		// ���翡 ���µ� => ���ſ� ������ Exit, ������ �Ѿ.
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

	// �浹�� �Ǿ����� Ȯ���ϱ� ���Ͽ� wire�� collider ���¸� �׷��ֱ����� coldebug ������ �׷��Ƚ��� �����ش�. 
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
