#include "pch.h"
#include "Ground.h"
#include "Transform.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameManager.h"

Ground::Ground(shared_ptr<GameObject> gameObject)
	:MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _itemSpeed(4.f)
{

}

Ground::~Ground()
{

}

void Ground::ContinusMove()
{
	if (!_isMove)
		return;

	_transform->MoveLook(-_itemSpeed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);

	if (!GameManager::GetInstance()->isPlayerStop)
		_transform->SetLocalPosition(Vector3(_transform->GetLocalPosition().x,
									 _transform->GetLocalPosition().y,
									_transform->GetLocalPosition().z - GameManager::GetInstance()->_moveToPoint.z));
}

void Ground::FixedUpdate()
{
	ContinusMove();
}
