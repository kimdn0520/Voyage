#include "pch.h"
#include "NoobRay.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderBase.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

NoobRay::NoobRay()
{}

NoobRay::~NoobRay()
{}

/// <summary>
/// �츮�� �����ٰ� �̵��� �Ұ� �ƴϱ⶧����! �׳� �ϴ� �ڽ��ݶ��̴����� üũ�� ���ش�.
/// </summary>
bool NoobRay::Intersect()
{
	// ������� �ݶ��̴� ������Ʈ�� �����´�.
	auto& colliders = SceneManager::GetInstance()->GetActiveScene()->GetColliders();
	float saveNear = 1000000.f;
	bool isCol = false;

	for (auto& collider : colliders)
	{
		if (collider->_colliderType == ColliderType::Box)
		{
			// �ڽ��� ĳ��Ʈ ���ְ�
			std::shared_ptr<BoxCollider> boxCollider = std::dynamic_pointer_cast<BoxCollider>(collider);
			
			//// ��ü�� worldMatrix�� inverse�� �����ش�.
			//Matrix worldMatrixInv = XMMatrixInverse(nullptr, boxCollider->GetGameObject()->GetTransform()->GetWorldMatrix());

			//// Ray�� Local �������� �����ش�.
			//rayOrigin = XMVector3TransformCoord(rayOrigin, worldMatrixInv);		
			//direction = XMVector3TransformNormal(direction, worldMatrixInv);	
			//XMVector3Normalize(direction);	// ������ ����ȭ �����ش�.

			// centerPos�� side ��������
			Vector3 centerPos = boxCollider->GetCenterPos();
			Vector3 side = boxCollider->GetSide();

			vector<Vector3> vertices(8);
			vertices[0] = Vector3(centerPos.x - side.x * 0.5f, centerPos.y - side.y * 0.5f, centerPos.z - side.z * 0.5f);
			vertices[1] = Vector3(centerPos.x - side.x * 0.5f, centerPos.y + side.y * 0.5f, centerPos.z - side.z * 0.5f);
			vertices[2] = Vector3(centerPos.x + side.x * 0.5f, centerPos.y + side.y * 0.5f, centerPos.z - side.z * 0.5f);
			vertices[3] = Vector3(centerPos.x + side.x * 0.5f, centerPos.y - side.y * 0.5f, centerPos.z - side.z * 0.5f);
			vertices[4] = Vector3(centerPos.x - side.x * 0.5f, centerPos.y - side.y * 0.5f, centerPos.z + side.z * 0.5f);
			vertices[5] = Vector3(centerPos.x + side.x * 0.5f, centerPos.y - side.y * 0.5f, centerPos.z + side.z * 0.5f);
			vertices[6] = Vector3(centerPos.x + side.x * 0.5f, centerPos.y + side.y * 0.5f, centerPos.z + side.z * 0.5f);
			vertices[7] = Vector3(centerPos.x - side.x * 0.5f, centerPos.y + side.y * 0.5f, centerPos.z + side.z * 0.5f);

			vector<unsigned int> idx = boxCollider->_idx;
			
			Vector3 directionVector = Vector3(direction.x, direction.y, direction.z);
			Vector3 originVector = Vector3(rayOrigin.x, rayOrigin.y, rayOrigin.z);
			Vector3 worldDir = Vector3(direction.x, direction.y, direction.z);

			float dist = 2100000.f;

			for (int i = 0; i < idx.size(); i += 3)
			{
				Vector3 v0 = vertices[idx[i]];
				Vector3 v1 = vertices[idx[i + 1]];
				Vector3 v2 = vertices[idx[i + 2]];

				Vector3 e1 = v1 - v0;
				Vector3 e2 = v2 - v0;

				Vector3 pvec = directionVector.Cross(e2);
				float det = e1.Dot(pvec);
				Vector3 tvec;
				if(det > 0)
					tvec = originVector - v0;
				else
				{
					tvec = v0 - originVector;
					det = -det;
				}

				if(det < 0.0001f)
					continue;

				float u = tvec.Dot(pvec);
				if(u < 0.0f || u > det)
					continue;

				Vector3 qvec;
				qvec = tvec.Cross(e1);
				float v = directionVector.Dot(qvec);
				if(v < 0.0f || u + v > det)
					continue;

				float t = e2.Dot(qvec);
				float flnvDet = 1.0f / det;
				t *= flnvDet;
				u *= flnvDet;
				v *= flnvDet;

				Vector3 world = Vector3(directionVector.x * t, directionVector.y * t, directionVector.z * t);
				float currentDist = world.Length();

				if (currentDist <= dist)
				{
					point = rayOrigin + world;
					dist = currentDist;
					isCol = true;
				}

				//Vector3 mCrossE1 = m.Cross(e1);
				//Vector3 uCrossE2 = directionVector.Cross(e2);

				//float t = e2.Dot(mCrossE1) / e1.Dot(uCrossE2);
				//float u = m.Dot(uCrossE2) / e1.Dot(uCrossE2);
				//float v = directionVector.Dot(mCrossE1) / e1.Dot(uCrossE2);

				//if (((u >= 0.f) && (u <= 1.f)) && ((v >= 0.f) && (v <= 1.f)) && (t >= 0.f))
				//{
				//	// ���� �󿡼��� �Ÿ��� �纻��.
				//	Vector3 world = Vector3(directionVector.x * t, directionVector.y * t, directionVector.z * t);
				//	float currentDist = world.Length();

				//	if (currentDist <= dist)
				//	{
				//		point = rayOrigin + world;
				//		dist = currentDist;
				//		isCol = true;
				//	}
				//}
			}
		}	
	}

	// �ѹ��̶� �ݶ��̴��� ray�� �浹�ߴٸ�
	if (isCol)
	{
		return true;
	}

	return false;
}
