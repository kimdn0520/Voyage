#pragma once
#include "IEventState.h"

class EventMachine;
class BottlePrefab;

class EVENT_INFANCY_END : public IEventState
{
public:
	EVENT_INFANCY_END(EventMachine* ref);
	virtual ~EVENT_INFANCY_END();

private:
	class EventMachine* ref;
	shared_ptr<BottlePrefab> bottlePrefab;
	
	bool _isCreate = false;
	bool _isDoubleCheck = false;

	float _alpha;

	float _blinkTime = 2.f;

	float _talkTime = 0.f;

	bool _isFade;

	int _nowText;

	bool _isTalk = false;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	void CreateBottle();
	void Talk();

	void ScriptCheck();


	bool SkipNextEvent();
	void SkipNextScene();
};

