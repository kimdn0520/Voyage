#include "pch.h"
#include "AudioEngineDLL.h"
#include "AudioSystem.h"
#include "SoundEvent.h"


static IAudioSystem* GAudioEngine = nullptr;

_FMODAUDIODLL bool DLLAudio::Initialize(const char* bankPath, int maxChanelCnt)
{
	bool isResult = false;
	if (GAudioEngine == nullptr)
	{
		GAudioEngine = new AudioSystem();
		isResult = GAudioEngine->Initialize(maxChanelCnt, bankPath);
	}
	else
	{
		_TRACE("FMOD Audio : Already Initialized");
	}
	return isResult;
}

_FMODAUDIODLL void DLLAudio::Release()
{
	if (GAudioEngine!= nullptr)
	{
		GAudioEngine->Release();
	}
	else
	{
		_TRACE("FMOD Audio : Not thing to Release");
	}

}

_FMODAUDIODLL void DLLAudio::Set3DSetting(float dopplerScale, float gameScale)
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->Set3DSetting(dopplerScale, gameScale, 1.0f);
	}
	else
	{
		_TRACE("FMOD Audio : (Set3DSetting)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL bool DLLAudio::LoadAllBank(const std::vector<std::string>& vec)
{
	bool isResult = false;
	if (GAudioEngine != nullptr)
	{
		isResult = GAudioEngine->LoadAllBank(vec);
	}
	else
	{
		_TRACE("FMOD Audio : (LoadAllBank)Not Initialized AudioEngine");
	}
	return isResult;
}

_FMODAUDIODLL void DLLAudio::Update()
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->Update();
	}
	else
	{
		_TRACE("FMOD Audio : (Update)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL SoundEvent* DLLAudio::PlayEvent(const std::string& soundpath)
{
	if (GAudioEngine != nullptr)
	{
		return GAudioEngine->PlayEvent(soundpath);
	}
	else
	{
		_TRACE("FMOD Audio - (PlayEvent)Not Initialized AudioEngine");
		return nullptr;
	}
}

_FMODAUDIODLL void DLLAudio::SetListener(const DirectX::SimpleMath::Matrix& veiewMatrix, const DirectX::SimpleMath::Vector3& velocity)
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->SetListener(veiewMatrix, velocity);
	}
	else
	{
		_TRACE("FMOD Audio - (SetListener)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL void DLLAudio::SetMasterVolume(float val)
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->SetMasterVolume(val);
	}
	else
	{
		_TRACE("FMOD Audio - (SetMasterVolume)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL void DLLAudio::SetAllBusVolume(float val)
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->SetAllBusVolume(val);
	}
	else
	{
		_TRACE("FMOD Audio - (SetAllBusVolume)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL float DLLAudio::GetBusVolume(const char* name)
{
	float value = 0;;
	if (GAudioEngine != nullptr)
	{
		value = GAudioEngine->GetBusVolume(name);
	}
	else
	{
		_TRACE("FMOD Audio - (GetBusVolume)Not Initialized AudioEngine");
	}
	return value;
}

_FMODAUDIODLL void DLLAudio::SetBusVolume(const char* name, float val)
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->SetBusVolume(name, val);
	}
	else
	{
		_TRACE("FMOD Audio - (SetBusVolume)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL bool DLLAudio::GetBusPaused(const char* name)
{
	bool isResult = false;
	if (GAudioEngine != nullptr)
	{
		isResult = GAudioEngine->GetBusPaused(name);
	}
	else
	{
		_TRACE("FMOD Audio - (GetBusPaused)Not Initialized AudioEngine");
	}
	return isResult;
}

_FMODAUDIODLL void DLLAudio::SetBusPaused(const char* name, bool val)
{
	if (GAudioEngine != nullptr)
	{
		GAudioEngine->SetBusPaused(name, val);
	}
	else
	{
		_TRACE("FMOD Audio - (SetBusPaused)Not Initialized AudioEngine");
	}
}

_FMODAUDIODLL float DLLAudio::GetEventDistance(const char* name)
{
	float value = 0;
	if (GAudioEngine != nullptr)
	{
		value = GAudioEngine->GetEventDistance(name);
	}
	else
	{
		_TRACE("FMOD Audio - (GetEventDistance)Not Initialized AudioEngine");
	}
	return value;
}

_FMODAUDIODLL bool DLLAudio::GetIs3D(const char* name)
{
	bool isResult = false;
	if (GAudioEngine != nullptr)
	{
		isResult = GAudioEngine->GetIs3D(name);
	}
	else
	{
		_TRACE("FMOD Audio - (GetIs3D)Not Initialized AudioEngine");
	}
	return isResult;
}
