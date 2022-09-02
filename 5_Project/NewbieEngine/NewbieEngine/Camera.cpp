#include "pch.h"
#include "Camera.h"
#include "GameObject.h"
#include "GraphicsEngineManager.h"
#include "GraphicsEngineAPI.h"
#include "Transform.h"
#include "NoobRay.h"

Camera::Camera(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::CAMERA),
	_cameraTransform(gameObject->GetComponent<Transform>()),
	_nearZ(1.0f), _farZ(500.0f), _fov(XM_PI / 4.f),
	_width(static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().width)),
	_height(static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().height)),
	_frustum(make_shared<Frustum>())
{}

Camera::~Camera()
{}

// height : �ٴٿ� ��ü�� �Ÿ��� �ǹ�
void Camera::SetReflection(float height)
{
	Vector3 up, pos, lootAt;
	float pitch, yaw, roll;
	Vector3 camPos = _cameraTransform->GetLocalPosition();
	Vector3 camRoate = _cameraTransform->GetWorldRotation();
	Vector3 axisZ, axisX, axisY;
	//float height = 2.75f;			// water height`

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	pos.x = camPos.x;
	pos.y = -camPos.y + (height * 2.0f);
	pos.z = camPos.z;

	// Calculate the rotation in radians.
	//pitch = camRoate.x * 0.0174532925f;
	yaw = camRoate.y * 0.0174532925f;
	//roll = camRoate.z * 0.0174532925f;

	// Setup where the camera is looking.
	lootAt.x = sinf(yaw) + camPos.x;
	lootAt.y = pos.y;
	lootAt.z = cosf(yaw) + camPos.y;

	axisZ = lootAt - pos;
	axisZ.Normalize();
	axisX = up.Cross(axisZ);
	axisY = axisZ.Cross(axisX);

	float x = axisX.Dot(pos);
	float y = axisY.Dot(pos);
	float z = axisZ.Dot(pos);

	Matrix V
	{
		axisX.x,axisY.x,axisZ.x,0.f,
		axisX.y,axisY.y,axisZ.y,0.f,
		axisX.z,axisY.z,axisZ.z,0.f,
		x,y,z,1.f
	};
	
	_reflectionTM = V;
}

// ������ǥ��� ǥ���� �������� ī�޶� ��ǥ��� ��ȯ�� �ؾ���.
// ���� ��ǥ ��ȯ�� ���������� ������ ��ġ�� �Ǵµ�
// ī�޶� ���� ���忡 �����ϱ� ���÷� ���� ����� ���ϸ�� �׷��� �׳� ī�޶��������� ������� ���ϸ�ȴ�.
// ���� [ī�޶��� ���� ��ǥ ��ȯ ����� �����]�� ī�޶� ��ȯ ���(�� ���) �̴�
// �̰� ������Ʈ�� ������Ŀ� �����ָ� �ȴ�.
void Camera::UpdateViewMatrix()
{
	Vector3 axisX = _cameraTransform->GetRight();			// ī�޶� x��
	Vector3 axisY = _cameraTransform->GetUp();				// ī�޶� y��
	Vector3 axisZ = _cameraTransform->GetLook();			// ī�޶� z�� (look���͸� z�� ��´�.) - ī�޶� �ٶ󺸴� ����
	Vector3 camPos = _cameraTransform->GetLocalPosition();

	axisZ.Normalize();										// z normalize
	axisY = axisZ.Cross(axisX);								// x���̶� �����ؼ� y ���ϰ�
	axisY.Normalize();
	axisX = axisY.Cross(axisZ);								// y���ѰŸ� z�� �����ؼ� x���ϰ�

	float x = -camPos.Dot(axisX);
	float y = -camPos.Dot(axisY);
	float z = -camPos.Dot(axisZ);

	Matrix V
	{
		axisX.x, axisY.x, axisZ.x, 0.f,
		axisX.y, axisY.y, axisZ.y, 0.f,
		axisX.z, axisY.z, axisZ.z, 0.f,
		x, y, z, 1.0f
	};

	_viewTM = V;
}

void Camera::UpdatePerspectiveMatrix()
{
	_width = (static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().width));
	_height = (static_cast<float>(GraphicsEngineManager::GetInstance()->GetWindowInfo().height));

	_projTM = XMMatrixPerspectiveFovLH(_fov, _width / _height, _nearZ, _farZ);

	_orthoProjTM = XMMatrixOrthographicLH(_width, _height, 0.f, 1.0f);
}

NoobRay Camera::ScreenPointToRay(POINT mousePos)
{
	NoobRay ray;

	// Screen���� ���� mousePos�� ViewSpace�� �ٲٴ� ����
	// ��ũ�� -> NDC -> View
	float viewX = (+2.0f * mousePos.x / _width - 1.0f) / _projTM(0, 0);
	float viewY = (-2.0f * mousePos.y / _height + 1.0f) / _projTM(1, 1);

	ray.rayOrigin = { 0.f, 0.f, 0.f , 1.f };
	ray.direction = { viewX, viewY, 1.0f, 0.f};

	// ViewSpace -> WorldSpace�� �ٲ㼭 ray�� �����Ѵ�.
	Matrix viewMatrixInv = XMMatrixInverse(nullptr, _viewTM);
	ray.rayOrigin = XMVector3TransformCoord(ray.rayOrigin, viewMatrixInv);		// ���� �����̽���~
	ray.direction = XMVector3TransformNormal(ray.direction, viewMatrixInv);		// ���� �����̽���~
	XMVector3Normalize(ray.direction);	// ������ ����ȭ �����ش�.

	return ray;
}

void Camera::Update()
{
	// ���, projection �̶� ��� ������Ʈ���ش�.
	UpdateViewMatrix();
	UpdatePerspectiveMatrix();

	_cameraInfo.worldPos = _cameraTransform->GetWorldPosition();
	_cameraInfo.viewTM = _viewTM;
	_cameraInfo.projTM = _projTM;
	_cameraInfo.orthoProjTM = _orthoProjTM;
	_cameraInfo.worldRotation = _cameraTransform->GetWorldRotation();
	_cameraInfo.right = _cameraTransform->GetRight();
	_cameraInfo.up = _cameraTransform->GetUp();
	_cameraInfo.look = _cameraTransform->GetLook();

	// �������ҵ� ���⼭ ������Ʈ���ش�.
	//_frustum->FrustumUpdate(_cameraTransform->GetWorldMatrix(), _viewTM, _nearZ, _farZ, _width, _height, _fov);

	if (_objectType == OBJECT_TYPE::WATER)
		GraphicsEngineManager::GetInstance()->SetWaterCameraInfo(_cameraInfo);
	else
	{
		GraphicsEngineManager::GetInstance()->SetCameraInfo(_cameraInfo);
	}
		
}