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
	// ��ġ ����
	prefab->GetComponent<Transform>()->SetLocalPosition(pos);

	// ���� ����
	SceneManager::GetInstance()->SetInstantiateGameObject(prefab);

	// ���� �� �����տ� �ڽĿ�����Ʈ�� ����ִٸ� ���� ������ ���� �ɾ��ش�.
	// �̷����ϸ� �ڽ��� �ڽĿ�����Ʈ�� ������ �װ͵� �ȴ�.
	// �ٵ� ���� AddGameObject���� �ڽı��� �������ش�.
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

	// ���� �ڽĿ�����Ʈ�� �ִٸ� �ڽĿ�����Ʈ�� ���� ������ �� ������ �ɾ��ش�.
	// �̷����ϸ� �ڽ��� �ڽĿ�����Ʈ�� ������ �װ͵� �ȴ�.
	if (!gameObject->GetChilds().empty())
	{
		for (int i = 0; i < gameObject->GetChilds().size(); i++)
		{
			Destroy(gameObject->GetChilds()[i]);
		}
	}
}