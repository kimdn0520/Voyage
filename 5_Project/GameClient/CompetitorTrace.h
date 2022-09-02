#pragma once
#include "MonoBehaviour.h"

class Transform;
class GameObject;

class CompetitorTrace : public MonoBehaviour
{
public:
	CompetitorTrace(shared_ptr<GameObject> gameObject);
	virtual ~CompetitorTrace();

private:
	shared_ptr<Transform> _transform;

	shared_ptr<GameObject> _player;

	float _destroyTime;

	float _speed;

	float _curTime;

public:
	void SetPlayer(shared_ptr<GameObject> gameObj) { _player = gameObj; } 

	void FixedUpdate() override;
};

