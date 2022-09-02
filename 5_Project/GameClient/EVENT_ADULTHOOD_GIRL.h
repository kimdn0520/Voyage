#pragma once
#include "IEventState.h"

class EventMachine;

class EVENT_ADULTHOOD_GIRL : public IEventState
{
public:
	EVENT_ADULTHOOD_GIRL(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_GIRL();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isFade = false;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	bool _isGirlCome = false;

	float _comingTime = 0.f;

	float _moveRange = 0.f;

	bool _isTalk = false;

	float _talkTime = 0.f;
public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;


	void ComeGirl();

	void Talk();
	void ScriptCheck();
};

