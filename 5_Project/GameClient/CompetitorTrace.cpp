#include "pch.h"
#include "CompetitorTrace.h"
#include "Transform.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameManager.h"
#include <random>

CompetitorTrace::CompetitorTrace(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _destroyTime(60.f), _speed(16.f), _curTime(0.f)
{}

CompetitorTrace::~CompetitorTrace()
{}

void CompetitorTrace::FixedUpdate()
{
	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	/*std::random_device rd;
	std::mt19937 gen(rd());
	uniform_int_distribution<int> dis(8, 20);*/

	_transform->MoveLook(_speed * TimeManager::GetInstance()->GetDeltaTime());

	if(_player != nullptr)
		_transform->LookAt(_player->GetTransform()->GetWorldPosition());

	if (!GameManager::GetInstance()->isPlayerStop)
		_transform->SetLocalPosition(_transform->GetLocalPosition() - GameManager::GetInstance()->_moveToPoint);

	// °æÀïÀÚ´Â 40ÃÊÈÄ ÆÄ±«
	if (_curTime > _destroyTime)
	{
		Destroy(GetGameObject());
	}
}
