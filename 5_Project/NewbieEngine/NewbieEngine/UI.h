#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"

class Transform;

class UI : public Component
{
public:
	NewbieEngine_DLL UI(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~UI();

protected:
	int _sortLayer = 0;

public:
	void SetSortLayer(int layer) { _sortLayer = layer; }

	virtual void Resize(int width, int height) {};
};

