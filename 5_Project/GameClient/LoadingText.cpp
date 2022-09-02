#include "pch.h"
#include "LoadingText.h"
#include "Text.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameProcess.h"

LoadingText::LoadingText(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_text(gameObject->GetComponent<Text>()),
	_color(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
{}

LoadingText::~LoadingText()
{}

void LoadingText::Awake()
{
	_text->GetTextInfo()->color = _color;
	_text->GetTextInfo()->scale = 3.0f;		// �ؽ�Ʈ ũ������ �����ָ� default�� 1.0f ��
}

void LoadingText::Update()
{
	_text->GetTextInfo()->text = "[L o a d i n g]\n";
}
