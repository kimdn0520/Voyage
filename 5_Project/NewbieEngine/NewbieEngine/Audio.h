#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"

//struct Audio_Save;

//struct Audio_Save
//{
//	bool				m_bEnable;
//	unsigned int		m_ComponentId;
//};

class SoundEvent;
class Transform;

class Audio : public Component
{
public :

	NewbieEngine_DLL Audio(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Audio();

public : 
	virtual void Update() override;

	NewbieEngine_DLL SoundEvent* PlayEvent(const char* path, bool duplicatePlay = true);
	NewbieEngine_DLL SoundEvent* PlayEvent(const char* path, const char* parameter, float parameterValue, bool duplicatePlay = true);
	NewbieEngine_DLL float GetEventDistance(const char* path);
	NewbieEngine_DLL void AllEventStop(bool fadeout = true);
private:
	bool GetIsAlreadyPlaying(const char* path);

	std::vector<SoundEvent*> _3DEvent_V;
	std::vector<SoundEvent*> _2DEvent_V;

	shared_ptr<Transform> _transform;
	
};

