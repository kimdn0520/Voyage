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
	/// 타겟이 탐지 범위 안에 있느지 검사
	/// "origin" 기준 위치
	/// "target" 대상 위치
	/// "detectionRange" 거리
	static bool IsTargetInDetectionRange(SimpleMath::Vector3 origin, const SimpleMath::Vector3 target, const float detectionRange)
	{
		// 거리를 구하자
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
	/// 타겟이 탐지 범위 안에 있느지 검사
	/// "origin" 기준 오브젝트
	/// "target" 대상 오브젝트
	/// "detectionRange" 거리
	static bool IsTargetInDetectionRange(shared_ptr<GameObject> origin, shared_ptr<GameObject> target, const float detectionRange)
	{
		// 거리를 구하자
		float distance = Vector3::Distance(origin->GetTransform()->GetWorldPosition(), target->GetTransform()->GetWorldPosition());
		return IsTargetInDetectionRange(origin->GetTransform()->GetWorldPosition(), target->GetTransform()->GetWorldPosition(), detectionRange);
	}

	/// <summary>
	/// target 오브젝트로의 각도를 구해서 리턴한다
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
	/// 각을 0 ~ 360 도 사이로 제한하기 위한 함수
	/// "angle" 정규화할 각
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
