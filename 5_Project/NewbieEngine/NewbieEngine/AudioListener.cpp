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
		//������ �̸� ī�޶� �־���� ������ ���� ī�޶� �����´�.

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
//	_ListenerCamera = camera;	// GameObjectŬ������ Camera������ƮŬ������ ������ ��� ��������.
//}