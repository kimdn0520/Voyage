#pragma once
#include "NewbieEngineDLL.h"
#include "AlignedAllocationPolicy.h"
#include "Component.h"
#include "ObjectInfo.h"
#include "Frustum.h"

class Transform;
class Frusutum;
class NoobRay;

enum class PROJECTION_TYPE
{
	PERSPECTIVE,			 // 원근 투영
	ORTHOGRAPHIC,			 // 직교 투영
};

class Camera : public Component, public AlignedAllocationPolicy<16>
{
public:
	NewbieEngine_DLL Camera(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Camera();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	Vector3 _targetPos;

	shared_ptr<Transform> _cameraTransform;

	// z를 시야로부터의 거리로 삼아 정점의 x, y좌표를 거리에 비례하여 수정한다.
	// near와 far 를 1 ~ 1000 으로 범위를 써주면 파이프라인 과정을 거치면서 0 ~ 1 로 변환이된다.
	float _nearZ;
	float _farZ;
	float _fov;
	float _width;
	float _height;

	Matrix _viewTM = {};
	Matrix _projTM = {};
	Matrix _orthoProjTM = {};
	// YD test
	Matrix _reflectionTM = {};

	CameraInfo _cameraInfo;

	shared_ptr<Frustum> _frustum;

	OBJECT_TYPE _objectType;

public:
	Matrix GetViewTM() { return _viewTM; }
	Matrix GetProjTM() { return _projTM; }
	// YD test
	Matrix GetReflectionViewMatrix() { return _reflectionTM; }

	void SetNear(float value) { _nearZ = value; }
	void SetFar(float value) { _farZ = value; }
	void SetFOV(float value) { _fov = value; }
	// YD test
	void SetReflection(float height);
	void SetObjectType(OBJECT_TYPE objecttype) { _objectType = objecttype; }
	

	void UpdateViewMatrix();
	void UpdatePerspectiveMatrix();

	// 마우스로 찍은 위치의 좌표값을 WorldSpace로 변환을해서
	// Ray에 시작지점과 방향을 저장한다.
	NewbieEngine_DLL NoobRay ScreenPointToRay(POINT mousePos);

	const shared_ptr<Frustum>& GetFrustum() { return _frustum; }

	virtual void Update() override;
};