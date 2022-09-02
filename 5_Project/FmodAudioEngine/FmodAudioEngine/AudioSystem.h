#pragma once
#include "IAudioSystem.h"
#include "fmod.h"

class SoundEvent;

namespace FMOD
{
	class System;

	namespace Studio
	{
		class System;
		class Bank;
		class Bus;
		class EventDescription;
		class EventInstance;
	}

}

// Fmod�� ����ϴ� ����� ����
using PATH = std::string;
using ID = unsigned int;


class AudioSystem : public IAudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

public:
	virtual bool Initialize(int maxChanelCnt, const char* bankPath) override;
	virtual void Release() override;
	virtual bool LoadAllBank(const std::vector<std::string>& vec) override;
	virtual void Update() override;

	virtual void Set3DSetting(float dopplerScale, float gameScale, float rollOffScale = 1.0f) override;
	virtual void SetListener(const DirectX::SimpleMath::Matrix& veiewMatrix,const DirectX::SimpleMath::Vector3& velocity = DirectX::SimpleMath::Vector3{ 0.f, 0.f, 0.f }) override;	//�ӵ��� 0�̸� ���÷�ȿ���� ������� �ʴ´�.
	void SetListener(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up, const FMOD_VECTOR& velocity = { 0.f, 0.f, 0.f });	//�ӵ��� 0�̸� ���÷�ȿ���� ������� �ʴ´�.

	virtual void  SetMasterVolume(float val) override;
	virtual float GetBusVolume(const PATH& name) const override;
	virtual bool  GetBusPaused(const PATH& name) const override;
	virtual void  SetAllBusVolume(float val) override;
	virtual void  SetBusVolume(const PATH& name, float val)override;
	virtual void  SetBusPaused(const PATH& name, bool val)override;

	virtual float GetEventDistance(const PATH& name) override;
	virtual bool  GetIs3D(const PATH& name) override;
public:
	SoundEvent* PlayEvent(const PATH& name);

	FMOD::Studio::EventInstance* GetEventInstance(ID id);
	FMOD::Studio::EventDescription* GetEventDescription(const PATH& name) const;
private:
	void UnloadAllBank();
	bool CheckFmodResult(const FMOD_RESULT& result) const;
	bool LoadBank(const std::string& name);

	std::unique_ptr<FMOD::Studio::System> pSystem;			//�ַ� ����� �ý���
	std::unique_ptr<FMOD::System> pLowLvSystem;				//�����ؽý���

	std::unordered_map<PATH, FMOD::Studio::Bank*> bank_UM;
	std::unordered_map<PATH, FMOD::Studio::Bus*> bus_UM;		//������ �׷�ȭ

	std::unordered_map<PATH, FMOD::Studio::EventDescription*> event_UM;
	std::unordered_map<ID, FMOD::Studio::EventInstance*> eventInstance_UM;

	float _masterVolume;	//0 ~ 1.0f ������ ��

};

