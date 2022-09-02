#pragma once
#include "IEventState.h"

class EventMachine;
class IslandPrefab;
class BirdPrefab;
class BirdDiePrefab;

class EVENT_ADULTHOOD_THUNDER : public IEventState
{
public:
	EVENT_ADULTHOOD_THUNDER(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_THUNDER();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isTalk = false;

	bool _isTalkEventCheck = false;

	float _talkTime = 0.f;

	shared_ptr<IslandPrefab> _islandPrefab;

	shared_ptr<BirdPrefab> _birdPrefab;
	
	shared_ptr<BirdDiePrefab> _birdDiePrefab;

	bool _isFade = false;;

	bool _isThunderCheck;

	bool _isBirdDieAnim;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	float _lightTime = 0.f;

	bool _isFadeIn = false;

	bool _isLastFade = false;
public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void Talk();
	void Thunder();
	void ScriptCheck();
};

