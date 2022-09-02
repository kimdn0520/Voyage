#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "MonoBehaviour.h"
#include "Transform.h"
#include "Animator.h"
#include "ColliderBase.h"
#include "UI.h"
#include "Text.h"

GameObject::GameObject()
{}

GameObject::~GameObject()
{}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::CAMERA];

	return dynamic_pointer_cast<Camera>(component);
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::TRANSFORM];

	return dynamic_pointer_cast<Transform>(component);
}

shared_ptr<ColliderBase> GameObject::GetColliderBase()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::COLLIDER];

	return dynamic_pointer_cast<ColliderBase>(component);
}

shared_ptr<UI> GameObject::GetUI()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::UI];

	return dynamic_pointer_cast<UI>(component);
}

shared_ptr<Text> GameObject::GetText()
{
	shared_ptr<Component> component = _components[(int)COMPONENT_TYPE::TEXT];

	return dynamic_pointer_cast<Text>(component);
}

void GameObject::PlayAnim(shared_ptr<GameObject> gameObject, size_t hashName, bool isLoop)
{
	if (gameObject->GetComponent<Animator>() != nullptr)
	{
		gameObject->GetComponent<Animator>()->Play(hashName, isLoop);
	}

	if (gameObject->GetChilds().size() != 0)
	{
		for (auto& gameObj : gameObject->GetChilds())
		{
			if (gameObj->GetComponent<Animator>() != nullptr)
			{
				gameObj->GetComponent<Animator>()->Play(hashName, isLoop);
			}

			PlayAnim(gameObj, hashName, isLoop);
		}
	}
}

void GameObject::Awake()
{
	if (_script != nullptr)
		_script->Awake();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}
}

void GameObject::Start()
{
	if (_script != nullptr)
		_script->Start();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}
}

void GameObject::FixedUpdate()
{
	if (_script != nullptr)
		_script->FixedUpdate();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedUpdate();
	}
}

void GameObject::SetActive(bool value)
{
	// 나자신 value 넣구
	activeSelf = value;

	// 하위 오브젝트 value 재귀로 넣주구~
	for (auto& child : _childs)
	{
		child->SetActive(value);
	}
}

void GameObject::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
{
	if(_script != nullptr)
		_script->OnTriggerEnter(other);
}

void GameObject::OnTriggerStay(std::shared_ptr<ColliderBase> other)
{
	if(_script != nullptr)
		_script->OnTriggerStay(other);
}

void GameObject::OnTriggerExit(std::shared_ptr<ColliderBase> other)
{
	if (_script != nullptr)
		_script->OnTriggerExit(other);
}

void GameObject::Update()
{
	if (_script != nullptr)
		_script->Update();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}
}

void GameObject::LateUpdate()
{
	if (_script != nullptr)
		_script->LateUpdate();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	if (_script != nullptr)
		_script->FinalUpdate();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

void GameObject::Render()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Render();
	}
}
