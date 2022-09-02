#pragma once
#include "MonoBehaviour.h"

class BadEndingSceneScript : public MonoBehaviour
{
public:
	BadEndingSceneScript(shared_ptr<GameObject> gameObject);
	virtual ~BadEndingSceneScript();

private:
	float _curTime = 0.f;

public:

	virtual void Update() override;
};

