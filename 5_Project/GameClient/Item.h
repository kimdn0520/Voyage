#pragma once
#include "MonoBehaviour.h"

class Transform;

class Item : public MonoBehaviour
{
public:
	Item(shared_ptr<GameObject> gameObject);
	virtual ~Item();

private:
	ItemType _itemType;

	shared_ptr<Transform> _transform;

	float _curTime = 0.f;

	float _destroyTime;

	float _itemSpeed;				// GameManager로 부터 speed를 부여받아야함

	bool _isMove = true;

	bool _isFarmingItem = false;

	float _rotateSpeed = 100.0f;

	float _plus = -1;

public:
	const ItemType& GetItemType() { return _itemType; }

	void SetType(ItemType itemType) { _itemType = itemType; }

	void SetIsMove(bool val) { _isMove = val; }

	void SetIsFarmingItem(bool val) { _isFarmingItem = val; }

	void ContinusMove();

	void BottleMove();

	void FixedUpdate() override;

	void SetPlus(float num) { _plus = num; }

	bool GetFarmingItem() { return _isFarmingItem; }
};

