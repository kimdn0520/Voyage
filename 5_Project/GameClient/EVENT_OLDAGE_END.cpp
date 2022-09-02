#include "pch.h"
#include "EVENT_OLDAGE_END.h"
#include "EventMachine.h"
#include "SceneManager.h"

EVENT_OLDAGE_END::EVENT_OLDAGE_END(EventMachine* ref)
{
}

EVENT_OLDAGE_END::~EVENT_OLDAGE_END()
{
}

void EVENT_OLDAGE_END::Start()
{
	
}

int EVENT_OLDAGE_END::Update()
{
	SceneManager::GetInstance()->SetLoadSceneName("HappyEndingScene");

	SceneManager::GetInstance()->LoadScene("HappyEndingScene");

	return EventMachine::OLDAGE_END;
}


void EVENT_OLDAGE_END::End()
{
	
}

//void EVENT_OLDAGE_FLIASHING::CameraMove()
//{
//	// 카메라 이동해주자. 
//	//  멀어지는 거를 카메라만 할까? 아님 오브젝트로 같이 할까?
//	if (_boatCamerTransCameraTrans <= _boatCamerTrans)
//	{
//		ref->_camera->GetTransform()->MoveLook(12.f * TimeManager::GetInstance()->GetDeltaTime());
//		ref->_camera->GetTransform()->MoveUp(-5.f * TimeManager::GetInstance()->GetDeltaTime());
//		ref->_camera->GetTransform()->MoveRight(-3.f * TimeManager::GetInstance()->GetDeltaTime());
//		_boatCamerTrans += 10.f * TimeManager::GetInstance()->GetDeltaTime();
//	}
//	else
//	{
//		float rotateVal = 0.5f * TimeManager::GetInstance()->GetDeltaTime();
//		_totalCameraTime += TimeManager::GetInstance()->GetDeltaTime();
//
//		if (_boatCamerTransCameraTrans <= _boatCamerRotateX)
//		{
//			if (_totalCameraTime > 0.01f)
//			{
//				// 카메라 올려줌 
//				_boatCamerRotateX += rotateVal;
//				ref->_camera->GetTransform()->RotateXAxis(+rotateVal);
//				_totalCameraTime = 0.f;
//			}
//		}
//		else
//		{
//			_isTalk = false;
//		}
//	}
//	/*ref->_camera->GetTransform()->RotateXAxis(-rotatescale);*/
//	/*_totalCameraTime = 0.f;*/
//	/*	ref->_camera->GetTransform()->MoveLook(10.f * TimeManager::GetInstance()->GetDeltaTime());*/
//}