#pragma once
#include "Component.h"

class Interaction : public Component
{
public:
	Interaction(shared_ptr<GameObject> gameObject);
	virtual ~Interaction();

	virtual void Start()  override;
	virtual void Update()  override;
	virtual void Render() override;

};

