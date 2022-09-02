#pragma once

class EventMachine;
class EventController;

class EventState
{
public:
	EventState() {};
	virtual ~EventState() {};

public:
	virtual void on_entry(EventMachine*, EventState*) {};
	virtual void on_exit(EventMachine*, EventState*) {};
	virtual void on_event(EventMachine*, EventController*) {};

public:
	virtual void transition(EventMachine*, EventState*);
};

