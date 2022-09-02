#include "pch.h"
#include "SoundEvent.h"
#include "AudioSystem.h"
#include "FmodHelper.h"
#include "SimpleMath.h"

SoundEvent::SoundEvent() : _id(0), path("")
{

}

SoundEvent::SoundEvent(AudioSystem* system, unsigned int id, const std::string path) : pAudioSystem(system),_id(id),path(path)
{
}


SoundEvent::~SoundEvent()
{
}

bool SoundEvent::IsValid()
{
	bool isSystemExist = false;
	bool isIdExist = false;

	isSystemExist = pAudioSystem != nullptr;
	if (isSystemExist)
	{
		isIdExist = pAudioSystem->GetEventInstance(_id) != nullptr;
	}

	return isSystemExist && isIdExist; // 모두 true일 때 반환하도록함
}

void SoundEvent::Restart()
{
	auto event = pAudioSystem != nullptr ? pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->start();
	}
}

void SoundEvent::Stop(bool isAllowFadeOut)
{
	auto event = pAudioSystem != nullptr ? pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->stop(isAllowFadeOut ? FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_IMMEDIATE);
	}
}

void SoundEvent::SetPaused(bool isPause)
{
	auto event = pAudioSystem != nullptr ? pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->setPaused(isPause);
	}
}

void SoundEvent::SetVolume(float value)
{
	auto event =pAudioSystem != nullptr ?pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->setVolume(value);
	}
}

void SoundEvent::SetPitch(float value)
{
	auto event = pAudioSystem != nullptr ? pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->setPitch(value);
	}
}

void SoundEvent::SetParameter(const char* name, float value)
{
	auto event = pAudioSystem != nullptr ? pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->setVolume(value);
	}
}

bool SoundEvent::GetPaused() const
{
	bool isvalue = false;
	
	auto event = pAudioSystem != nullptr ? pAudioSystem->GetEventInstance(_id) : nullptr;
	
	if (event != nullptr)
	{
		event->getPaused(&isvalue);
	}

	return isvalue;
}

float SoundEvent::GetVolume() const
{
	float value = 0;

	auto event =
		pAudioSystem != nullptr ?
		pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->getVolume(&value);
	}

	return value;
}

float SoundEvent::GetPitch() const
{
	float value = 0;

	auto event =
		pAudioSystem != nullptr ?
		pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->getPitch(&value);
	}

	return value;
}

float SoundEvent::GetParameter(const char* name)
{
	float value = 0;

	auto event =
		pAudioSystem != nullptr ?
		pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		event->getParameterByName(name, &value);
	}

	return value;
}

float SoundEvent::GetEventDistance()
{
	float min = 0;
	float max = 0;

	auto event =
		pAudioSystem != nullptr ?
		pAudioSystem->GetEventDescription(path) : nullptr;

	if (event != nullptr)
	{
		//_event->getProperty(FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE, &value);
		event->getMinMaxDistance(&min, &max);
	}

	return max;
}

bool SoundEvent::Is3D() const
{
	bool is3D = false;

	auto event =
		pAudioSystem != nullptr ?
		pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		FMOD::Studio::EventDescription* eventDesc = nullptr;
		event->getDescription(&eventDesc);
		if (eventDesc != nullptr)
		{
			eventDesc->is3D(& is3D);
		}
	}

	return is3D;
}

void SoundEvent::Set3DAttributes(const DirectX::SimpleMath::Matrix& worldTrans, const DirectX::SimpleMath::Vector3& velocity)
{
	auto event =pAudioSystem != nullptr ?pAudioSystem->GetEventInstance(_id) : nullptr;

	if (event != nullptr)
	{
		FMOD_3D_ATTRIBUTES _3dAtt;

		_3dAtt.position = VectorToFMODVec(worldTrans.Translation());
		_3dAtt.forward = VectorToFMODVec(worldTrans.Forward());
		_3dAtt.up = VectorToFMODVec(worldTrans.Up());
		_3dAtt.velocity = VectorToFMODVec(velocity);

		event->set3DAttributes(&_3dAtt);
	}
}
