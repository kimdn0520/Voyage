#include "pch.h"
#include "MoveObject.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "Transform.h"
#include "GameManager.h"

MoveObject::MoveObject(shared_ptr<GameObject> gameObject)
	:MonoBehaviour(gameObject),
	_trnasform(gameObject->GetTransform()),_moveSpeed(-2.0f)
{

}

MoveObject::~MoveObject()
{

}


void MoveObject::Awake()
{

}

void MoveObject::FixedUpdate()
{
	// 움직이는것
	_trnasform->MoveLook(_moveSpeed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);
}

void MoveObject::Update()
{

}

void MoveObject::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
{

}

void MoveObject::OnTriggerStay(std::shared_ptr<ColliderBase> other)
{

}

void MoveObject::OnTriggerExit(std::shared_ptr<ColliderBase> other)
{

}
