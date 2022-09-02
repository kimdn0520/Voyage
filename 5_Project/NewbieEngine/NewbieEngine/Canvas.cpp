#include "pch.h"
#include "Canvas.h"
#include "GameObject.h"
#include "Transform.h"

Canvas::Canvas(shared_ptr<GameObject> gameObject)
	: UI(gameObject)
{}

Canvas::~Canvas()
{}

void Canvas::SetCamera(shared_ptr<GameObject> camera)
{
	_camera = camera;
}

void Canvas::Update()
{
	// Canvas의 포지션은 Camera를 따라간다.
	_transform->SetLocalPosition(_camera->GetTransform()->GetWorldPosition());
}
