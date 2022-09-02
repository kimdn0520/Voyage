#pragma once
#include "IEventState.h"

class EventMachine;

class EVENT_OLDAGE_END : public IEventState
{

public:
	EVENT_OLDAGE_END(EventMachine* ref);
	virtual ~EVENT_OLDAGE_END();

private:
	class EventMachine* ref;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;



	//void CameraMove();
	//void ActiveCredit();
};

