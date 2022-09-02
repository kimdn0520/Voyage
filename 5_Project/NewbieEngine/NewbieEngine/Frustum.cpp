#include "pch.h"
#include "Frustum.h"

Frustum::Frustum()
{}

Frustum::~Frustum()
{}

bool Frustum::ContainsPoint(shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix)
{
	const Matrix worldViewMatrix = worldMatrix * _viewMatrix;

	// 바운딩 박스 8개의 점과
	for (int i = 0; i < 8; i++)
	{
		Vector3 pointsWorld = XMVector3Transform(boundingCube->_localPoint[i], worldViewMatrix);

		// 6개의 프러스텀의 면으로 해결을 본다
		for (int j = 0; j < 6; j++)
		{
			// n = (a, b, c)
			// ax + by + cz + d = 0     plane이 (a, b, c, d) 이렇게 되있다고보면됨 
			Vector3 normal = Vector3(_planes[j].x, _planes[j].y, _planes[j].z);

			// 어떤 평면중 하나라도 벗어나면 밖에 있는거임
			// 모두 음수여야됨 break로 빠져나가고 다른점을 비교를 하자
			if (normal.Dot(pointsWorld) + _planes[j].w > 0)
			{
				break;
			}

			// 다 비교했는데 안빠져나갔으면 모든 평면 안에 있는거니까 보여야함
			if(j == 5)
				return true;
		}

	}

	// 8개의 점이 다 프러스텀 안쪽에 없었다면.. false
	return false;
}

/// <summary>
/// 임의의 한점 P(px, py, pz)와 평면과의 관계는..
/// 1) a * px + b * py + c * pz + d > 0            점 P는 평면 위에 존재 한다.
/// 2) a* px + b * py + c * pz + d = 0             점 P는 평면 상에 존재한다.
/// 3) a* px + b * py + c * pz + d < 0             점 P는 평면 아래에 존재한다.
/// 어떤 물체의 위치 P를 각 6개의 면에 대해 다음과 같은 판정을 하여 모두 평면 아래에
/// 존재한다면 그 물체는 절두체 안에 존재한다.
/// </summary>
void Frustum::FrustumUpdate(Matrix cameraWorldTM, Matrix viewMatrix, float nearZ, float farZ, float windowWidth, float windowHeight, float fov)
{
	_viewMatrix = viewMatrix;

	// 가까운 평면의 half_height
	float nearHalfH = tan(fov / 2) * nearZ;

	// 가까운 평면의 half_width
	float nearHalfW = windowWidth / windowHeight * nearHalfH;

	// 먼 평면의 half_height
	float farHalfH = tan(fov / 2) * farZ;

	// 먼 평면의 half_width
	float farHalfW = windowWidth / windowHeight * farHalfH;

	// 꼭짓점 8개 위치 설정
	Vector4 points[8] =
	{
		{ -nearHalfW, nearHalfH, nearZ, 1.f },		// nearLT
		{ nearHalfW, nearHalfH, nearZ, 1.f },		// nearRT
		{ nearHalfW, -nearHalfH, nearZ, 1.f },		// nearRB
		{ -nearHalfW, -nearHalfH, nearZ, 1.f },		// nearLB
		{ -farHalfW, farHalfH, farZ, 1.f },			// farLT
		{ farHalfW, farHalfH, farZ, 1.f },			// farRT
		{ farHalfW, -farHalfH, farZ, 1.f },			// farRB
		{ -farHalfW, -farHalfH, farZ, 1.f }			// farLB
	};

	_worldPos =
	{
		::XMVector3TransformCoord(points[0], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[1], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[2], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[3], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[4], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[5], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[6], XMMatrixIdentity()),
		::XMVector3TransformCoord(points[7], XMMatrixIdentity())
	};

	// 평면의 방정식을 만든다. XMVECTOR 에 (a,b,c,d) 이렇게 들어간다보면된다.
	// 노말벡터가 밖을 향하도록 잘 유도해야한다. 시계 반시계 자알~
	_planes[PLANE_FRONT] = ::XMPlaneFromPoints(_worldPos[0], _worldPos[1], _worldPos[2]);	// CW
	_planes[PLANE_BACK] = ::XMPlaneFromPoints(_worldPos[4], _worldPos[7], _worldPos[5]);	// CCW
	_planes[PLANE_UP] = ::XMPlaneFromPoints(_worldPos[4], _worldPos[5], _worldPos[1]);		// CW
	_planes[PLANE_DOWN] = ::XMPlaneFromPoints(_worldPos[7], _worldPos[3], _worldPos[6]);	// CCW
	_planes[PLANE_LEFT] = ::XMPlaneFromPoints(_worldPos[4], _worldPos[0], _worldPos[7]);	// CW
	_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(_worldPos[5], _worldPos[6], _worldPos[1]);	// CCW
}
