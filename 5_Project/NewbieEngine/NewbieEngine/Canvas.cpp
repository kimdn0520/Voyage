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
	// Canvas�� �������� Camera�� ���󰣴�.
	_transform->SetLocalPosition(_camera->GetTransform()->GetWorldPosition());
}
