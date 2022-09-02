#pragma once

#include "NewbieEngineDLL.h"
#include "Component.h"


class Camera;
class GameObject;

struct AudioListener_Save
{
	bool				m_bEnable;
	unsigned int		m_ComponentId;
	unsigned int		m_ListenerCamera;
};

class AudioListener : public Component
{
public :

	NewbieEngine_DLL AudioListener(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~AudioListener();

public:
	virtual void Update() override;
	NewbieEngine_DLL void SetCameraTransform(std::shared_ptr<Camera> camera);
	void SetCameraObj(shared_ptr<GameObject> cam) { _ListenerCamera = cam; }

private:
	shared_ptr<GameObject>  _ListenerCamera;
};

