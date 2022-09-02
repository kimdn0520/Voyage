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
	// ������Ʈ�� ������
	virtual void OnEnable() {};

	// ������Ʈ�� ������
	virtual void OnDisable() {};

	NewbieEngine_DLL void Instantiate(shared_ptr<GameObject> prefab, Vector3 pos);

	// ���α׷� �����߿� ���ӿ�����Ʈ�� ������ �� �ֵ��� ���ش�.
	NewbieEngine_DLL void Destroy(shared_ptr<GameObject> gameObject);

	virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) {};
	virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) {};
	virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) {};

	/*virtual void OnCollisionEnter(ColliderBase* col) {};
	virtual void OnCollisionStay(ColliderBase* col) {};
	virtual void OnCollisionExit(ColliderBase* col) {};*/

};

