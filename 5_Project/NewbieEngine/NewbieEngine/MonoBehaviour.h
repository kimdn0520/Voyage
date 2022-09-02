#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"

class ColliderBase;
class GameObject;

class MonoBehaviour : public Component
{
public:
	NewbieEngine_DLL MonoBehaviour(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~MonoBehaviour();

public:
	// 오브젝트가 켜질때
	virtual void OnEnable() {};

	// 오브젝트가 꺼질때
	virtual void OnDisable() {};

	NewbieEngine_DLL void Instantiate(shared_ptr<GameObject> prefab, Vector3 pos);

	// 프로그램 실행중에 게임오브젝트를 삭제할 수 있도록 해준다.
	NewbieEngine_DLL void Destroy(shared_ptr<GameObject> gameObject);

	virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) {};
	virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) {};
	virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) {};

	/*virtual void OnCollisionEnter(ColliderBase* col) {};
	virtual void OnCollisionStay(ColliderBase* col) {};
	virtual void OnCollisionExit(ColliderBase* col) {};*/

};

