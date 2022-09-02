#include "pch.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "Scene.h"
#include "GameObject.h"

shared_ptr<SceneManager> SceneManager::sceneManager = nullptr;

shared_ptr<SceneManager> SceneManager::GetInstance()
{
	if (sceneManager == nullptr)
		sceneManager = make_shared<SceneManager>();

	return sceneManager;
}

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	if (!isPause)
	{
		_activeScene->FixedUpdate();

		// 이동 -> 충돌의 fixedupdate 이후 충돌 체크해주는 부분
		PhysicsManager::GetInstance()->Update(_activeScene->_colliders);	
	}

	_activeScene->Update();
	
	if (!isPause)
	{
		_activeScene->LateUpdate();
		_activeScene->FinalUpdate();
	}
}

void SceneManager::Render()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Render();
}

void SceneManager::LoadScene(string sceneName)
{
	isReservedLoadScene = true;

	_nextScene =_sceneList[sceneName];
} 

void SceneManager::ReservedSceneLoad()
{
	if (_activeScene != nullptr)
	{
		_activeScene->AllClearGameObject();
	}

	_activeScene = _nextScene;
	_activeScene->Init();

	_activeScene->Awake();
	_activeScene->Start();

	isReservedLoadScene = false;
}

void SceneManager::SetRemoveGameObject(shared_ptr<GameObject> gameObj)
{
	_destroyObjList.push(gameObj);
}

void SceneManager::ReservedRemoveGameObject()
{
	if (_activeScene != nullptr)
	{
		while (!_destroyObjList.empty())
		{
			shared_ptr<GameObject> destroyGameObj = _destroyObjList.front();

			// 현재 씬에 있는 해당 게임오브젝트를 삭제한다
			_activeScene->RemoveGameObject(destroyGameObj);

			_destroyObjList.pop();
		
			destroyGameObj.reset();
		}
	}
}

void SceneManager::SetInstantiateGameObject(shared_ptr<GameObject> gameObj)
{
	_instantiateGameObjectList.push(gameObj);
}

void SceneManager::ReservedInstantiateGameObject()
{
	if (_activeScene != nullptr)
	{
		if (!_instantiateGameObjectList.empty())
		{
			_activeScene->AddGameObject(_instantiateGameObjectList.front());
		
			_instantiateGameObjectList.front()->Awake();

			_instantiateGameObjectList.front()->Start();

			_instantiateGameObjectList.pop();
		}
	}
}

void SceneManager::Release()
{
	for (auto& scene : _sceneList)
	{
		scene.second->Release();
	}

	_sceneList.clear();
}
