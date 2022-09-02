#pragma once
#include "IEventState.h"


class EventMachine;

class EVENT_OLDAGE_FLIASHING : public IEventState
{
public:
	EVENT_OLDAGE_FLIASHING(EventMachine* ref);
	virtual~EVENT_OLDAGE_FLIASHING();


private:
	class EventMachine* ref;

	float _alpha = 1.0f;
	float _blinkTime = 5.f;
	float _curSpawnTime = 0.f;
	float _curTime = 0.f;
	float _talkTime = 0.f;
	float _totalCameraTime = 0.f;
	
	//// Áøµ¿ 	
	//float _frequency = 0.0f;

	float _boatCamerTrans = 5.f;
	float _boatCamerTransCameraTrans = 0.f;

	float _boatSumCamerRotateX = 0.f;
	float _boatCamerRotateX = (3.141592) / 8;


	int _curFlowerCnt = 0;
	int _nowText;

	bool _isLastTalk = false;
	bool _isFade = false;
	
	bool _isEventCheck = false;
	
	bool _isTalk = false;
	bool _isFlising = false;
	
	bool _isFlisingTalk = true;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void Talk();
	void FlishingTalk();
	
	void FliashingCheck();
	void ScriptCheck();

	void StreamFlower();
	void CameraMove();
};

