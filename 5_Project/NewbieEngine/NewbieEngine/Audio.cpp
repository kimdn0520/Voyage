#include "pch.h"
#include "Audio.h"
#include "AudioEngineDLL.h"
#include "AudioDLLDefine.h"
#include "SoundEvent.h"
#include "GameObject.h"
#include "Transform.h"

Audio::Audio(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::AUDIO),
	_transform(gameObject->GetTransform())
{}

Audio::~Audio()
{
	AllEventStop();
}

void Audio::Update()
{
	for (int index = 0; index < _3DEvent_V.size();)
	{
		//유효하지 않으면
		if (!_3DEvent_V.at(index)->IsValid())
		{
			_3DEvent_V.erase(_3DEvent_V.begin() + index);
		}
		else
		{
			_3DEvent_V.at(index)->Set3DAttributes(_transform->GetWorldMatrix());
			index++;
		}
	}

	for (int index = 0; index < _2DEvent_V.size();)
	{
		//유효하지 않으면
		if (!_2DEvent_V.at(index)->IsValid())
		{
			_2DEvent_V.erase(_2DEvent_V.begin() + index);
		}
		else
		{
			index++;
		}
	}
}

SoundEvent* Audio::PlayEvent(const char* path, bool duplicatePlay)
{
	SoundEvent* _event = nullptr;
	//중복재생 비허용이면
	if (duplicatePlay == false)
	{
		if (GetIsAlreadyPlaying(path))
		{
			return _event;
		}
	}

	_event = DLLAudio::PlayEvent(std::string(path));

	//존재하는 sound 경로일때 
	if (_event != nullptr)
	{
		if (_event->Is3D())
		{
			_3DEvent_V.emplace_back(_event);
			_event->Set3DAttributes(_transform->GetWorldMatrix());
		}
		else
		{
			_2DEvent_V.emplace_back(_event);
		}
	}

	return _event;
}

SoundEvent* Audio::PlayEvent(const char* path, const char* parameter, float parameterValue, bool duplicatePlay)
{
	SoundEvent* _event = Audio::PlayEvent(path, duplicatePlay);
	if (_event != nullptr)
	{
		_event->SetParameter(parameter, parameterValue);
	}
	return _event;
}

float Audio::GetEventDistance(const char* path)
{
	return DLLAudio::GetEventDistance(path);
}

void Audio::AllEventStop(bool fadeout)
{
	// Stop all sounds
	for (auto& event : _2DEvent_V)
	{
		event->Stop(fadeout);
	}
	for (auto& event : _3DEvent_V)
	{
		event->Stop(fadeout);
	}

	// Clear events
	_2DEvent_V.clear();
	_3DEvent_V.clear();
}

bool Audio::GetIsAlreadyPlaying(const char* path)
{
	if (DLLAudio::GetIs3D(path))
	{
		for (int index = 0; index < _3DEvent_V.size(); index++)
		{
			if (_3DEvent_V[index]->path == std::string(path))
			{
				//이미 재생중입니다.
				return true;
			}
		}

	}
	else
	{
		for (int index = 0; index < _2DEvent_V.size(); index++)
		{
			if (_2DEvent_V[index]->path == std::string(path))
			{
				//이미 재생중입니다.
				return true;
			}
		}
	}

	//재생중이지 않습니다.
	return false;
}
