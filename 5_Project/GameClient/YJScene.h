#pragma once
#include "Scene.h"

class YJScene :public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};

