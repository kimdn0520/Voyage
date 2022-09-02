#pragma once
#include "Scene.h"

class Credit : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};

