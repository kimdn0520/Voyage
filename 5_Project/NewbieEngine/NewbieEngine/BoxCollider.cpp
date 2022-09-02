#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"

BoxCollider::BoxCollider(shared_ptr<GameObject> gameObject)
	: ColliderBase(gameObject),
	_centerPos(Vector3(0.f, 0.f, 0.f)), _side(Vector3(0.f, 0.f, 0.f))
{
	// picking ÇÒ¶§ ¾²ÀÏ indexBuffer
	_idx.resize(36);
	// ¾Õ¸é
	_idx[0] = 0; _idx[1] = 1; _idx[2] = 2;
	_idx[3] = 0; _idx[4] = 2; _idx[5] = 3;
	// µÞ¸é
	_idx[6] = 4; _idx[7] = 5; _idx[8] = 6;
	_idx[9] = 4; _idx[10] = 6; _idx[11] = 7;
	// À­¸é
	_idx[12] = 1; _idx[13] = 7; _idx[14] = 6;
	_idx[15] = 1; _idx[16] = 6; _idx[17] = 2;
	// ¾Æ·§¸é
	_idx[18] = 0; _idx[19] = 3; _idx[20] = 5;
	_idx[21] = 0; _idx[22] = 5; _idx[23] = 4;
	// ¿ÞÂÊ¸é
	_idx[24] = 4; _idx[25] = 7; _idx[26] = 1;
	_idx[27] = 4; _idx[28] = 1; _idx[29] = 0;
	// ¿À¸¥ÂÊ¸é
	_idx[30] = 3; _idx[31] = 2; _idx[32] = 6;
	_idx[33] = 3; _idx[34] = 6; _idx[35] = 5;
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::SetColliderInfo(Vector3 centerPos, Vector3 side)
{
	_oriCenterPos = centerPos;
	_side = side;
}

bool BoxCollider::CheckBoxCollider(std::shared_ptr<BoxCollider> other)
{
	shared_ptr<BoxCollider> otherBoxCollider = dynamic_pointer_cast<BoxCollider>(other);

	Vector3 otherCenter = otherBoxCollider->GetCenterPos();
	Vector3 otherSide = otherBoxCollider->GetSide();

	Vector3 minA = _centerPos - (_side * 0.5f);
	Vector3 maxA = _centerPos + (_side * 0.5f);

	Vector3 minB = otherCenter - (otherSide * 0.5f);
	Vector3 maxB = otherCenter + (otherSide * 0.5f);

	// for each i in (x, y, z) if a_min(i) > b_max(i) or b_min(i) > a_max(i) then return false
	Vector3 disjoint = XMVectorOrInt(XMVectorGreater(minA, maxB), XMVectorGreater(minB, maxA));

	return !DirectX::Internal::XMVector3AnyTrue(disjoint);
}

bool BoxCollider::CheckSphereCollider(std::shared_ptr<SphereCollider> other)
{
	Vector3 sphereCenter = other->GetCenterPos();
	Vector3 sphereRadius = XMVectorReplicatePtr(&other->GetRadius());

	Vector3 boxMin = _centerPos - (_side * 0.5f);
	Vector3 boxMax = _centerPos + (_side * 0.5f);

	// Find the distance to the nearest point on the box.
	// for each i in (x, y, z)
	// if (SphereCenter(i) < BoxMin(i)) d2 += (SphereCenter(i) - BoxMin(i)) ^ 2
	// else if (SphereCenter(i) > BoxMax(i)) d2 += (SphereCenter(i) - BoxMax(i)) ^ 2

	XMVECTOR d = XMVectorZero();

	// Compute d for each dimension.
	XMVECTOR LessThanMin = XMVectorLess(sphereCenter, boxMin);
	XMVECTOR GreaterThanMax = XMVectorGreater(sphereCenter, boxMax);

	XMVECTOR MinDelta = XMVectorSubtract(sphereCenter, boxMin);
	XMVECTOR MaxDelta = XMVectorSubtract(sphereCenter, boxMax);

	// Choose value for each dimension based on the comparison.
	d = XMVectorSelect(d, MinDelta, LessThanMin);
	d = XMVectorSelect(d, MaxDelta, GreaterThanMax);

	// Use a dot-product to square them and sum them together.
	XMVECTOR d2 = XMVector3Dot(d, d);

	return XMVector3LessOrEqual(d2, XMVectorMultiply(sphereRadius, sphereRadius));
}

bool BoxCollider::Intersetcs(shared_ptr<ColliderBase> otherCollider)
{
	std::shared_ptr<SphereCollider> sphereCol = std::dynamic_pointer_cast<SphereCollider>(otherCollider);

	std::shared_ptr<BoxCollider> boxCol = std::dynamic_pointer_cast<BoxCollider>(otherCollider);

	if (sphereCol != nullptr)
		return CheckSphereCollider(sphereCol);

	if (boxCol != nullptr)
		return CheckBoxCollider(boxCol);

	return false;
}

void BoxCollider::FixedUpdate()
{
	_centerPos = _oriCenterPos + _transform->GetWorldPosition();
}
