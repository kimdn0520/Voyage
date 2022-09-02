#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::RENDERER)
{
}

Renderer::~Renderer()
{}
