#pragma once
#include "IEventState.h"

class EventMachine;
class GameObject;

class EVENT_YOUTH_SOLO : public IEventState
{
public:
	EVENT_YOUTH_SOLO(EventMachine* ref);
	virtual ~EVENT_YOUTH_SOLO();

private:
	class EventMachine* ref;

	bool _isTalk = false;
	float _talkTime = 0.f;
	int _nowText = 0;

	bool _isFade = false;
	float _alpha = 1.f;
	float _blinkTime = 2.f;

	bool _isFadeOut = false;

	shared_ptr<GameObject> _boatObject;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	void IntroScript();

	void ScriptCheck();

	bool SkipNextScene();
};

