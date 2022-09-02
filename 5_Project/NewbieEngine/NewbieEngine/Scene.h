#pragma once
#include "NewbieEngineDLL.h"

class GameObject;
class Camera;
class ColliderBase;
class UI;
class Text;

class Scene
{
private:
	std::vector<shared_ptr<GameObject>>			_gameObjects;
	
	// ���ӿ�����Ʈ�� Camera�� ��Ƴ��� vector
	std::vector<shared_ptr<Camera>>				_cameras;
	
	// ���ӿ�����Ʈ�� �ݶ��̴��� ���� ���ӿ�����Ʈ�� ��Ƴ��� vector
	std::vector<std::shared_ptr<ColliderBase>>  _colliders;

	// ���ӿ�����Ʈ�� UI�� ��Ƴ��� vector
	std::vector<std::shared_ptr<UI>> _ui;

	// ���ӿ�����Ʈ�� Text�� ��Ƴ��� vector
	std::vector<std::shared_ptr<Text>> _text;

public:
	static bool isMainCamera;

public:
	friend class SceneManager;

public:
	NewbieEngine_DLL shared_ptr<Camera> GetMainCamera();
	//vector<shared_ptr<Camera>> GetCameras() { return _cameras; }
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

	const vector<shared_ptr<ColliderBase>>& GetColliders() { return _colliders; }

	const vector<shared_ptr<UI>>& GetUI() { return _ui; }

	const vector<shared_ptr<Text>>& GetText() { return _text; }

	// ���� ���� ���ӿ�����Ʈ�� �����ž�
	NewbieEngine_DLL void AddGameObject(shared_ptr<GameObject> gameObject);

	// �� ���ӿ�����Ʈ�� �ڽĿ�����Ʈ�� ������ �־��ַ���.. �Ⱦ�����..?
	NewbieEngine_DLL void FindPushChildObject(shared_ptr<GameObject> gameObject);

	// ���ӿ�����Ʈ�� ����ž�
	NewbieEngine_DLL void RemoveGameObject(shared_ptr<GameObject> gameObject);

	// ���� ���� ��� ���ӿ�����Ʈ�� ����ž�
	NewbieEngine_DLL void AllClearGameObject();

	// �����
	void Release();

	// ������ ������Ʈ���� ����ų� �Ҷ�
	virtual void Init() {};
	
	void Awake();				// ���� ���� �ʱ�ȭ �ϴ� �Լ�
	void Start();				// ���� �Լ�
	void FixedUpdate();			// ���� ������Ʈ(�̵� -> �浹 -> ����?)
	void Update();				// ������Ʈ
	void LateUpdate();			// ī�޶� ������ ������Ʈ
	void FinalUpdate();			// ������ ������Ʈ

	void Render();				// ������ �׷��Ƚ��� �Ѱ�����
};

