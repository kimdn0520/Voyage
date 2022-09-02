#pragma once
#include "NewbieEngineDLL.h"

class ColliderBase;

class NoobRay
{
public:
	NewbieEngine_DLL NoobRay();
	NewbieEngine_DLL ~NoobRay();

public:
	Vector4 rayOrigin;			// ������ ����(���� ���� ����)
	
	Vector4 direction;			// �׻� ����ȭ�� ����, ���� ���̰� �ƴ� ���͸� �Ҵ��ϸ� ����ȭ �Ұ�

	Vector3 point;				// �� ����

public:
	NewbieEngine_DLL bool Intersect();
};

