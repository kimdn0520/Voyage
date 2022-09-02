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
	// ���� ���� ������Ʈ ����~
	void Update();

	// ���� ���� ���� ����~
	void Render();
	
	// ���� ���� �ٶǴ���~
	NewbieEngine_DLL const shared_ptr<Scene>& GetActiveScene() { return _activeScene; }

	// ����� ���� �ִ��� ������~
	//const bool& IsReservedLoadScene() { return _isReservedLoadScene; }
	
	// ����Ǿ��ִ� ������Ʈ�� �ٶǴ���~
	const map<string, shared_ptr<Scene>> GetSceneList() { return _sceneList; }

	// ���� �־��ִ� �Լ�~
	template <typename TScene>
	void InputScene(string sceneName);

	// ���� �ҷ��ɴϴ�~
	NewbieEngine_DLL void LoadScene(string sceneName);

	// ����� ���� �ҷ��ðž�~
	void ReservedSceneLoad();

	// ������ ���ӿ�����Ʈ�� destroyObjList�� �־��ش�.
	NewbieEngine_DLL void SetRemoveGameObject(shared_ptr<GameObject> gameObj);

	// ���� ����� ������Ʈ�� �����մϴ�.
	void ReservedRemoveGameObject();

	// ������ ���ӿ�����Ʈ�� instantiateGameObjectList�� �־��ش�.
	NewbieEngine_DLL void SetInstantiateGameObject(shared_ptr<GameObject> gameObj);

	// ���� ����� ������Ʈ�� �����մϴ�.
	void ReservedInstantiateGameObject();

	NewbieEngine_DLL string& GetLoadSceneName() { return _nextSceneName; }

	NewbieEngine_DLL void SetLoadSceneName(string name) { _nextSceneName = name; }

	void Release();

	// Pick �Ҷ� ���ǵ� ��� �ּ���~
	// shared_ptr<GameObject> Pick(int screenX, int screenY);
	// shared_ptr<GameObject> GetpickedObject() { return _pickedObject; }
	// void SetPickedObject(shared_ptr<GameObject> pickedObj) { _pickedObject = pickedObj; }

		
};

template<typename TScene>
inline void SceneManager::InputScene(string sceneName)
{
	_sceneList[sceneName] = make_shared<TScene>();
}
