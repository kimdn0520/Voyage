#pragma once
#include "MonoBehaviour.h"

class Camera;
class Transform;
class Audio;

class CameraScript : public MonoBehaviour
{
public:
	CameraScript(shared_ptr<GameObject> gameObject);
	virtual ~CameraScript();

private:
	float _speed;

public:
	shared_ptr<Camera> _camera;
	shared_ptr<Transform> _transform;

	shared_ptr<Transform> _targetTransfom;

	Vector3 _offset;

	shared_ptr<Audio> _audio;

	int _oriMousePosX = 0;
	int _oriMousePosY = 0;

	int _mousePosX = 0;
	int _mousePosY = 0;

	float _rotateVal = 0.f;

	virtual void Update() override;

	virtual void LateUpdate() override;
};

