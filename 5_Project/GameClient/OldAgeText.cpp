#include "pch.h"
#include "OldAgeText.h"
#include "Text.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameProcess.h"

OldAgeText::OldAgeText(shared_ptr<GameObject> gameObject) :MonoBehaviour(gameObject),
_tempText(gameObject->GetComponent<Text>()),
_color(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
{
}

OldAgeText::~OldAgeText()
{
}

void OldAgeText::Awake()
{
	_tempText->GetTextInfo()->color = _color;
	_tempText->GetTextInfo()->scale = 2.F;

#pragma region TEXT

	// 24
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " �ذ� �� �� �� ���ƿ�"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::DODO, " �Ҹ� �� �����ϰ� ���νÿ�.."));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " ����.. �ϳ� ����Ұ� �־��"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::DODO, " ���� �׷�"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " �� �� �츮 ó�� ���� ��"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " ��� ������ �ҿ��� �� �����"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " ���� ���� �־����"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " ���� ��ſ��� �ູ��"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " �־����� �𸣰ھ��"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " �� �Ƹ� ���� �� �� �����ſ���"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " ���ϰ� ������ ���� ����� �Ƴ���"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " �� �� �־ �ູ�߾��"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " �̾��ؿ� ����� �ΰ� ������.."));

	// 25
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " ���ƾƾƾƾƾ�!!"));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " �ȵ�.."));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " �ȵ�..."));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " �ȵ�...."));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " �̷� ���� ����"));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " ��...�� ���� ������°ž�"));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " END "));

	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ���� ����ϴ� ������"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ���� ���� ���� ����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ���� Ǫ���� �޵�"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ���� ����ϴ� �Ƴ�����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " �ᱹ ���� �� ���������ݾƾ�"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " �̷��� �ʹ� ���ǹ��Ѱ� �ƴϾ�?"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " �ʹ����� �㹫��..�̴�� ���� ���� ����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " �����ƿ� ����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " �� ���� �ູ�߾��"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " �׷���.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ����ϴ� �͵�"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " �״� �͵�	"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " �ΰ��� �Ƹ��ٿ��̿���"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " �ױ� ������"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ��Ű��� ����� �Ϸ簡 �ʹ����� �����߰�"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ��ġ�ִ� ������ �� �־����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " �̾���.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " �̾��� �� �ʿ� �����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ����ؿ�"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ....."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ����.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ���� �����"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ����,,�׵��� �Բ� ���༭ ����.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ....."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ....."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " (������ �긮��) ����.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ���߸��� ����� �����̾��"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ������ �ູ�߼�"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " END"));


#pragma endregion OldAge Vector

}

void OldAgeText::Update()
{
	if (_isPanelCheck)
	{
		switch (_selectText)
		{
		case 0:
		{
			_tempText->GetTextInfo()->text = _oldAgeText1[_nowText].second;
		}
		break;
		case 1:
		{
			_tempText->GetTextInfo()->text = _oldAgeText2[_nowText].second;
		}
		break;
		case 2:
		{
			_tempText->GetTextInfo()->text = _oldAgeText3[_nowText].second;
		}
		break;
		}
	}
}

int OldAgeText::ReturnTalker1(int nowtalker)
{
	if (_oldAgeText1.size() <= nowtalker)
		return 0;
	return _oldAgeText1[nowtalker].first;
}

int OldAgeText::ReturnTalker2(int nowtalker)
{
	if (_oldAgeText2.size() <= nowtalker)
		return 0;
	return _oldAgeText2[nowtalker].first;
}

int OldAgeText::ReturnTalker3(int nowtalker)
{
	if (_oldAgeText3.size() <= nowtalker)
		return 0;
	return _oldAgeText3[nowtalker].first;
}

//int OldAgeText::ReturnTalker4(int nowtalker)
//{
//	if (_oldAgeText4.size() <= nowtalker)
//		return 0;
//	return _oldAgeText4[nowtalker].first;
//}
