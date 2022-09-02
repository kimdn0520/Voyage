#include "pch.h"
#include "Interaction.h"

Interaction::Interaction(shared_ptr<GameObject> gameObject):
	Component(gameObject, COMPONENT_TYPE:: Interaction)
{
}

Interaction::~Interaction()
{
}

void Interaction::Start()
{
}

void Interaction::Update()
{

}

void Interaction::Render()
{
}
