#pragma once
#include "MonoBehaviour.h"

class Text;
class Transform;

class TextScript : public MonoBehaviour
{
public:
	TextScript(shared_ptr<GameObject> gameObject);
	virtual ~TextScript();

private:
	shared_ptr<Text> _text;

	Vector4 _color;

	shared_ptr<Transform> _boatTransform;

	bool _isDebug = false;

public:


public:
	void SetTransform(shared_ptr<Transform> boat);

public:

	virtual void Awake() override;
	virtual void Update() override;
};

