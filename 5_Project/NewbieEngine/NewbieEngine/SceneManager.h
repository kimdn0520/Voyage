#pragma once
#include "NewbieEngineDLL.h"

class Scene;
class GameObject;

class SceneManager
{
public:
	SceneManager() {};
	~SceneManager() {};

private:
	static shared_ptr<SceneManager> sceneManager;

public:
	static NewbieEngine_DLL shared_ptr<SceneManager> GetInstance();

private:
	shared_ptr<Scene> _activeScene;
	shared_ptr<Scene> _nextScene;
	map<string, shared_ptr<Scene>> _sceneList;

	string _nextSceneName;

	std::queue<shared_ptr<GameObject>> _destroyObjList;

	queue<shared_ptr<GameObject>> _instantiateGameObjectList;

	//bool _isReservedLoadScene = false;

	// shared_ptr<GameObject> _pickedObject;

public:
	bool isReservedLoadScene = false;

	bool isPause = false;

public:
	// 현재 씬을 업데이트 하쟈~
	void Update();

	// 현재 씬을 렌더 하쟈~
	void Render();
	
	// 현재 씬을 겟또다제~
	NewbieEngine_DLL const shared_ptr<Scene>& GetActiveScene() { return _activeScene; }

	// 예약된 씬이 있는지 없는지~
	//const bool& IsReservedLoadScene() { return _isReservedLoadScene; }
	
	// 저장되어있는 씬리스트들 겟또다제~
	const map<string, shared_ptr<Scene>> GetSceneList() { return _sceneList; }

	// 씬을 넣어주는 함수~
	template <typename TScene>
	void InputScene(string sceneName);

	// 씬을 불러옵니다~
	NewbieEngine_DLL void LoadScene(string sceneName);

	// 예약된 씬을 불러올거야~
	void ReservedSceneLoad();

	// 제거할 게임오브젝트를 destroyObjList에 넣어준다.
	NewbieEngine_DLL void SetRemoveGameObject(shared_ptr<GameObject> gameObj);

	// 제거 예약된 오브젝트를 제거합니다.
	void ReservedRemoveGameObject();

	// 생성할 게임오브젝트를 instantiateGameObjectList에 넣어준다.
	NewbieEngine_DLL void SetInstantiateGameObject(shared_ptr<GameObject> gameObj);

	// 생성 예약된 오브젝트를 생성합니다.
	void ReservedInstantiateGameObject();

	NewbieEngine_DLL string& GetLoadSceneName() { return _nextSceneName; }

	NewbieEngine_DLL void SetLoadSceneName(string name) { _nextSceneName = name; }

	void Release();

	// Pick 할때 쓸건데 잠시 주석쓰~
	// shared_ptr<GameObject> Pick(int screenX, int screenY);
	// shared_ptr<GameObject> GetpickedObject() { return _pickedObject; }
	// void SetPickedObject(shared_ptr<GameObject> pickedObj) { _pickedObject = pickedObj; }

		
};

template<typename TScene>
inline void SceneManager::InputScene(string sceneName)
{
	_sceneList[sceneName] = make_shared<TScene>();
}
