#pragma once

class GameObject;

enum class COMPONENT_TYPE
{
	// 이동 행렬 -> 물리(움직이고 충돌을 했으면 물리작용?) 
	TRANSFORM,					// FIXED UPDATE 
	COLLIDER,					// FIXED UPDATE
	RIGIDBODY,					// FIXED UPDATE
	PARTICLE,

	ANIMATOR,					// UPDATE
	SKINANIMATOR,				// UPDATE
	CAMERA,						// UPDATE, script 에서는 LateUpdate
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
	// 렌더랑 모노비헤비이어는 포함 안한다.
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
	// 어떤 타입인지 알아오깅.. 쓰일까?
	COMPONENT_TYPE GetType() { return _type; }

	// lock()이 성공하면 shared_ptr 객체를 리턴해서 사용할 수 있다.
	// 이 컴포넌트가 달린 게임오브젝트를 얻어올수 있다.
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

