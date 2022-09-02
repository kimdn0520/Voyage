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

// height : 바다와 물체의 거리를 의미
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

// 월드좌표계로 표현된 정점들을 카메라 좌표계로 변환을 해야함.
// 월드 좌표 변환의 역방향으로 과정을 거치면 되는데
// 카메라가 지금 월드에 있으니까 로컬로 가는 행렬을 구하면댐 그래서 그냥 카메라월드행렬의 역행렬을 구하면된다.
// 따라서 [카메라의 월드 좌표 변환 행렬의 역행렬]이 카메라 변환 행렬(뷰 행렬) 이다
// 이걸 오브젝트의 월드행렬에 곱해주면 된다.
void Camera::UpdateViewMatrix()
{
	Vector3 axisX = _cameraTransform->GetRight();			// 카메라 x축
	Vector3 axisY = _cameraTransform->GetUp();				// 카메라 y축
	Vector3 axisZ = _cameraTransform->GetLook();			// 카메라 z축 (look벡터를 z축 삼는다.) - 카메라가 바라보는 지점
	Vector3 camPos = _cameraTransform->GetLocalPosition();

	axisZ.Normalize();										// z normalize
	axisY = axisZ.Cross(axisX);								// x축이랑 외적해서 y 구하고
	axisY.Normalize();
	axisX = axisY.Cross(axisZ);								// y구한거를 z랑 외적해서 x구하고

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

	// Screen에서 받은 mousePos를 ViewSpace로 바꾸는 과정
	// 스크린 -> NDC -> View
	float viewX = (+2.0f * mousePos.x / _width - 1.0f) / _projTM(0, 0);
	float viewY = (-2.0f * mousePos.y / _height + 1.0f) / _projTM(1, 1);

	ray.rayOrigin = { 0.f, 0.f, 0.f , 1.f };
	ray.direction = { viewX, viewY, 1.0f, 0.f};

	// ViewSpace -> WorldSpace로 바꿔서 ray를 정의한다.
	Matrix viewMatrixInv = XMMatrixInverse(nullptr, _viewTM);
	ray.rayOrigin = XMVector3TransformCoord(ray.rayOrigin, viewMatrixInv);		// 월드 스페이스로~
	ray.direction = XMVector3TransformNormal(ray.direction, viewMatrixInv);		// 월드 스페이스로~
	XMVector3Normalize(ray.direction);	// 방향은 정규화 시켜준다.

	return ray;
}

void Camera::Update()
{
	// 뷰랑, projection 이랑 계속 업데이트해준다.
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

	// 프러스텀도 여기서 업데이트해준다.
	//_frustum->FrustumUpdate(_cameraTransform->GetWorldMatrix(), _viewTM, _nearZ, _farZ, _width, _height, _fov);

	if (_objectType == OBJECT_TYPE::WATER)
		GraphicsEngineManager::GetInstance()->SetWaterCameraInfo(_cameraInfo);
	else
	{
		GraphicsEngineManager::GetInstance()->SetCameraInfo(_cameraInfo);
	}
		
}