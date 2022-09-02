#pragma once
#include "IEventState.h"

class EventMachine;

class EVENT_TITLE : public IEventState
{
public:
	EVENT_TITLE(EventMachine* ref);
	virtual ~EVENT_TITLE();

private:
	class EventMachine* ref;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;
};

