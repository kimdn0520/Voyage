#pragma once
#include "MonoBehaviour.h"

class Transform;

class CompetitorController : public MonoBehaviour
{
public:
	CompetitorController(shared_ptr<GameObject> gameObject);
	virtual ~CompetitorController();

private:
	shared_ptr<Transform> _transform;

	float _destroyTime;

	float _speed;

	float _curTime;

public:

	void FixedUpdate() override;
};

