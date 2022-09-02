#pragma once

#include "IEventState.h"

class EventMachine;

class EVENT_ADULTHOOD_INTRO : public IEventState
{
public:
	EVENT_ADULTHOOD_INTRO(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_INTRO();

private:
	class EventMachine* ref;

	bool _isFade;

	float _alpha;

	float _blinkTime = 5.f;

	float _curSpawnTime = 0.f;
	float _curTime = 0.f;
	int _curMoneyCnt = 0;

	float _talkTime = 0.f;

	bool _isItem1 = false;
	bool _isItem2 = false;
	bool _isItem3 = false;

	int _nowText;

	bool _isEventCheck = false;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void Farming();
	void Talk();
	void ScriptCheck();
};

