#pragma once

#include "IEventState.h"

class EventMachine;
class MusicNotePrefab;

class EVENT_ADULTHOOD_RHYTHM : public IEventState
{
public:
	EVENT_ADULTHOOD_RHYTHM(EventMachine* ref);
	virtual ~EVENT_ADULTHOOD_RHYTHM();

private:
	class EventMachine* ref;
	float _curSpawnTime = 0.f;
	float _curTime = 0.f;
	int _curheartCnt = 0;

	// text ฐüทร
	float _outTextTime = 0.f;
	bool _isFirstText = false;
	
	bool _isNote1 = false;
	bool _isNote2 = false;
	bool _isNote3 = false;
	bool _isNote4 = false;
	bool _isNote5 = false;
	bool _isNote6 = false;
	bool _isNote7 = false;
	bool _isNote8 = false;
	bool _isNote9 = false;
	bool _isNote10 = false;

	bool _isAddNote1 = false;
	bool _isAddNote2 = false;
	bool _isAddNote3 = false;
	bool _isAddNote4 = false;
	bool _isAddNote5 = false;
	bool _isAddNote6 = false;

	bool _isLastText = false;
	bool _isTalk = false;
	bool _isEventCheck = false;

	int _curNoteCnt = 0;

	int _NoteIdx = 0;

	int _nowText = 0;

	float _talkTime = 0.f;

	float _alpha = 1.f;

	float _blinkTime = 2.f;

	float _itemFloat = -1.f;

	std::vector<shared_ptr<MusicNotePrefab>> _musicNotePrefab;

public:
	virtual void Start() override;
	virtual int Update() override;
	virtual void End() override;


	bool SkipNextEvent();
	void  SkipNextScene();

	void StreamNote();
	void Talk();
	void ScriptCheck();
	void Text();
};

