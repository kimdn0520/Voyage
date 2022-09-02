#pragma once
#include "Scene.h"

class HappyEndingScene : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};

