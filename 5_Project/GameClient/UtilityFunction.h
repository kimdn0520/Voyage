#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#define RAD_TO_DEGREE(rad) { rad * (180.0f / 3.1415926535f) }


namespace UtilityFunction
{
	shared_ptr<GameObject> origin;
	shared_ptr<GameObject> target;

	/// <summary>
	/// Ÿ���� Ž�� ���� �ȿ� �ִ��� �˻�
	/// "origin" ���� ��ġ
	/// "target" ��� ��ġ
	/// "detectionRange" �Ÿ�
	static bool IsTargetInDetectionRange(SimpleMath::Vector3 origin, const SimpleMath::Vector3 target, const float detectionRange)
	{
		// �Ÿ��� ������
		float distance = SimpleMath::Vector3::Distance(origin, target);

		if (distance < detectionRange)
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	/// <summary>
	/// Ÿ���� Ž�� ���� �ȿ� �ִ��� �˻�
	/// "origin" ���� ������Ʈ
	/// "target" ��� ������Ʈ
	/// "detectionRange" �Ÿ�
	static bool IsTargetInDetectionRange(shared_ptr<GameObject> origin, shared_ptr<GameObject> target, const float detectionRange)
	{
		// �Ÿ��� ������
		float distance = Vector3::Distance(origin->GetTransform()->GetWorldPosition(), target->GetTransform()->GetWorldPosition());
		return IsTargetInDetectionRange(origin->GetTransform()->GetWorldPosition(), target->GetTransform()->GetWorldPosition(), detectionRange);
	}

	/// <summary>
	/// target ������Ʈ���� ������ ���ؼ� �����Ѵ�
	/// </summary>
	static float CalcAngleToTarget(shared_ptr<GameObject> origi, shared_ptr<GameObject> target)
	{
		Vector3 myPos = origin->GetTransform()->GetWorldPosition();
		Vector3 targetPos = target->GetTransform()->GetWorldPosition();

		float dx = targetPos.x - myPos.x;
		float dz = targetPos.z - myPos.z;

		float rad = std::atan2f(dx, dz);
		float angle = RAD_TO_DEGREE(rad);

		return angle;
	}

	/// <summary>
	/// ���� 0 ~ 360 �� ���̷� �����ϱ� ���� �Լ�
	/// "angle" ����ȭ�� ��
	static bool NormalizeAngle(float& angle)
	{
		if (angle > 360)
		{
			angle -= 360;
		}
		else if (angle < 0)
		{
			angle += 360;
		}

		return angle;
	}

}
