#pragma once
#include "Scene.h"

class BadEndingScene : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};

