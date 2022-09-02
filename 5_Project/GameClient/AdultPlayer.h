#pragma once
#include "MonoBehaviour.h"
#include "Transform.h"

enum class AdultPlayerState
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

class AdultPlayer : public MonoBehaviour
{
public:
	AdultPlayer(shared_ptr<GameObject> gameObject);
	virtual ~AdultPlayer();

private:
	hash<string> hasher;

	shared_ptr<Transform> _transform;

	shared_ptr<GameObject> _gameObject;

	shared_ptr<GameObject> _mainCamera;

	shared_ptr<Panel> _panel;

	shared_ptr<Audio> _audio;

	bool _isFishing = false;

	Vector3 currentPoint;
	Vector3 targetPoint;

	size_t _dodo_Idle;
	size_t _dodo_Row;

	size_t _adultBoy_Idle;
	size_t _adultBoy_Row;

	size_t _adultGirl_Idle;
	size_t _adultGirl_Row;

	size_t _grandMa_Idle;
	size_t _grandMa_Row;
	size_t _grandMa_Die;

	size_t _hero_Idle;
	size_t _hero_Row;

	Vector3 rotation;
	Vector3 _startRoataion;

	AdultPlayerState currentState;
	ItemType  getItemType;

	// speed
	float _speed = 200.f;

	float _timer = 0.0f;
	float _fishingtimer;

	float _h = 0.f;
	float _v = 0.f;
	float _frequency = 0.0f;
	float _shaking;
	float _introTotalTime = 0.f;

	bool _isIdleAnimStart = true;
	bool _isMoveAnimStart = true;

	bool _isSeaGullCheck = false;

	bool _selectRoute = false;

	Vector3 _dir;
	float _dist = 0.f;			// 그 지점으로 이동해야할 거리
	float _distToPoint = 0.f;	// 그 지점으로 이동중인 거리

	vector<shared_ptr<GameObject>> _interactItems;

	shared_ptr<GameObject> _targetItem;

	shared_ptr<GameObject> _adultBoy;
	shared_ptr<GameObject> _adultGirl;
	shared_ptr<GameObject> _grandMa;
	shared_ptr<GameObject> _dodo;

	shared_ptr<GameObject> _fishingGaugeBack;

	shared_ptr<GameObject> _fishingGauge;

	shared_ptr<GameObject> _fishingRange;

	float _beAttackedTime = 0.f;

	int _rotateCheck = 0;

	bool _moveCheck = false;

	bool _isIdleAnimCheck = false;

	bool _isFarmingCancel = false;

public:
	shared_ptr<GameObject> disaster;

public:
	void Idle();
	void Move();
	void ShakeObject();
	void Talk();
	int ShakingBoat(bool check, bool right);
	void Fishing();

	bool ShakingEvent(bool check, bool right);

	bool TargetItem();

	void SetAdultBoy(shared_ptr<GameObject> adultBoy) { _adultBoy = adultBoy; }
	void SetAdultGirl(shared_ptr<GameObject> adultGirl) { _adultGirl = adultGirl; }
	void SetGrandMa(shared_ptr<GameObject> grandMa) { _grandMa = grandMa; }
	void SetDodo(shared_ptr<GameObject> dodo) { _dodo = dodo; }

	void DestroyAdultBoyGirl()
	{
		Destroy(_adultBoy);
		Destroy(_adultGirl);
	}

	void SetMainCamera(shared_ptr<GameObject> gameObject) { _mainCamera = gameObject; }
	void SetFishingGaugeBack(shared_ptr<GameObject> fishingGaugeBack) { _fishingGaugeBack = fishingGaugeBack; }
	void SetFishingGauge(shared_ptr<GameObject> fishingGauge) { _fishingGauge = fishingGauge; }
	void SetFishingRange(shared_ptr<GameObject> fishingRange) { _fishingRange = fishingRange; }

	virtual void Awake() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;

	void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

	bool GetSeaGullCheck() { return _isSeaGullCheck; }

	bool MoveCheck() { return _moveCheck; }

	void SetAdultFarmingCancel(bool check) { _isFarmingCancel = check; }

	void SetStartRotate() { _transform->SetLocalRotation(_startRoataion); }
};

