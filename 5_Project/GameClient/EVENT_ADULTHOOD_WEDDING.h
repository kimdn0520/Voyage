#pragma once
#include "IEventState.h"

class EventMachine;

class EVENT_ADULTHOOD_WEDDING : public IEventState
{
public:
	EVENT_ADULTHOOD_WEDDING(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_WEDDING();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isFade = false;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	bool _isLastFade = false;

	float _curSpawnTime = 0.f;

	float _totalCameraTime = 0.f;

	int _curRoseCnt = 0;

	float _curTime = 0.f;

	bool _isTalk = false;

	float _talkTime = 0.f;

	bool _isMusic=false;

	bool _isCheck1=false;
	bool _isCheck2=false;
	bool _isCheck3=false;
	bool _isCheck4=false;

	bool _isJump = false;
public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;


	void Farming();
	void Talk();
};

