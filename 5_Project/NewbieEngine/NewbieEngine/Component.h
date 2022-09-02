#pragma once

class GameObject;

enum class COMPONENT_TYPE
{
	// �̵� ��� -> ����(�����̰� �浹�� ������ �����ۿ�?) 
	TRANSFORM,					// FIXED UPDATE 
	COLLIDER,					// FIXED UPDATE
	RIGIDBODY,					// FIXED UPDATE
	PARTICLE,

	ANIMATOR,					// UPDATE
	SKINANIMATOR,				// UPDATE
	CAMERA,						// UPDATE, script ������ LateUpdate
	LIGHT,						// UPDATE

	UI,							// UPDATE, LATEUPDATE,RENDER

	AUDIO,						// UPDATE
	AUDIOLISTENER,				// UPDATE

	TEXT,						// Render
	RENDERER,					// Render
	
	MONO_BEHAVIOUR,				// script
	END,
};

enum
{
	// ������ ��������̾�� ���� ���Ѵ�.
	COMPONENT_COUNT = (int)(COMPONENT_TYPE::END) - 1	
};

class Component
{
public:
	Component(shared_ptr<GameObject> gameObject, COMPONENT_TYPE type);
	virtual ~Component();

private:
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	weak_ptr<GameObject> _gameObject;

public:
	// � Ÿ������ �˾ƿ���.. ���ϱ�?
	COMPONENT_TYPE GetType() { return _type; }

	// lock()�� �����ϸ� shared_ptr ��ü�� �����ؼ� ����� �� �ִ�.
	// �� ������Ʈ�� �޸� ���ӿ�����Ʈ�� ���ü� �ִ�.
	shared_ptr<GameObject> GetGameObject() { return _gameObject.lock(); }
	//shared_ptr<Transform> GetTransform();

	virtual void Awake() { }
	virtual void Start() { }
	virtual void FixedUpdate() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { }

	virtual void Render() { }
};

