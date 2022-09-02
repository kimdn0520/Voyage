#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"
#include "ObjectInfo.h"

class Transform;

// [Dyanamic]
// 주변의 모든 물체와 상호작용이 가능하다.
// 캐릭터가 중력과 힘을 갖고 있어서 다른 물체와 부딪히면 뒤로 밀려난다.

// [Kinematic]
// 중력과 힘이 없고 스크립트에 의해 이동만할 때 사용한다.

// [Static]
// 움직이지 않는 물체
// 다이나믹 타입을 가지고 있는 캐릭터와 부딪힌 스태틱 타입의 돌은 움직이지 않는다.
// 캐릭터만 돌부리에 넘어지는 것

class Rigidbody : public Component
{
public:
	NewbieEngine_DLL Rigidbody(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Rigidbody();

private:
	shared_ptr<Transform> _transform;

	BodyType _bodyType;			// 바디 타입

	float _mass;				// 질량 (질량이 큰 물체와 작은 물체가 충돌하면 큰 물체가 적게 움직임)

	float _drag;				// Drag 값이 작으면 오브젝트가 무거워 보이고 값이 크면 가벼워 보입니다. Drag 의 일반적인 값은 .001(단단한 금속 덩어리)과 10(깃털) 사이입니다.

	float _angularDrag;			// Drag 값이 높을수록 회전속도가 느려진다.

	float _gravityScale;		// 중력 크기(값을 1로하면 기본 중력가속도인 9.8 m/s 가 적용)

	Vector3 _gravity;			// 중력(기본 9.8m/s)

	Vector3 _velocity;			// 속도벡터

	Vector3 _acceleration;		// 가속도벡터

	bool _isGravity;			// 중력 작용을 할건지 말건지

	bool _isKinematic;			// 힘, 충돌이 rigdibody에 영향을 미치지 않음

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

	// 위치로 이동
	void MovePosition(Vector3 dir);

	// 특정 방향으로 힘을 가함(관성 작용이 있음)
	void AddForce(Vector3 dir);

	// 충돌 처리
	void ResolveCollision(shared_ptr<Rigidbody> otherRigidbody);

	virtual void FixedUpdate() override;
};

