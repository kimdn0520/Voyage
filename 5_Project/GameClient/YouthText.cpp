#include "pch.h"
#include "YouthText.h"
#include "Text.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameProcess.h"

YouthText::YouthText(shared_ptr<GameObject> gameObject)
	:MonoBehaviour(gameObject),
	_tempText(gameObject->GetComponent<Text>()),
	_color(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
{

}

YouthText::~YouthText()
{}

void YouthText::Awake()
{
	_tempText->GetTextInfo()->color = _color;
	_tempText->GetTextInfo()->scale = 2.0f;

#pragma region TEXT
	// Infancy Text
	_youthText1.push_back(make_pair((int)IDNUM::MOM ,"�����̵� ó���� �����"));
	_youthText1.push_back(make_pair((int)IDNUM::FATHER, "�׷��� �Ϻ����� �ʾƵ� �ȴܴ�."));
	_youthText1.push_back(make_pair((int)IDNUM::DODO, "�ٽ� ����Կ�.."));

	_youthText2.push_back(make_pair((int)IDNUM::ETC, "(������) - ��ġ �� �巡��"));
	_youthText2.push_back(make_pair((int)IDNUM::MOM, "���߾�!"));
	_youthText2.push_back(make_pair((int)IDNUM::FATHER, "�׷���!"));
	
	_youthText3.push_back(make_pair((int)IDNUM::DODO, "���� �帣�� ��ü�� ����?"));
	_youthText3.push_back(make_pair((int)IDNUM::MOM, "���̶���"));
	_youthText3.push_back(make_pair((int)IDNUM::DODO, "���� ����?"));
	_youthText3.push_back(make_pair((int)IDNUM::FATHER, "��� �Ͽ� å���� ���� ������"));
	_youthText3.push_back(make_pair((int)IDNUM::MOM, "�� ���� �̷��� ���ص� ������"));
	_youthText3.push_back(make_pair((int)IDNUM::FATHER, "�ٸ�, ���� �۴ٸ� �� �Ͽ� å���� �ʿ���"));
	_youthText3.push_back(make_pair((int)IDNUM::ETC, "�� �������� ȹ���ϼ���"));
	_youthText3.push_back(make_pair((int)IDNUM::ETC, "END Phase"));

	_youthText4.push_back(make_pair((int)IDNUM::ETC, "    7����"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "������ Ȱ¦��� ���"));
	_youthText4.push_back(make_pair((int)IDNUM::DODO, "�� ȥ�ڼ��� ���� �� �������?"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "��! �� �س� �� �־�"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "����, �ڽ��� �������̳� �����Կ�"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "�����ϰ� ����������"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "������ �������ϸ� ���� ���� ���ư���"));
	_youthText4.push_back(make_pair((int)IDNUM::MOM, "����Ѵܴ� ���"));
	_youthText4.push_back(make_pair((int)IDNUM::DODO, "���� ����ؿ�"));
	_youthText4.push_back(make_pair((int)IDNUM::MOM, "�ǰ��� �������Ѵ�."));
	_youthText4.push_back(make_pair((int)IDNUM::ETC, "END Phase"));

#pragma endregion Youth
}

void YouthText::Update()
{
	if (_isPanelCheck)
	{
		switch (_selectText)
		{
		case 0:
		{
			_tempText->GetTextInfo()->text = _youthText1[_nowText].second;
		}
		break;
		case 1:
		{
			_tempText->GetTextInfo()->text = _youthText2[_nowText].second;
		}
		break;
		case 2:
		{
			_tempText->GetTextInfo()->text = _youthText3[_nowText].second;
		}
		break;
		case 3:
		{
			_tempText->GetTextInfo()->text = _youthText4[_nowText].second;
		}
		break;

		}
	}
}

int YouthText::ReturnTalker1(int nowtalker)
{
	if (_youthText1.size() <= nowtalker)
		return 0;

	return _youthText1[nowtalker].first;
}

int YouthText::ReturnTalker2(int nowtalker)
{
	if (_youthText2.size() <= nowtalker)
		return 0;

	return _youthText2[nowtalker].first;
}

int YouthText::ReturnTalker3(int nowtalker)
{
	if (_youthText3.size() <= nowtalker)
		return 0;

	return _youthText3[nowtalker].first;
}

int YouthText::ReturnTalker4(int nowtalker)
{
	if (_youthText4.size() <= nowtalker)
		return 0;

	return _youthText4[nowtalker].first;
}

