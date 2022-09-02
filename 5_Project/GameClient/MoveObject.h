#pragma once
#include "MonoBehaviour.h"

class Transform;

class MoveObject : public MonoBehaviour
{
public:
	MoveObject(shared_ptr<GameObject> gameObject);
	virtual ~MoveObject();

private:
	float _moveSpeed;

	shared_ptr<Transform> _trnasform;
	
public:

	virtual void Awake() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;

	void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
};

