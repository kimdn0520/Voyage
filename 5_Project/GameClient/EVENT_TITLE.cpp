#include "pch.h"
#include "EVENT_TITLE.h"
#include "EventMachine.h"

EVENT_TITLE::EVENT_TITLE(EventMachine* ref)
	: ref(ref)
{}

EVENT_TITLE::~EVENT_TITLE()
{
}

void EVENT_TITLE::Start()
{
	// 이 이벤트를 시작할때 한번 실행 해줄 거
}

int EVENT_TITLE::Update()
{
	if(ref->isIntro)
		return EventMachine::INTRO;

	return EventMachine::TITLE;
}

void EVENT_TITLE::End()
{
	// 이 이벤트를 빠져나갈때 한번 실행 해줄 거
	ref->isIntro = false;
}