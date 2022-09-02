#pragma once

#include "Scene.h"

class TestScene3 : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};


