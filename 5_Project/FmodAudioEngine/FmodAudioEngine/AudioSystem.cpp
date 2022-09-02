#include "pch.h"
#include "AudioSystem.h"
#include "SoundEvent.h"
#include "SimpleMath.h"
#include "FmodHelper.h"

AudioSystem::AudioSystem() : _masterVolume(0.8f)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize(int maxChanelCnt, const char* bankPath)
{
	FMOD::Studio::System* system = nullptr;
	FMOD::System* lowSystem = nullptr;

	if (!CheckFmodResult(FMOD::Studio::System::create(&system)))
	{
		exit(-1);
		return false;
	}

	system->getCoreSystem(&lowSystem);

	// 저레벨 fmodeh 이니셜 라이즈 가능
	if (!CheckFmodResult(system->initialize(maxChanelCnt, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0)))
	{
		exit(-1);
		return false;
	}

	pSystem.reset(system);
	pLowLvSystem.reset(lowSystem);

	// 문자열에서 슬래시가 빠져있다면 확인후 넣어줌
	std::string _bankPath(bankPath);
	if (_bankPath.back() != '/')
	{
		_bankPath = _bankPath + "/";
	}

	if (!LoadBank(_bankPath + "Master.bank"))
	{
		return false;
	}

	if (!LoadBank(_bankPath + "Master.strings.bank"))
	{
		return false;
	}


	return true;
}

void AudioSystem::Release()
{
	UnloadAllBank();
	if (pSystem != nullptr)
	{
		pSystem->release();
	}
}

bool AudioSystem::LoadAllBank(const std::vector<std::string>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (!LoadBank(vec[i]))
		{
			// return false;
		}
	}
	return true;
}

void AudioSystem::Update()
{
	//업데이트에서 정지된 이벤트를 찾는다.
	std::vector<ID> doneEventVal;

	for (auto& _iter : eventInstance_UM)
	{
		FMOD::Studio::EventInstance* _event(_iter.second);
		FMOD_STUDIO_PLAYBACK_STATE _state;

		_event->getPlaybackState(&_state);
		if (_state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			//이벤트를 해제하고 done id 맵에 추가한다.
			_event->release();
			doneEventVal.emplace_back(_iter.first);
		}
	}

	//완료된 이벤트 인스턴스를 맵에서 제거
	for (auto _id : doneEventVal)
	{
		eventInstance_UM.erase(_id);
	}

	/// <summary>
	/// FMOD Update
	/// </summary>
	pSystem->update();
}

void AudioSystem::Set3DSetting(float dopplerScale, float gameScale, float rollOffScale)
{
	// 1=정상, 1보다 크면 과장된 소리를 낸다.
	// 게임 단위의 크기 = 1m // 도플러와 관계없음.
	pLowLvSystem->set3DSettings(dopplerScale, gameScale, rollOffScale);
}

void AudioSystem::SetListener(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up, const FMOD_VECTOR& velocity)
{
	FMOD_3D_ATTRIBUTES listener;

	listener.position = position;
	listener.forward = forward;
	listener.up = up;
	listener.velocity = velocity;

	//리스너가 한명이면 0값을 넣는다.
	CheckFmodResult(pSystem->setListenerAttributes(0, &listener));
}

void AudioSystem::SetListener(const DirectX::SimpleMath::Matrix& veiewMatrix, const DirectX::SimpleMath::Vector3& velocity)
{
	DirectX::SimpleMath::Matrix invertViewMatrix = veiewMatrix.Invert();

	DirectX::SimpleMath::Vector3 a = invertViewMatrix.Translation();
	SetListener(VectorToFMODVec(invertViewMatrix.Translation()),
		VectorToFMODVec(invertViewMatrix.Forward()),
		VectorToFMODVec(invertViewMatrix.Up()),
		VectorToFMODVec(velocity));
}

void AudioSystem::SetMasterVolume(float val)
{
	for (auto& bus : bus_UM)
	{
		float _volume = 0;
		float _masteredVolume = 0;
		bus.second->getVolume(&_masteredVolume);
		_volume = _masteredVolume / _masterVolume * val;
		bus.second->setVolume(_volume);
	}

	_masterVolume = val;
}

float AudioSystem::GetBusVolume(const PATH& name) const
{
	float value = 0;

	auto bus = bus_UM.find(name) != bus_UM.end() ? bus_UM.find(name)->second : nullptr;

	if (bus != nullptr)
	{
		bus->getVolume(&value);
	}

	return value;
}

bool AudioSystem::GetBusPaused(const PATH& name) const
{

	bool isvalue = false;

	auto bus = bus_UM.find(name) != bus_UM.end() ? bus_UM.find(name)->second : nullptr;

	if (bus != nullptr)
	{
		bus->getPaused(&isvalue);
	}

	return isvalue;
}

void AudioSystem::SetAllBusVolume(float val)
{
	for (auto& bus : bus_UM)
	{
		bus.second->setVolume(val);
	}
}

