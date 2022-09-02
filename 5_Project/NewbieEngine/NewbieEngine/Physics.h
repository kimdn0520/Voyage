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
	// ���콺 ��ġ ��ǥ���� ���� ray�� ���ؼ� raycastHit�� hit point�� �����Ѵ�.
	bool RayCast(NoobRay ray, RaycastHit raycastHit, float distance);
};

