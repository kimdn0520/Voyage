#pragma once

#include "IEventState.h"

class EventMachine;
class Island2Prefab;
class TurtlePrefab;

class EVENT_ADULTHOOD_TURTLE : public IEventState
{
public:
	EVENT_ADULTHOOD_TURTLE(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_TURTLE();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isTalk = false;

	bool _isFade = false;

	bool _isLastFade = false;

	bool _isSellDreamTalk_1 = true;

	bool _isSellDreamTalk_2 = false;

	bool _isSellDreamTalk_3 = false;

	bool _isCameraMove = true;

	bool _isCameraStarMove = false;

	float _turtleSumCameraTrans = 0.f;
	float _turtleCameraTrans = 40.f;

	float _turtleSumCamerRotateX = 0.f;
	float _turtleCamerRotateX = (3.141592) / 7;
	float _totalCameraTime = 0.f;

	float _turtleSumCamerRotateX_2 = 0.f;
	float _turtleCamerRotateX_2 = -(3.141592) / 8;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	float _talkTime = 0.f;

	float _curTime = 0.f;

	shared_ptr<Island2Prefab> _island2Prefab;

	shared_ptr<TurtlePrefab> _turtlePrefab;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void SellDreamScript_1();
	void SellDreamScript_2();
	void SellDreamScript_3();

	void SellDreamScriptCheck_1();
	void SellDreamScriptCheck_2();
	void SellDreamScriptCheck_3();

	void CameraMove();

	void CameraStarMove();
};

