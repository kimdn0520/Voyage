#pragma once
#include "IEventState.h"

class EventMachine;
class GameObject;

class EVENT_INFANCY_SHAKING : public IEventState
{
public:
	EVENT_INFANCY_SHAKING(EventMachine* ref);
	virtual ~EVENT_INFANCY_SHAKING();

private:
	class EventMachine* ref;

private:
	int _shakingCount = 0;;
	int _rotateCheck = 0;
	bool _isRight = true;
	float _rotateTime = 0.f;

	bool _isLifeCut=true;

	int _nowText = 0;

	float _talkTime = 0.f;

	bool _isEventCheck = false;

	bool _isCheck1 = false;
	bool _isCheck2 = false;
	bool _isCheck3 = false;
	bool _isCheck4 = false;

	float _lastTalkTime=0.f;

	bool _isDeath = false;

	float _alpha = 0.f;

	float _blinkTime = 2.f;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	void ShakingBoat();
	void Talking();

	void ScriptCheck();


	bool SkipNextEvent();
	void SkipNextScene();
};

