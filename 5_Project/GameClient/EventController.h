#pragma once
#include "MonoBehaviour.h"

/// <summary>
/// FSM 기반 생각
/// 필요가 있을까? 아ㅓ리ㅏ머ㅣㅏㅓ마살려줘/...
/// machine, state, event
/// </summary>

enum eventState
{
	Stop,
	CutScene,
	Dialogue,
	SmallEvent,
};


class EventMachine;
class EventState;

class EventController : MonoBehaviour
{
public:
	EventController(shared_ptr<GameObject> gameObject);
	virtual ~EventController() {};

public:
	virtual void notify(EventMachine*, EventState*);
};