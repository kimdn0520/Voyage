#pragma once
#include "MonoBehaviour.h"

class LoadSceneScript : public MonoBehaviour
{
public:
	LoadSceneScript(shared_ptr<GameObject> gameObject);
	virtual ~LoadSceneScript();

private:
	string _nextScene;

public:
	virtual void Update() override;
};

