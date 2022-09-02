#pragma once

#include "IEventState.h"

class EventMachine;
class IslandPrefab;
class BirdPrefab;

class EVENT_ADULTHOOD_SEAGULL : public IEventState
{
public:
	EVENT_ADULTHOOD_SEAGULL(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_SEAGULL();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isTalk = false;

	bool _isFade = false;

	float _alpha = 0.f;

	float _blinkTime = 0.f;
	
	float _talkTime = 0.f;

	float _seagullSumCameraTrans = 0.f;
	float _seagullCameraTrans = 40.f;

	float _seagullSumCamerRotateX = 0.f;
	float _seagullCamerRotateX = (3.141592) / 8;
	float _totalCameraTime = 0.f;

	bool _isSeagullTalk = true;

	shared_ptr<IslandPrefab> _islandPrefab;

	shared_ptr<BirdPrefab> _birdPrefab;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void IntroScript();

	void ScriptCheck();

	void CameraMove();
};

