#pragma once
#include "IEventState.h"

class EventMachine;

class EVENT_YOUTH_ROW : public IEventState
{
public:
	EVENT_YOUTH_ROW(EventMachine* ref);
	virtual ~EVENT_YOUTH_ROW();

private:
	class EventMachine* ref;

	bool _isTalk = false;
	bool _isTalkEvent = false;
	bool _isEndTalk = false;
	float _talkTime = 0.f;
	int _nowText = 0;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	void IntroScript();

	void ScriptCheck();

	bool SkipNextScene();
};

