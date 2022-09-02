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
	// �� �̺�Ʈ�� �����Ҷ� �ѹ� ���� ���� ��
}

int EVENT_TITLE::Update()
{
	if(ref->isIntro)
		return EventMachine::INTRO;

	return EventMachine::TITLE;
}

void EVENT_TITLE::End()
{
	// �� �̺�Ʈ�� ���������� �ѹ� ���� ���� ��
	ref->isIntro = false;
}