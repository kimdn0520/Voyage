#pragma once
#include "NewbieEngineDLL.h"

class RaycastHit
{
public:
	NewbieEngine_DLL RaycastHit();
	NewbieEngine_DLL ~RaycastHit();

private:
	Vector3 _point;			// ������ �浹ü�� �ε����� ���� ������ �浹 ����

	Vector3 _distance;		// ������ �������� �浹 ���������� �Ÿ�

public:
	const Vector3& GetPoint() { return _point; }		

};

