#pragma once
#include "SimpleMath.h"

struct BoundingCube
{
	BoundingCube(const DirectX::SimpleMath::Vector3& localSide, const DirectX::SimpleMath::Vector3& localCenter) :
		_localSide(localSide), _localCenter(localCenter)
	{
		float halfX = _localSide.x / 2;
		float halfY = _localSide.y / 2;
		float halfZ = _localSide.z / 2;

		_localPoint[0] = _localCenter + DirectX::SimpleMath::Vector3(-halfX, -halfY, -halfZ);
		_localPoint[1] = _localCenter + DirectX::SimpleMath::Vector3(-halfX, halfY, -halfZ);
		_localPoint[2] = _localCenter + DirectX::SimpleMath::Vector3(halfX, -halfY, -halfZ);
		_localPoint[3] = _localCenter + DirectX::SimpleMath::Vector3(-halfX, -halfY, halfZ);
		_localPoint[4] = _localCenter + DirectX::SimpleMath::Vector3(halfX, -halfY, halfZ);
		_localPoint[5] = _localCenter + DirectX::SimpleMath::Vector3(halfX, halfY, -halfZ);
		_localPoint[6] = _localCenter + DirectX::SimpleMath::Vector3(halfX, halfY, halfZ);
		_localPoint[7] = _localCenter + DirectX::SimpleMath::Vector3(-halfX, halfY, halfZ);
	}

	DirectX::SimpleMath::Vector3 _localPoint[8];	// 꼭짓점

	DirectX::SimpleMath::Vector3 _localCenter;		// 중심

	DirectX::SimpleMath::Vector3 _localSide;		// 변
};