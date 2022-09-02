#include "pch.h"
#include "Frustum.h"

Frustum::Frustum()
{}

Frustum::~Frustum()
{}

bool Frustum::ContainsPoint(shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix)
{
	const Matrix worldViewMatrix = worldMatrix * _viewMatrix;

	// �ٿ�� �ڽ� 8���� ����
	for (int i = 0; i < 8; i++)
	{
		Vector3 pointsWorld = XMVector3Transform(boundingCube->_localPoint[i], worldViewMatrix);

		// 6���� ���������� ������ �ذ��� ����
		for (int j = 0; j < 6; j++)
		{
			// n = (a, b, c)
			// ax + by + cz + d = 0     plane�� (a, b, c, d) �̷��� ���ִٰ���� 
			Vector3 normal = Vector3(_planes[j].x, _planes[j].y, _planes[j].z);

			// � ����� �ϳ��� ����� �ۿ� �ִ°���
			// ��� �������ߵ� break�� ���������� �ٸ����� �񱳸� ����
			if (normal.Dot(pointsWorld) + _planes[j].w > 0)
			{
				break;
			}

			// �� ���ߴµ� �Ⱥ����������� ��� ��� �ȿ� �ִ°Ŵϱ� ��������
			if(j == 5)
				return true;
		}

	}

	// 8���� ���� �� �������� ���ʿ� �����ٸ�.. false
	return false;
}

/// <summary>
/// ������ ���� P(px, py, pz)�� ������ �����..
/// 1) a * px + b * py + c * pz + d > 0            �� P�� ��� ���� ���� �Ѵ�.
/// 2) a* px + b * py + c * pz + d = 0             �� P�� ��� �� �����Ѵ�.
/// 3) a* px + b * py + c * pz + d < 0             �� P�� ��� �Ʒ��� �����Ѵ�.
/// � ��ü�� ��ġ P�� �� 6���� �鿡 ���� ������ ���� ������ �Ͽ� ��� ��� �Ʒ���
/// �����Ѵٸ� �� ��ü�� ����ü �ȿ� �����Ѵ�.
/// </summary>
void Frustum::FrustumUpdate(Matrix cameraWorldTM, Matrix viewMatrix, float nearZ, float farZ, float windowWidth, float windowHeight, float fov)
{
	_viewMatrix = viewMatrix;

	// ����� ����� half_height
	float nearHalfH = tan(fov / 2) * nearZ;

	// ����� ����� half_width
	float nearHalfW = windowWidth / windowHeight * nearHalfH;

	// �� ����� half_height
	float farHalfH = tan(fov / 2) * farZ;

	// �� ����� half_width
	float farHalfW = windowWidth / windowHeight * farHalfH;

	// ������ 8�� ��ġ ����
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

	// ����� �������� �����. XMVECTOR �� (a,b,c,d) �̷��� ���ٺ���ȴ�.
	// �븻���Ͱ� ���� ���ϵ��� �� �����ؾ��Ѵ�. �ð� �ݽð� �ھ�~
	_planes[PLANE_FRONT] = ::XMPlaneFromPoints(_worldPos[0], _worldPos[1], _worldPos[2]);	// CW
	_planes[PLANE_BACK] = ::XMPlaneFromPoints(_worldPos[4], _worldPos[7], _worldPos[5]);	// CCW
	_planes[PLANE_UP] = ::XMPlaneFromPoints(_worldPos[4], _worldPos[5], _worldPos[1]);		// CW
	_planes[PLANE_DOWN] = ::XMPlaneFromPoints(_worldPos[7], _worldPos[3], _worldPos[6]);	// CCW
	_planes[PLANE_LEFT] = ::XMPlaneFromPoints(_worldPos[4], _worldPos[0], _worldPos[7]);	// CW
	_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(_worldPos[5], _worldPos[6], _worldPos[1]);	// CCW
}
