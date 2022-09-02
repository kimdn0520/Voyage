#pragma once
#include "IEventState.h"

class EventMachine;
class BottlePrefab;

class EVENT_YOUTH_INTRO : public IEventState
{
public:
	EVENT_YOUTH_INTRO(EventMachine* ref);
	virtual ~EVENT_YOUTH_INTRO();

private:
	class EventMachine* ref;

	shared_ptr<BottlePrefab> bottlePrefab;

	bool _isFade;

	float _alpha;

	float _blinkTime = 5.f;

	bool _isTalk = false;
	float _talkTime = 0.f;
	int _nowText = 0;;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	void IntroScript();

	void ScriptCheck();

	bool SkipNextScene();
};

