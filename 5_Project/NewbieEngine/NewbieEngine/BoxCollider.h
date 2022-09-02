#pragma once
#include "NewbieEngineDLL.h"
#include "ColliderBase.h"

class Transform;
class BoxCollider;
class SphereCollider;
	
class BoxCollider : public ColliderBase
{
public:
	NewbieEngine_DLL BoxCollider(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~BoxCollider();

private:
	Vector3 _oriCenterPos;

	Vector3 _centerPos;

	Vector3 _side;

	vector<unsigned int> _idx;

public:
	friend class NoobRay;

public:
	const Vector3& GetCenterPos() { return _centerPos; }

	const Vector3& GetSide() { return _side; }

	NewbieEngine_DLL void SetColliderInfo(Vector3 centerPos, Vector3 side);

	bool CheckBoxCollider(std::shared_ptr<BoxCollider> other);

	bool CheckSphereCollider(std::shared_ptr<SphereCollider> other);

	bool Intersetcs(shared_ptr<ColliderBase> otherCollider) override;

	void FixedUpdate() override;
};

