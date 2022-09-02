#pragma once
#include "AudioDLLDefine.h"

class AudioSystem;

namespace DirextX
{
	struct XMMATRIX;
	struct XMFLOAT3;

	namespace SimpleMath
	{
		struct Vector3;
		struct Matrix;
	}
}

namespace FMOD
{
	namespace Studio { class EventInstance; }
}


class SoundEvent
{
public:
	SoundEvent();
	SoundEvent(AudioSystem* system, unsigned int id, const std::string path);
	~SoundEvent();


public:
	_FMODAUDIODLL bool IsValid();
	
	// �̺�Ʈ�� ó������ �����
	_FMODAUDIODLL void Restart();
	
	// �̺�Ʈ�� �����Ѵ�
	_FMODAUDIODLL void Stop(bool allowFadeOut = true);
	// Setters
	_FMODAUDIODLL void SetPaused(bool pause);
	_FMODAUDIODLL void SetVolume(float value);
	_FMODAUDIODLL void SetPitch(float value);
	_FMODAUDIODLL void SetParameter(const char* name, float value);

	// Getters
	_FMODAUDIODLL bool GetPaused() const;
	_FMODAUDIODLL float GetVolume() const;
	_FMODAUDIODLL float GetPitch() const;
	_FMODAUDIODLL float GetParameter(const char* name);
	_FMODAUDIODLL float GetEventDistance();
	
	// ��ġ ��� �Լ�
	_FMODAUDIODLL bool Is3D() const;
	_FMODAUDIODLL void Set3DAttributes(const DirectX::SimpleMath::Matrix& worldTrans, const DirectX::SimpleMath::Vector3& velocity = { 0.0f,0.0f ,0.0f });

private:
	std:: shared_ptr<AudioSystem> pAudioSystem;
	const unsigned int _id;

public :
	const std::string path;
};

