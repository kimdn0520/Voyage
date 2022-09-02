#pragma once
#include "MonoBehaviour.h"

/// <summary>
/// Play어가 꼭 가지고 있어야 하는 건 무엇일깡
/// 
/// 
/// </summary>


enum class PlayerState
{
	Idle,
	Move,
	Fishing,
	Dead,
	Talk,
};

class Transform;
class Animator;
class Panel;
class Audio;

class Player : public MonoBehaviour
{
public:
	Player(shared_ptr<GameObject> gameObject);
	virtual ~Player();

private:
	hash<string> hasher;

	shared_ptr<Transform> _transform;

	shared_ptr<GameObject> _gameObject;

	shared_ptr<GameObject> _mainCamera;

	shared_ptr<Audio> _audio;

	shared_ptr<Panel> _panel;

	bool _isFishing = false;

	Vector3 targetPoint;
	Vector3 currentPoint;

	size_t _adultBoy_Idle;
	size_t _adultBoy_Row;

	size_t _adultGirl_Idle;
	size_t _adultGirl_Row;

	Vector3 rotation;
	Vector3 _startRoataion;

	PlayerState currentState;
	ItemType  getItemType;

	// Hp
	int heart;

	// speed
	float _speed = 200.f;

	float _timer = 0.0f;
	float _fishingtimer;

	float _h = 0.f;
	float _v = 0.f;
	float _frequency = 0.0f;
	float _shaking;
	float _introTotalTime=0.f;

	bool _isIdleAnimStart = true;
	bool _isMoveAnimStart = true;

	float _dist = 0.f;			// 그 지점으로 이동해야할 거리
	float _distToPoint = 0.f;	// 그 지점으로 이동중인 거리

	vector<shared_ptr<GameObject>> _interactItems;

	shared_ptr<GameObject> _targetItem;

	shared_ptr<GameObject> _adultBoy;
	shared_ptr<GameObject> _adultGirl;
	shared_ptr<GameObject> _baby;

	shared_ptr<GameObject> _fishingGaugeBack;

	shared_ptr<GameObject> _fishingGauge;
	
	shared_ptr<GameObject> _fishingRange;


	int _rotateCheck=0;

	bool _moveCheck = false;

	bool _isIdleAnimCheck = false;

	bool _isFarmingCancel = false;
public:
	void Idle();
	void Move();
	void ShakeObject();
	int ShakingBoat(bool check, bool right);
	void Talk();
	void Fishing();

	bool ShakingEvent(bool check, bool right);

	bool TargetItem();
	bool IsDead();

	void SetAdultBoy(shared_ptr<GameObject> adultBoy) { _adultBoy = adultBoy; }
	void SetAdultGirl(shared_ptr<GameObject> adultGirl) { _adultGirl = adultGirl; }
	void SetBaby(shared_ptr<GameObject> baby) { _baby = baby; }

	void SetMainCamera(shared_ptr<GameObject> gameObject) { _mainCamera = gameObject; }
	void SetFishingGaugeBack(shared_ptr<GameObject> fishingGaugeBack) { _fishingGaugeBack = fishingGaugeBack; }
	void SetFishingGauge(shared_ptr<GameObject> fishingGauge) { _fishingGauge = fishingGauge; }
	void SetFishingRange(shared_ptr<GameObject> fishingRange) { _fishingRange = fishingRange;}

	virtual void Awake() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;

	void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

	bool MoveCheck() { return _moveCheck; }

	void SetFarmingCancel(bool check) { _isFarmingCancel = check; }
};

