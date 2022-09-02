#pragma once
#include "Scene.h"

class AdulthoodScene : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;
};

