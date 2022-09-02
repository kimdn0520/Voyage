#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Resources.h"
#include "Transform.h"
#include "UI.h"
#include "Text.h"

shared_ptr<Camera> Scene::GetMainCamera()
{
	if (_cameras.empty())
		return nullptr;

	return _cameras[0];
}

//void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
//{
//	// Camera 오브젝트는 따로 _cameras 에도 모아준다.(Camera 형태로 넣어준다.)
//	if (gameObject->GetCamera() != nullptr)
//	{
//		_cameras.push_back(gameObject->GetCamera());
//	}
//
//	// ColliderBase를 가지고있는 게임오브젝트라면 ColliderBase만 따로 모아준다.
//	if (gameObject->GetColliderBase() != nullptr)
//	{
//		_colliders.push_back(gameObject->GetColliderBase());
//	}
//
//	// UI를 가지고있는 게임오브젝트라면 UI만 따로 모아준다.
//	if (gameObject->GetUI() != nullptr)
//	{
//		_ui.push_back(gameObject->GetUI());
//	}
//
//	// Text를 가지고있는 게임오브젝트라면 UI만 따로 모아준다.
//	if (gameObject->GetText() != nullptr)
//	{
//		_text.push_back(gameObject->GetText());
//	}
//
//	_gameObjects.push_back(gameObject);
//}

void Scene::FindPushChildObject(shared_ptr<GameObject> gameObject)
{
	AddGameObject(gameObject);

	if (gameObject->GetChilds().size() != 0)
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			//AddGameObject(gameObject->GetChilds()[i]);
			FindPushChildObject(gameObject->GetChilds()[i]);
		}
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	// Camera 오브젝트는 따로 _cameras 에도 모아준다.(Camera 형태로 넣어준다.)
	if (gameObject->GetCamera() != nullptr)
	{
		_cameras.push_back(gameObject->GetCamera());
	}

	// ColliderBase를 가지고있는 게임오브젝트라면 ColliderBase만 따로 모아준다.
	if (gameObject->GetColliderBase() != nullptr)
	{
		_colliders.push_back(gameObject->GetColliderBase());
	}

	// UI를 가지고있는 게임오브젝트라면 UI만 따로 모아준다.
	if (gameObject->GetUI() != nullptr)
	{
		_ui.push_back(gameObject->GetUI());
	}

	// Text를 가지고있는 게임오브젝트라면 UI만 따로 모아준다.
	if (gameObject->GetText() != nullptr)
	{
		_text.push_back(gameObject->GetText());
	}

	_gameObjects.push_back(gameObject);

	// 넣어줄 게임오브젝트가 자식오브젝트를 가지고 있다면 찾아가서 씬에 넣어준다.
	if (gameObject->GetChilds().size() != 0)
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			AddGameObject(gameObject->GetChilds()[i]);
		}
	}
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(_cameras.begin(), _cameras.end(), gameObject->GetCamera());
		if (findIt != _cameras.end())
		{
			_cameras.erase(findIt);
			findIt->reset();
		}
	}

	if (gameObject->GetColliderBase())
	{
		auto findIt = std::find(_colliders.begin(), _colliders.end(), gameObject->GetColliderBase());
		if (findIt != _colliders.end())
		{
			findIt->reset();
			_colliders.erase(findIt);
		}
	}

	if (gameObject->GetUI())
	{
		auto findIt = std::find(_ui.begin(), _ui.end(), gameObject->GetUI());
		if (findIt != _ui.end())
		{
			findIt->reset();
			_ui.erase(findIt);
		}
	}

	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
	{
		findIt->reset();
		_gameObjects.erase(findIt);
	}

	if (gameObject->GetChilds().size() != 0)
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			RemoveGameObject(gameObject->GetChilds()[i]);
		}
	}
}

void Scene::AllClearGameObject()
{
	for (auto& gameObject : _gameObjects)
		gameObject.reset();

	for (auto& camera : _cameras)
		camera.reset();

	for (auto& collider : _colliders)
		collider.reset();

	for (auto& ui : _ui)
		ui.reset();

	for (auto& text : _text)
		text.reset();

	_gameObjects.clear();
	_cameras.clear();
	_colliders.clear();
	_ui.clear();
	_text.clear();
}

void Scene::Release()
{
	for (auto& gameObject : _gameObjects)
		gameObject.reset();

	for (auto& camera : _cameras)
		camera.reset();

	for (auto& collider : _colliders)
		collider.reset();

	for (auto& ui : _ui)
		ui.reset();

	for (auto& text : _text)
		text.reset();

	_gameObjects.clear();
	_cameras.clear();
	_colliders.clear();
	_ui.clear();
	_text.clear();
}

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::FixedUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}
}

void Scene::Render()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		/*if (gameObject->isCheckFrustum)
		{
			if (!_cameras[0]->GetFrustum()->ContainsPoint(
				Resources::GetInstance()->GetBoundingCube(gameObject->GetName()), gameObject->GetTransform()->GetWorldMatrix()))
			{
				continue;
			}
		}*/

		// 게임오브젝트 activeslef true 인것만 렌더쪽에 넘겨줄거야
		if (gameObject->activeSelf)
			gameObject->Render();
	}
}
