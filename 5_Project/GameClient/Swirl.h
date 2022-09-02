#pragma once
#include "MonoBehaviour.h"

class Transform;

class Swirl : public MonoBehaviour
{
public:
	Swirl(shared_ptr<GameObject> gameObject);
	virtual ~Swirl();

private:
	shared_ptr<Transform> _transform;

	float _rotateSpeed;

	float _speed;

	float _curTime;

	float _destroyTime;

public:

	virtual void FixedUpdate() override;

	virtual void Update() override;
};

