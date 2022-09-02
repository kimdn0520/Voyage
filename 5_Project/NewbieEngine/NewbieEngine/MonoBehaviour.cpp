#include "pch.h"
#include "MonoBehaviour.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

MonoBehaviour::MonoBehaviour(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::MONO_BEHAVIOUR)
{}

MonoBehaviour::~MonoBehaviour()
{}

void MonoBehaviour::Instantiate(shared_ptr<GameObject> prefab, Vector3 pos)
{
	// 위치 설정
	prefab->GetComponent<Transform>()->SetLocalPosition(pos);

	// 생성 예약
	SceneManager::GetInstance()->SetInstantiateGameObject(prefab);

	// 만약 그 프리팹에 자식오브젝트가 들어있다면 생성 예약을 같이 걸어준다.
	// 이렇게하면 자식의 자식오브젝트가 있으면 그것도 된다.
	// 근데 지금 AddGameObject에서 자식까지 생성해준다.
	/*if (!prefab->GetChilds().empty())
	{
		for (int i = 0; i < prefab->GetChilds().size(); i++)
		{
			Instantiate(prefab->GetChilds()[i], prefab->GetChilds()[i]->GetComponent<Transform>()->GetLocalPosition());
		}
	}*/
}

void MonoBehaviour::Destroy(shared_ptr<GameObject> gameObject)
{
	SceneManager::GetInstance()->SetRemoveGameObject(gameObject);

	// 만약 자식오브젝트가 있다면 자식오브젝트도 삭제 예약을 그 다음에 걸어준다.
	// 이렇게하면 자식의 자식오브젝트가 있으면 그것도 된다.
	if (!gameObject->GetChilds().empty())
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			Destroy(gameObject->GetChilds()[i]);
		}
	}
}