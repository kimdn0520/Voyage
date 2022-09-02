#pragma once
#include "IEventState.h"

class EventMachine;

class EVENT_OLDAGE_INTRO : public IEventState
{
public:
	EVENT_OLDAGE_INTRO(EventMachine* ref);
	virtual ~EVENT_OLDAGE_INTRO();

private:
	class EventMachine* ref;

	float _alpha;
	float _blinkTime = 5.f;
	float _curSpawnTime = 0.f;
	float _curTime = 0.f;
	float _talkTime = 0.f;

	int _curFlowerCnt = 0;
	int _nowText = 0;

	bool _isFade = false;
	bool _isEventCheck = false;
	bool _isTalk = false;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void StreamFlower();
	void Talk(); 
	void ScriptCheck();

};