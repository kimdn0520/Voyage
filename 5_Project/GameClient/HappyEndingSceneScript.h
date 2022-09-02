#pragma once
#include "MonoBehaviour.h"

class HappyEndingSceneScript : public MonoBehaviour
{
public:
	HappyEndingSceneScript(shared_ptr<GameObject> gameObject);
	virtual ~HappyEndingSceneScript();

private:
	float _curTime = 0.f;

public:

	virtual void Update() override;
};

