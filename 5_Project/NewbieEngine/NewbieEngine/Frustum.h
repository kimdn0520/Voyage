#pragma once
#include "BoundingVolumes.h"

enum PLANE_TYPE
{
	PLANE_FRONT,
	PLANE_BACK,
	PLANE_UP,
	PLANE_DOWN,
	PLANE_LEFT,
	PLANE_RIGHT,

	PLANE_END
};

class Frustum
{
public:
	Frustum();
	~Frustum();

private:
	array<Vector4, PLANE_END> _planes;
	vector<Vector3> _worldPos;

	Matrix _viewMatrix;

public:
	bool ContainsPoint(shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix);

	void FrustumUpdate(Matrix cameraWorldTM, Matrix viewMatrix, float nearZ, float farZ, float windowWidth, float windowHeight, float fov);
};

