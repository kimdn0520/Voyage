#pragma once
#include "Scene.h"


class Infancy : public Scene
{
private:
	hash<string> _hasher;

	float _resizeWidth = 0.f;
	float _resizeHeight = 0.f;

public:
	virtual void Init() override;
};

