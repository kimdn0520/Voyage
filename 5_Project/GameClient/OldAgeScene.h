#pragma once
#include "Scene.h"

class OldAgeScene : public Scene
{
private:
	hash<string> _hasher;

public:
	virtual void Init() override;

};

