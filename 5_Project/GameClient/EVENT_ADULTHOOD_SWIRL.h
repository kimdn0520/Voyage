#pragma once

#include "IEventState.h"

class EventMachine;
class SwirlPrefab;

class EVENT_ADULTHOOD_SWIRL : public IEventState
{
public:
	EVENT_ADULTHOOD_SWIRL(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_SWIRL();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isTalk = false;

	bool _isFade = false;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	float _talkTime = 0.f;

	float _curSpawnTime = 0.f;

	bool _isSpawn = false;

	bool _isDeath = false;

	int _curSwirlCnt = 0;

	bool _isSetTextInfo = false;

	bool _isTalk_Y = false;

	bool _isTalk_N = false;

	float _curTime = 0.f;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void IntroScript();

	void ScriptCheck();

	void SpawnSwirl();

	void YesScript();
	void YesScriptCheck();

	void NoScript();
	void NoScriptCheck();
};

