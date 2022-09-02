#pragma once
#include "NewbieEngineDLL.h"

class ColliderBase;

class NoobRay
{
public:
	NewbieEngine_DLL NoobRay();
	NewbieEngine_DLL ~NoobRay();

public:
	Vector4 rayOrigin;			// 광선의 원점(대충 시작 지점)
	
	Vector4 direction;			// 항상 정규화된 벡터, 단위 길이가 아닌 벡터를 할당하면 정규화 할것

	Vector3 point;				// 그 교점

public:
	NewbieEngine_DLL bool Intersect();
};

