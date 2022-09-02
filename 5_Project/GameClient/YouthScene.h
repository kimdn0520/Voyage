#pragma once
#include "Scene.h"

class YouthScene : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;

};

