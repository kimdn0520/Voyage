#include "pch.h"
#include "Rigidbody.h"
#include "Transform.h"

Rigidbody::Rigidbody(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::RIGIDBODY),
	_mass(1.f),
	_drag(0.f),
	_angularDrag(0.05f),
	_isGravity(true),
	_isKinematic(false)
{}

Rigidbody::~Rigidbody()
{}

void Rigidbody::FixedUpdate()
{
	if(_bodyType == BodyType::Static)
		return;

}
