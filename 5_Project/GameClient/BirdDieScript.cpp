#include "pch.h"
#include "BirdDieScript.h"
#include "InputManager.h"
#include "GameObject.h"

BirdDieScript::BirdDieScript(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject)
{
}

BirdDieScript::~BirdDieScript()
{
}

void BirdDieScript::Awake()
{
	_gameObject = GetGameObject();
	_npc_die = hasher("Action");
}

void BirdDieScript::FixedUpdate()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::KEY_1))
	{
		_gameObject->PlayAnim(_gameObject, _npc_die, false);
	}
}

void BirdDieScript::Update()
{
}

void BirdDieScript::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
{
}

void BirdDieScript::OnTriggerStay(std::shared_ptr<ColliderBase> other)
{
}

void BirdDieScript::OnTriggerExit(std::shared_ptr<ColliderBase> other)
{
}