void AudioSystem::SetBusVolume(const PATH& name, float val)
{
	auto bus = bus_UM.find(name) != bus_UM.end() ? bus_UM.find(name)->second : nullptr;

	if (bus != nullptr)
	{
		bus->setVolume(val);
	}
}

void AudioSystem::SetBusPaused(const PATH& name, bool val)
{
	auto bus = bus_UM.find(name) != bus_UM.end() ? bus_UM.find(name)->second : nullptr;

	if (bus != nullptr)
	{
		bus->setPaused(val);
	}

}

float AudioSystem::GetEventDistance(const PATH& name)
{
	float min = 0;
	float max = 0;

	GetEventDescription(name)->getMinMaxDistance(&min, &max);

	return max;
}

bool AudioSystem::GetIs3D(const PATH& name)
{
	bool isvalue = false;
	GetEventDescription(name)->is3D(&isvalue);
	return isvalue;
}

SoundEvent* AudioSystem::PlayEvent(const PATH& name)
{
	static ID s_ID = 0;
	ID _retId = 0;

	auto iter = event_UM.find(name);
	if (iter != event_UM.end())
	{
		//이벤트 인스턴스 생성
		FMOD::Studio::EventInstance* _event = nullptr;
		iter->second->createInstance(&_event);
		if (_event)
		{
			_event->start();
			s_ID++;
			_retId = s_ID;
			eventInstance_UM.emplace(_retId, _event);
		}
		return new SoundEvent(this, _retId, name);
	}
	else
	{
		_TRACE("FMOD Audio - (PlayEvent)No Data %s", name);
		return nullptr;
	}
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(ID id)
{
	if (eventInstance_UM.find(id) != eventInstance_UM.end())
	{
		return eventInstance_UM.find(id)->second;
	}
	else
	{
		return nullptr;
	}
}

FMOD::Studio::EventDescription* AudioSystem::GetEventDescription(const PATH& name) const
{
	if (event_UM.find(name) != event_UM.end())
	{
		return event_UM.find(name)->second;
	}
	else
	{
		return nullptr;
	}
}

void AudioSystem::UnloadAllBank()
{
	for (auto& iter : bank_UM)
	{
		// Unload the sample data, then the bank itself
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	// No banks means no events
	bank_UM.clear();
	event_UM.clear();
	bus_UM.clear();
}

bool AudioSystem::CheckFmodResult(const FMOD_RESULT& result) const
{
	if (result != FMOD_OK)
	{
#ifdef _DEBUG || __TRACE
		_TRACE("FMOD Audio - ERROR (%d) %s\n", result, FMOD_ErrorString(result));
#endif
		return false;
	}
	return true;
}

bool AudioSystem::LoadBank(const std::string& name)
{
	//두번 로딩되지 않도록 한다.
	if (bank_UM.find(name) != bank_UM.end())
	{
#ifdef _DEBUG __TRACE
		_TRACE("FMOD Audio - Already Loaded(%s)", name.c_str());
#endif
		return true;
	}

	FMOD::Studio::Bank* _bank = nullptr;

	if (CheckFmodResult(pSystem->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &_bank)))
	{
		char _pathBuffer[512] = { 0, };

		//로드한 뱅크를 맵에 추가한다.
		bank_UM.emplace(name, _bank);
		//스트리밍 형식이 아닌 모든 샘플 데이터를 로드
		_bank->loadSampleData();
		//뱅크의 이벤트 수를 얻는다.
		int _eventCnt = 0;
		_bank->getEventCount(&_eventCnt);
		if (_eventCnt > 0)
		{
			//뱅크에서 이벤트 디스크립션 리스트를 얻는다
			std::vector<FMOD::Studio::EventDescription*> _event_V(_eventCnt);
			_bank->getEventList(_event_V.data(), _eventCnt, &_eventCnt);
			//얻은 리스트를 map에 삽입
			for (int i = 0; i < _eventCnt; i++)
			{
				_event_V[i]->getPath(_pathBuffer, 512, nullptr);
				//중복검사 : 중복되지 않는 것만 삽입한다.
				if (event_UM.find(std::string(_pathBuffer)) == event_UM.end())
				{
					event_UM.insert(std::make_pair(_pathBuffer, _event_V[i]));
				}
			}

		}

		//뱅크에서 버스 수를 얻는다.
		int _busCnt = 0;
		_bank->getBusCount(&_busCnt);
		if (_busCnt > 0)
		{
			//뱅크에서 버스 리스트를 얻는다
			std::vector<FMOD::Studio::Bus*> _bus_V(_busCnt);
			_bank->getBusList(_bus_V.data(), _busCnt, &_busCnt);
			//얻은 리스트를 map에 삽입
			for (int i = 0; i < _busCnt; i++)
			{
				_bus_V[i]->getPath(_pathBuffer, 254, nullptr);
				bus_UM.emplace(_pathBuffer, _bus_V[i]);
			}
		}
		return true;
	}

	return false;
}
