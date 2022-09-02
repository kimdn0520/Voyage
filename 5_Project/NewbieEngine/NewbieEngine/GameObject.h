#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"

class MonoBehaviour;
class Camera;
class Transform;
class ColliderBase;
class UI;
class Text;

enum class Tag
{
	Default,
	Player,
	Enemy,
	Ground,
	Ground2,
	Item,
	Npc,
	Disaster,
	BadEnding,
};

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	NewbieEngine_DLL GameObject();
	NewbieEngine_DLL virtual ~GameObject();

private:
	Tag _tag = Tag::Default;	
	size_t _name;

	array<shared_ptr<Component>, COMPONENT_COUNT> _components;	// ���ӿ�����Ʈ�� �����ִ� ������Ʈ��
	shared_ptr<MonoBehaviour> _script;														// ���ӿ�����Ʈ�� ��ũ��Ʈ�� �ϳ��� ����.

public:
	bool isCheckFrustum = false;	// �������� üũ�ؾ��� ������Ʈ�ΰ�?

	bool activeSelf = true;			// ���ӿ�����Ʈ�� �����ִ°� �����ִ°�

public:
	// ���ӿ�����Ʈ�� �޸� ������Ʈ�� ���ü��־��
	template <typename TComponent>
	shared_ptr<TComponent> GetComponent();

	// ���ӿ�����Ʈ�� �޸� ��ũ��Ʈ�� ���ðſ���~
	template <typename TComponent>
	shared_ptr<TComponent> GetScript();

	// ������Ʈ�� �߰��Ҽ��־��!
	template <typename TComponent>
	void AddComponent();

	NewbieEngine_DLL shared_ptr<Camera> GetCamera();

	NewbieEngine_DLL shared_ptr<Transform> GetTransform();

	NewbieEngine_DLL shared_ptr<ColliderBase> GetColliderBase();

	NewbieEngine_DLL shared_ptr<UI> GetUI();

	NewbieEngine_DLL shared_ptr<Text> GetText();

	size_t GetName() { return _name; }

	NewbieEngine_DLL void SetName(string name)
	{
		hash<string> hasher;
		_name = hasher(name);
	};

	NewbieEngine_DLL Tag GetTag() { return _tag; }

	NewbieEngine_DLL void SetTag(Tag tag) { _tag = tag; }

	vector<shared_ptr<GameObject>> _childs;

	vector<shared_ptr<GameObject>>& GetChilds() { return _childs; }

	NewbieEngine_DLL void PlayAnim(shared_ptr<GameObject> gameObject, size_t hashName, bool isLoop);

	NewbieEngine_DLL void SetChild(shared_ptr<GameObject>& child)
	{
		_childs.push_back(child);
	};

	NewbieEngine_DLL void SetActive(bool value);

public:
	void OnTriggerEnter(std::shared_ptr<ColliderBase> other);

	void OnTriggerStay(std::shared_ptr<ColliderBase> other);

	void OnTriggerExit(std::shared_ptr<ColliderBase> other);

public:
	void Awake();
	void Start();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();
};

template<typename TComponent>
inline shared_ptr<TComponent> GameObject::GetComponent()
{
	for (int i = 0; i < COMPONENT_COUNT; i++)
	{
		shared_ptr<TComponent> castedComponent = dynamic_pointer_cast<TComponent>(_components[i]);

		if (castedComponent != nullptr)
			return castedComponent;
	}

	return nullptr;
}

template<typename TComponent>
inline shared_ptr<TComponent> GameObject::GetScript()
{
	shared_ptr<TComponent> castedComponent = dynamic_pointer_cast<TComponent>(_script);

	if (castedComponent != nullptr)
		return castedComponent;

	return nullptr;
}

template<typename TComponent>
inline void GameObject::AddComponent()
{
	shared_ptr<TComponent> newComponent = make_shared<TComponent>(this->shared_from_this());	// this�� ���ӿ�����Ʈ�� ������Ʈ�� �Ѱ��ش�.

	int typeIndex = (int)newComponent->GetType(); 

	if (typeIndex < COMPONENT_COUNT)
	{
		_components[typeIndex] = newComponent;
	}
	else
	{
		_script = dynamic_pointer_cast<MonoBehaviour>(newComponent);
	}
}
