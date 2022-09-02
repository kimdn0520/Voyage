#pragma once
#include "IEventState.h"

class EventMachine;
class GameObject;
class BoatPrefab;

class EVENT_INTRO : public IEventState
{
public:
	EVENT_INTRO(EventMachine* ref);
	virtual ~EVENT_INTRO();

private:
	class EventMachine* ref;

private:	
	Vector3 _preCameraRotate;

	float _introSumCamerRotateX = 0.f;
	float _introCamerRotateX = (3.141592) / 2;
	float _introStopTime = 0.f;
	float _totalBoatTime = 0.f;
	float _totalCameraTime = 0.f;
	bool _resetBoat = false;
	bool _isCreate = false;
	bool _isEndIntroTalk = false;
	int _curBoatCnt = 0;

	float _talkTime = 0.f;

	int _nowText = 0;;

	std::vector<shared_ptr<BoatPrefab>> boatPrefab;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:
	bool IntroAnimation();
	void IntroCreateBoat();
	bool IntroCameraAnimation();
	void IntroScript();
	
	void ScriptCheck();

	bool SkipNextEvent();
	void SkipNextScene();
};

