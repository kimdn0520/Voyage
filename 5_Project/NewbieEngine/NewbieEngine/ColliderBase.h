#pragma once
#include "Component.h"
#include "ObjectInfo.h"

class Transform;

/// <summary>
/// ������Ʈ�� ����� �޾ұ� ������ friend class�� gameObject�� ���� �� �ִ�.
/// </summary>
class ColliderBase : public Component
{
public:
	NewbieEngine_DLL ColliderBase(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~ColliderBase();

protected:
	ColliderType _colliderType = {};

	bool _isTrigger = false;		// true���Ǹ� Ʈ�����ϴµ� ���ǰ� ���� ������ ������ �����ʴ´�

	bool _isCol = false;

	std::shared_ptr<Transform> _transform;

	std::vector<std::shared_ptr<ColliderBase>> _curColList;

	std::vector<std::shared_ptr<ColliderBase>> _preColList;

public:
	friend class PhysicsManager;

	friend class NoobRay;

public:
	inline void ClearCurList() { _curColList.clear(); }

	void SetIsCol(bool val) { _isCol = val; }

	void SetIsTrigger(bool val) { _isTrigger = val; }

	virtual bool Intersetcs(shared_ptr<ColliderBase> otherCollider) abstract;
};

