#pragma once
#include "Component.h"
#include "ObjectInfo.h"

class Transform;

/// <summary>
/// 컴포넌트의 상속을 받았기 때문에 friend class인 gameObject를 얻어올 수 있다.
/// </summary>
class ColliderBase : public Component
{
public:
	NewbieEngine_DLL ColliderBase(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~ColliderBase();

protected:
	ColliderType _colliderType = {};

	bool _isTrigger = false;		// true가되면 트리거하는데 사용되고 물리 엔진의 적용을 받지않는다

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

