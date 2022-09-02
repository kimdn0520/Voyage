#include "pch.h"
#include "Item.h"
#include "Transform.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameManager.h"

Item::Item(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _destroyTime(20.f), _itemSpeed(10.f)
{}

Item::~Item()
{}

void Item::ContinusMove()
{
	if (_isFarmingItem)
		return;
	if (!_isMove)
	{
		_transform->Rotate(Vector3(0, _rotateSpeed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed * _plus, 0));
		return;
	}
	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
	//_plus *= -1;

	_transform->MoveLook(-_itemSpeed * TimeManager::GetInstance()->GetDeltaTime());
	_transform->Rotate(Vector3(0, _rotateSpeed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed * _plus, 0));

	if (!GameManager::GetInstance()->isPlayerStop)
		_transform->SetLocalPosition(_transform->GetLocalPosition() - GameManager::GetInstance()->_moveToPoint);

	if (_curTime > _destroyTime)
	{
		Destroy(GetGameObject());
	}
}

void Item::BottleMove()
{
	_transform->MoveRight(-_itemSpeed * TimeManager::GetInstance()->GetDeltaTime());
}

void Item::FixedUpdate()
{
	switch (_itemType)
	{
	case ItemType::Heart:
	{
		ContinusMove();
	}
	break;
	case ItemType::Star:
	{
		ContinusMove();
	}
	break;
	case ItemType::BigStar:
	{
		ContinusMove();
	}
	break;
	case ItemType::Money:
	{
		ContinusMove();
	}
	break;
	case ItemType::Note:
	{
		ContinusMove();
	}
	break;
	case ItemType::LilyFlower:
	{
		ContinusMove();
	}
	break;
	case ItemType::Rose:
	{
		ContinusMove();

	}
	break;
	case ItemType::Bottle:
	{
		BottleMove();
	}
	break;
	}
}
