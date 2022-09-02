#include "pch.h"
#include "UI.h"

UI::UI(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::UI)
{}

UI::~UI()
{}
