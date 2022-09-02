#pragma once
#include "NewbieEngineDLL.h"

class RaycastHit
{
public:
	NewbieEngine_DLL RaycastHit();
	NewbieEngine_DLL ~RaycastHit();

private:
	Vector3 _point;			// 광선이 충돌체에 부딪히는 세계 공간의 충돌 지점

	Vector3 _distance;		// 광선의 원점에서 충돌 지점까지의 거리

public:
	const Vector3& GetPoint() { return _point; }		

};

