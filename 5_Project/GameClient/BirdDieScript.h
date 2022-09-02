#pragma once
#include "MonoBehaviour.h"

class Transform;
class Animator;
class Panel;

class BirdDieScript : public MonoBehaviour
{
public:
	BirdDieScript(shared_ptr<GameObject> gameObject);
	virtual ~BirdDieScript();

private:
	hash<string> hasher;

	shared_ptr<Transform> _transform;

	shared_ptr<GameObject> _gameObject;

	size_t _npc_die;

public:
	virtual void Awake() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;

	void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
};

