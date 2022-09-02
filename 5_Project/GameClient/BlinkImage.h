#pragma once
#include "MonoBehaviour.h"

class Panel;

class BlinkImage : public MonoBehaviour
{
public:
	BlinkImage(shared_ptr<GameObject> gameObject);
	virtual ~BlinkImage();

private:
	shared_ptr<Panel> _panel;

	float _alpha;

	float _blinkTime;

	bool _isFade = false;

public:
	virtual void Update() override;
};

