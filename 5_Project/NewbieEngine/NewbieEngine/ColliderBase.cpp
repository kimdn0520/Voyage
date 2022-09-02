#include "pch.h"
#include "ColliderBase.h"
#include "GameObject.h"
#include "Transform.h"

ColliderBase::ColliderBase(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::COLLIDER),
	_colliderType(ColliderType::Box),		// default´Â box
	_transform(gameObject->GetTransform())
{}

ColliderBase::~ColliderBase()
{}
