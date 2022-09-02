#pragma once
#include "MonoBehaviour.h"

class Panel;

class PressAnyKey : public MonoBehaviour
{
public:
	PressAnyKey(shared_ptr<GameObject> gameObject);
	virtual ~PressAnyKey();

private:
	float _speed;

	shared_ptr<Panel> _panel;

	float _alpha;

	float _blinkTime;

	bool _isFade;

public:
	virtual void Update() override;
};

