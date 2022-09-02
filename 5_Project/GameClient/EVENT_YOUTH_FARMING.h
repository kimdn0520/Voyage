#pragma once
#include "IEventState.h"
#include "StarPrefab.h"

class EventMachine;

class EVENT_YOUTH_FARMING : public IEventState
{
public:
	EVENT_YOUTH_FARMING(EventMachine* ref);
	virtual ~EVENT_YOUTH_FARMING();

private:
	class EventMachine* ref;

	bool _isTalk = false;
	float _talkTime = 0.f;
	int _nowText = 0;

	float _curSpawnTime = 0.f;
	float _curTime = 0.f;

	int _curStarCnt = 0;

	float _alpha;

	float _blinkTime = 2.f;

	std::vector<shared_ptr<StarPrefab>> _starPrefab;

	bool _isStar1 = false;
	bool _isStar2 = false;
	bool _isStar3 = false;
	bool _isStar4 = false;
	bool _isStar5 = false;

	int _starIndex = 0;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	void IntroScript();

	void ScriptCheck();

	void FarmingItem();
	
	bool SkipNextScene();
};

