#include "pch.h"
#include "EventController.h"
#include "EventState.h"
#include "EventMachine.h"


EventController::EventController(shared_ptr<GameObject> gameObject)
	:MonoBehaviour(gameObject)
{
}

void EventController::notify(EventMachine*m, EventState*s)
{
	if (s)
	{
		s->on_event(m, this);
	}
}
