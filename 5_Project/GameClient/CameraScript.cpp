#include "pch.h"
#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Audio.h"
#include "EventMachine.h"

CameraScript::CameraScript(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _audio(gameObject->GetComponent<Audio>()),
	_speed(70.f), _offset(Vector3(0.f, 100.f, 30.f))
{}

CameraScript::~CameraScript()
{}

void CameraScript::Update()
{
	//_audio->PlayEvent("event:/SFX/SFX_WaveSound", false);
}

void CameraScript::LateUpdate()
{
	//if (EventMachine::GetInstance()->isSellDreamTalk)
	//{
	//	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::RBUTTON))
	//	{
	//		_oriMousePosX = InputManager::GetInstance()->GetMousePos().x;
	//		_oriMousePosY = InputManager::GetInstance()->GetMousePos().y;

	//		_transform->OnMouseDown(0, _oriMousePosY);
	//	}
	//	else if (InputManager::GetInstance()->GetButton(KEY_TYPE::RBUTTON))
	//	{
	//		_mousePosX = InputManager::GetInstance()->GetMousePos().x;
	//		_mousePosY = InputManager::GetInstance()->GetMousePos().y;

	//		_rotateVal += (_oriMousePosY - _mousePosY);

	//		if (_rotateVal >= 300)
	//		{
	//			_rotateVal = 300;
	//		}
	//		else if (_rotateVal <= 0)
	//		{
	//			_rotateVal = 0;
	//		}
	//		else
	//		{
	//			_transform->OnMouseMove(0, _mousePosY, 0.25f);
	//		}

	//		/*if ((_oriMousePosY - _mousePosY) > 0 && (_oriMousePosY - _mousePosY < 200))
	//		{
	//			_transform->OnMouseMove(0, _mousePosY, 0.1f);
	//		}*/
	//	}
	//}

	//// ī�޶��� ���� look ���Ϳ��� ������ �̵�
	//if (InputManager::GetInstance()->GetButton(KEY_TYPE::W))
	//	_transform->MoveLook(_speed * TimeManager::GetInstance()->GetDeltaTime());

	//// ī�޶��� ���� look ���Ϳ��� �ڷ� �̵�
	//if (InputManager::GetInstance()->GetButton(KEY_TYPE::S))
	//	_transform->MoveLook(-_speed * TimeManager::GetInstance()->GetDeltaTime());

	//// ī�޶��� ���� right ���Ϳ��� �������� �̵�
	//if (InputManager::GetInstance()->GetButton(KEY_TYPE::A))
	//	_transform->MoveRight(-_speed * TimeManager::GetInstance()->GetDeltaTime());

	//// ī�޶��� ���� right ���Ϳ��� ���������� �̵�
	//if (InputManager::GetInstance()->GetButton(KEY_TYPE::D))
	//	_transform->MoveRight(_speed * TimeManager::GetInstance()->GetDeltaTime());

	//// ī�޶��� ���� up ���Ϳ��� ���� �̵�
	//if (InputManager::GetInstance()->GetButton(KEY_TYPE::E))
	//	_transform->MoveUp(_speed * TimeManager::GetInstance()->GetDeltaTime());

	//// ī�޶��� ���� up ���Ϳ��� �Ʒ��� �̵�
	//if (InputManager::GetInstance()->GetButton(KEY_TYPE::Q))
	//	_transform->MoveUp(-_speed * TimeManager::GetInstance()->GetDeltaTime());

	//if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::RBUTTON))
	//{
	//	_transform->OnMouseDown(InputManager::GetInstance()->GetMousePos().x,
	//							InputManager::GetInstance()->GetMousePos().y);
	//}
	//else if (InputManager::GetInstance()->GetButton(KEY_TYPE::RBUTTON))
	//{
	//	_transform->OnMouseMove(InputManager::GetInstance()->GetMousePos().x,
	//							InputManager::GetInstance()->GetMousePos().y, 0.25f);
	//}

	/*Vector3 updatePos = Vector3(_targetTransfom->GetWorldPosition().x + _offset.x,
								_targetTransfom->GetWorldPosition().y + _offset.y,
								_targetTransfom->GetWorldPosition().z + _offset.z);

	_transform->SetLocalPosition(updatePos);*/
}
