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
	PERSPECTIVE,			 // ���� ����
	ORTHOGRAPHIC,			 // ���� ����
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

	// z�� �þ߷κ����� �Ÿ��� ��� ������ x, y��ǥ�� �Ÿ��� ����Ͽ� �����Ѵ�.
	// near�� far �� 1 ~ 1000 ���� ������ ���ָ� ���������� ������ ��ġ�鼭 0 ~ 1 �� ��ȯ�̵ȴ�.
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

	// ���콺�� ���� ��ġ�� ��ǥ���� WorldSpace�� ��ȯ���ؼ�
	// Ray�� ���������� ������ �����Ѵ�.
	NewbieEngine_DLL NoobRay ScreenPointToRay(POINT mousePos);

	const shared_ptr<Frustum>& GetFrustum() { return _frustum; }

	virtual void Update() override;
};