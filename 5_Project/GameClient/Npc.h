#pragma once

#include "MonoBehaviour.h"

class Transform;
class Animator;
class Audio;
class UI;

class Npc : public MonoBehaviour
{
public:

	Npc(shared_ptr<GameObject> gameObject);
	virtual ~Npc();

private:
	hash<string> hasher;

	shared_ptr<Transform> _transform;

	shared_ptr<GameObject> _gameObject;

	shared_ptr<GameObject> _mainCamera;

	shared_ptr<Audio> _audio;
	
	shared_ptr<UI> _ui;

	Vector3 targetPoint;

public:
	

};

