#pragma once
#include "MonoBehaviour.h"

class Text;

class LoadingText : public MonoBehaviour
{
public:
	LoadingText(shared_ptr<GameObject> gameObject);
	virtual ~LoadingText();

private:
	shared_ptr<Text> _text;

	Vector4 _color;

public:

	virtual void Awake() override;
	virtual void Update() override;
};

