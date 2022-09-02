#pragma once
#include "IEventState.h"

class EventMachine;
class StarPrefab;
class BigStarPrefab;

class EVENT_ADULTHOOD_UPSTAR : public IEventState
{
public:
	EVENT_ADULTHOOD_UPSTAR(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_UPSTAR();

private:
	vector<shared_ptr<StarPrefab>> _starsObj;
	shared_ptr<BigStarPrefab> _bigStar;

	vector <bool> _isStarsCheck = { false, };
	bool _isBigStarCheck = false;

	Vector3 _cameraPos;

	bool _isFade = false;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	bool _isCameraDown = false;

	vector<float> _itemTime = { 0.f, };
	
	float _bigItemTime = 0.f;

	float _bigItemRotate = 0.f;

	float _introSumCamerRotateX = 0.f;

	vector<float >_itemY = { 0.f, };

	float _introCamerRotateX = (3.141592) / 2;

	float _introStopTime = 0.f;

	float _totalCameraTime = 0.f;
	
	int _nowText = 0;

	float _talkTime = 0.f;

	bool _isLastFadeCheck =false;

	bool _isTalkCheck = false;

public:
	class EventMachine* ref;

	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

public:

	bool IntroCameraAnimation();
	void UpStar();
	void Talk();
	void ScriptCheck();
	//void ItemRotate();
};

