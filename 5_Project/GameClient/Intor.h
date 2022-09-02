#pragma once
#include "Scene.h"
class Intor:public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};

