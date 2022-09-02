#pragma once
#include "AudioDLLDefine.h"

class SoundEvent;

namespace DLLAudio 
{
	_FMODAUDIODLL bool Initialize(const char* bankPath, int maxChanelCnt = 512);
	_FMODAUDIODLL void Release();
	_FMODAUDIODLL void Set3DSetting(float dopplerScale, float gameScale);
	_FMODAUDIODLL bool LoadAllBank(const std::vector<std::string>& vec);
	_FMODAUDIODLL void Update();

	_FMODAUDIODLL SoundEvent* PlayEvent(const std::string& soundpath);
	_FMODAUDIODLL void SetListener(const DirectX::SimpleMath::Matrix& veiewMatrix, const DirectX::SimpleMath::Vector3& velocity);

	_FMODAUDIODLL void SetMasterVolume(float val);
	_FMODAUDIODLL void SetAllBusVolume(float val);
	_FMODAUDIODLL float GetBusVolume(const char* name);
	_FMODAUDIODLL void SetBusVolume(const char* name, float val);
	_FMODAUDIODLL bool GetBusPaused(const char* name);
	_FMODAUDIODLL void SetBusPaused(const char* name, bool val);
	_FMODAUDIODLL float GetEventDistance(const char* name);
	_FMODAUDIODLL bool GetIs3D(const char* name);
}

