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

	array<shared_ptr<Component>, COMPONENT_COUNT> _components;	// 게임오브젝트가 갖고있는 컴포넌트들
	shared_ptr<MonoBehaviour> _script;														// 게임오브젝트당 스크립트는 하나만 들어간다.

public:
	bool isCheckFrustum = false;	// 프러스텀 체크해야할 오브젝트인가?

	bool activeSelf = true;			// 게임오브젝트가 켜져있는가 꺼져있는가

public:
	// 게임오브젝트에 달린 컴포넌트를 얻어올수있어욤
	template <typename TComponent>
	shared_ptr<TComponent> GetComponent();

	// 게임오브젝트에 달린 스크립트를 얻어올거에여~
	template <typename TComponent>
	shared_ptr<TComponent> GetScript();

	// 컴포넌트를 추가할수있어요!
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
	shared_ptr<TComponent> newComponent = make_shared<TComponent>(this->shared_from_this());	// this로 게임오브젝트도 컴포넌트에 넘겨준다.

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
