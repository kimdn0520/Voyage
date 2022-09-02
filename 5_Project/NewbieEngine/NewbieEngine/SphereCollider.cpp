#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Transform.h"

SphereCollider::SphereCollider(shared_ptr<GameObject> gameObject)
	: ColliderBase(gameObject),
	_centerPos(Vector3(0.f, 0.f, 0.f)), _radius(1.f)
{
	_colliderType = ColliderType::Sphere;
}

SphereCollider::~SphereCollider()
{}

void SphereCollider::SetColliderInfo(Vector3 centerPos, float radius)
{
	_oriCenterPos = centerPos;
	_radius = radius;
}

bool SphereCollider::CheckBoxCollider(std::shared_ptr<BoxCollider> other)
{
	Vector3 sphereCenter = _centerPos;
	Vector3 sphereRadius = XMVectorReplicatePtr(&_radius);

	Vector3 boxMin = other->GetCenterPos() - (other->GetSide() * 0.5f);
	Vector3 boxMax = other->GetCenterPos() + (other->GetSide() * 0.5f);

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

bool SphereCollider::CheckSphereCollider(std::shared_ptr<SphereCollider> other)
{
	float centerDist = (other->_centerPos - _centerPos).Length();
	
	return centerDist <= _radius + other->_radius; 
}

bool SphereCollider::Intersetcs(shared_ptr<ColliderBase> otherCollider)
{
	std::shared_ptr<SphereCollider> sphereCol = std::dynamic_pointer_cast<SphereCollider>(otherCollider);

	std::shared_ptr<BoxCollider> boxCol = std::dynamic_pointer_cast<BoxCollider>(otherCollider);

	if (sphereCol != nullptr)
		return CheckSphereCollider(sphereCol);

	if (boxCol != nullptr)
		return CheckBoxCollider(boxCol);

	return false;
}

void SphereCollider::FixedUpdate()
{
	_centerPos = _oriCenterPos + _transform->GetWorldPosition();
}
