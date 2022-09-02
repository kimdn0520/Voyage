#pragma once
#include "MonoBehaviour.h"

class Transform;

class Ground : public MonoBehaviour
{
public:
	Ground(shared_ptr<GameObject> gameObject);
	virtual ~Ground();

private:
	ItemType _itemType;

	shared_ptr<Transform> _transform;

	float _itemSpeed;				// GameManager�� ���� speed�� �ο��޾ƾ���

	bool _isMove = true;

public:
	void ContinusMove();

	void SetIsMove(bool val) { _isMove = val; }

	void FixedUpdate() override;
};

