#include "pch.h"
#include "Swirl.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "GameManager.h"

Swirl::Swirl(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _rotateSpeed(500.f), _curTime(0.f), _speed(20.f), _destroyTime(20.f)
{

}

Swirl::~Swirl()
{
}

void Swirl::FixedUpdate()
{
	_curTime += 1 * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	_transform->MoveLook(-_speed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);

	if (!GameManager::GetInstance()->isPlayerStop)
		_transform->SetLocalPosition(_transform->GetLocalPosition() - GameManager::GetInstance()->_moveToPoint);

	if (_curTime > _destroyTime)
	{
		Destroy(GetGameObject());
	}
}

void Swirl::Update()
{
	_transform->Rotate(Vector3(0, _rotateSpeed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed, 0));
}
