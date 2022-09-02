#pragma once
#include "NewbieEngineDLL.h"
#include "ColliderBase.h"

class Transform;
class BoxCollider;
class SphereCollider;

class SphereCollider : public ColliderBase
{
public:
	NewbieEngine_DLL SphereCollider(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~SphereCollider();

private:
	Vector3 _oriCenterPos;

	Vector3 _centerPos;

	float _radius;

public:
	const Vector3& GetCenterPos() { return _centerPos; }

	const float& GetRadius() { return _radius; }

	NewbieEngine_DLL void SetColliderInfo(Vector3 centerPos, float radius);

	bool CheckBoxCollider(std::shared_ptr<BoxCollider> other);

	bool CheckSphereCollider(std::shared_ptr<SphereCollider> other);

	bool Intersetcs(shared_ptr<ColliderBase> otherCollider) override;

	void FixedUpdate() override;
};

