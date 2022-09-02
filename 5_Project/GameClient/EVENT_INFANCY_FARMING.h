#pragma once
#include "IEventState.h"

class HeartPrefab;
class EventMachine;

class EVENT_INFANCY_FARMING : public IEventState
{
public:
	EVENT_INFANCY_FARMING(EventMachine* ref);
	virtual ~EVENT_INFANCY_FARMING();

private:
	class EventMachine* ref;

private:
	float _curSpawnTime = 0.f;
	float _curTime = 0.f;
	int _curheartCnt = 0;

	// text ฐüทร
	float _outTextTime=0.f;
	bool _isFirstText = false;
	bool _isHeart1 = false;
	bool _isHeart2 = false;
	bool _isHeart3 = false;

	bool _isAddHeart1 = false;
	bool _isAddHeart2 = false;
	bool _isAddHeart3 = false;
	bool _isAddHeart4 = false;
	bool _isAddHeart5 = false;

	bool _isLastText = false;
	
	int _nowText = 0;

	float _talkTime = 0.f;

	float _alpha = 1.f;

	float _blinkTime = 2.f;

	float _itemFloat=-1.f;

	std::vector<shared_ptr<HeartPrefab>> _heartPrefab;

	int _heartIdx=0;
public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void Text();

	void ScriptCheck();


	bool SkipNextEvent();
	void SkipNextScene();
};

