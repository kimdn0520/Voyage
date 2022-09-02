#pragma once

#include "IEventState.h"

class EventMachine;
class GameObject;
class BoatPrefab;
class AdultBoyPrefab;

class EVENT_ADULTHOOD_COMPETITOR : public IEventState
{
public:
	EVENT_ADULTHOOD_COMPETITOR(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_COMPETITOR();

private:
	class EventMachine* ref;

	int _nowText = 0;

	bool _isTalk = false;

	bool _isFade = false;

	float _alpha = 0.f;

	float _blinkTime = 0.f;

	float _talkTime = 0.f;

	float _curTime = 0.f;

	bool _isCompetitor = false;

	bool _isSpawn = false;

	bool _isChoiceEvent = false;

	bool _isDeath = false;

	bool _isSetTextInfo = false;

	bool _isTalk_Y = false;

	bool _isTalk_N = false;

	/*shared_ptr<GameObject> competitorPanel1;

	shared_ptr<GameObject> competitorPanel2;*/

	shared_ptr<BoatPrefab> boatPrefab1;

	shared_ptr<BoatPrefab> boatPrefab2;

	shared_ptr<BoatPrefab> boatPrefab3;

	shared_ptr<AdultBoyPrefab> competitorPrefab1;

	shared_ptr<AdultBoyPrefab> competitorPrefab2;

	shared_ptr<AdultBoyPrefab> competitorPrefab3;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;

	void IntroScript();

	void ScriptCheck();

	void SpawnCompetitor();

	void YesScript();

	void YesScriptCheck();

	void NoScript();

	void NoScriptCheck();
};

