#pragma once
#include "NewbieEngineDLL.h"

class NoobRay;
class RaycastHit;

class Physics
{
public:
	NewbieEngine_DLL Physics();
	NewbieEngine_DLL ~Physics();

private:
	
public:
	// 마우스 위치 좌표값을 받은 ray를 통해서 raycastHit의 hit point를 저장한다.
	bool RayCast(NoobRay ray, RaycastHit raycastHit, float distance);
};

