#pragma once
#include "Component.h"

class Renderer : public Component
{
public:
	Renderer(shared_ptr<GameObject> gameObject);
	virtual ~Renderer();

};

