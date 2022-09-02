
#include "pch.h"
#include "EventState.h"
#include "EventMachine.h"


void EventState::transition(EventMachine*m, EventState*s)
{
	if (m)
	{
		m->transition(s);
	}
}
