#include "pch.h"
#include "AudioListener.h"
#include "NewbieEngine.h"
#include "Camera.h"
#include "GameObject.h"
#include "AudioEngineDLL.h"

AudioListener::AudioListener(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::AUDIOLISTENER)
{
	
}

AudioListener::~AudioListener()
{
}

void AudioListener::Update()
{

	if (_ListenerCamera == nullptr)
	{
		//이전에 미리 카메라를 넣어놓지 않으면 직접 카메라를 가져온다.

		if (_ListenerCamera != nullptr)
		{
			DLLAudio::SetListener(_ListenerCamera->GetCamera()->GetViewTM(), { 0,0,0 });
		}
	}
	else
	{
		DLLAudio::SetListener(_ListenerCamera->GetCamera()->GetViewTM(), { 0,0,0 });
	}

}

//void AudioListener::SetCameraTransform(std::shared_ptr<Camera> camera)
//{
//	_ListenerCamera = camera;	// GameObject클래스에 Camera컴포넌트클래스를 넣을수 없어서 오류나요.
//}