#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"
#include "ObjectInfo.h"

class Transform;

// [Dyanamic]
// �ֺ��� ��� ��ü�� ��ȣ�ۿ��� �����ϴ�.
// ĳ���Ͱ� �߷°� ���� ���� �־ �ٸ� ��ü�� �ε����� �ڷ� �з�����.

// [Kinematic]
// �߷°� ���� ���� ��ũ��Ʈ�� ���� �̵����� �� ����Ѵ�.

// [Static]
// �������� �ʴ� ��ü
// ���̳��� Ÿ���� ������ �ִ� ĳ���Ϳ� �ε��� ����ƽ Ÿ���� ���� �������� �ʴ´�.
// ĳ���͸� ���θ��� �Ѿ����� ��

class Rigidbody : public Component
{
public:
	NewbieEngine_DLL Rigidbody(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Rigidbody();

private:
	shared_ptr<Transform> _transform;

	BodyType _bodyType;			// �ٵ� Ÿ��

	float _mass;				// ���� (������ ū ��ü�� ���� ��ü�� �浹�ϸ� ū ��ü�� ���� ������)

	float _drag;				// Drag ���� ������ ������Ʈ�� ���ſ� ���̰� ���� ũ�� ������ ���Դϴ�. Drag �� �Ϲ����� ���� .001(�ܴ��� �ݼ� ���)�� 10(����) �����Դϴ�.

	float _angularDrag;			// Drag ���� �������� ȸ���ӵ��� ��������.

	float _gravityScale;		// �߷� ũ��(���� 1���ϸ� �⺻ �߷°��ӵ��� 9.8 m/s �� ����)

	Vector3 _gravity;			// �߷�(�⺻ 9.8m/s)

	Vector3 _velocity;			// �ӵ�����

	Vector3 _acceleration;		// ���ӵ�����

	bool _isGravity;			// �߷� �ۿ��� �Ұ��� ������

	bool _isKinematic;			// ��, �浹�� rigdibody�� ������ ��ġ�� ����

	bool _freezePos[3];			
	
	bool _freezeRot[3];			

public:
	BodyType GetBodyType() { return _bodyType; }
	void SetBodyType(BodyType bodyType) { _bodyType = bodyType; }

	float GetMass() { return _mass; }
	void SetMass(float mass) { _mass = mass; }

	float GetDrag() { return _drag; }
	void SetDrag(float drag) { _drag = drag; }

	float GetGravityScale() { return _gravityScale; }
	void SetGravityScale(float gravityScale) { _gravityScale = gravityScale; }

	Vector3 GetVelocity() { return _velocity; }
	void SetVelocity(Vector3 velocity) { _velocity = velocity; }

	void SetAcceleration(Vector3 accel) { _acceleration = accel; }

	void OnGravity(bool value) { _isGravity = value; }

	// ��ġ�� �̵�
	void MovePosition(Vector3 dir);

	// Ư�� �������� ���� ����(���� �ۿ��� ����)
	void AddForce(Vector3 dir);

	// �浹 ó��
	void ResolveCollision(shared_ptr<Rigidbody> otherRigidbody);

	virtual void FixedUpdate() override;
};

