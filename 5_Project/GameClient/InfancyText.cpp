#include "pch.h"
#include "InfancyText.h"
#include "Text.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameProcess.h"

InfancyText::InfancyText(shared_ptr<GameObject> gameObject)
	:MonoBehaviour(gameObject),
	_tempText(gameObject->GetComponent<Text>()),
	_color(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
{
}

InfancyText::~InfancyText()
{
}

void InfancyText::Awake()
{
	_tempText->GetTextInfo()->color = _color;
	_tempText->GetTextInfo()->scale = 2.F;

#pragma region TEXT
	// Infancy Text
	_infancyText1.push_back(make_pair((int)IDNUM::MOM," ������ ������ �����ϳ׿�"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER," �� ���ø� ������ ���ھ�"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER," ������ ������ �ߵ� �ڴ±�"));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM," ..."));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM, " �� õ�� ���ƿ�"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER, " �׷���.. �츮���� �ʹ� ������ ���̾�"));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM, " ��������"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER, " ���ư�����"));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM, " ���콺�� Ŭ���� �̵��غ���."));
									  
	_infancyText2.push_back(make_pair((int)IDNUM::MOM," ��Ʈ�� ������ ü���� Ű����ִܴ�"));
	_infancyText2.push_back(make_pair((int)IDNUM::FATHER," �Ǹ��� ����� ���� �ʾƵ� ������."));
	_infancyText2.push_back(make_pair((int)IDNUM::MOM," �ǰ��ϰԸ� �ڶ��ַ�"));
	_infancyText2.push_back(make_pair((int)IDNUM::FATHER," ���� ����Ѵܴ� ���"));
									  
	_infancyText3.push_back(make_pair((int)IDNUM::FATHER," �̷�.. ���̰� �����ȱ�"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," �踦 ��� �ٽ� ����߰ھ��"));
	_infancyText3.push_back(make_pair((int)IDNUM::ETC," (Q,E�� �̿��Ͽ� �踦 �����ּ���)"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM ," ���� ���� �츮 �Ʊ�..."));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," �ߵ� �ܴ� �츮 �Ʊ�.."));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," ���� ���� �츮 �Ʊ�.."));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," �ߵ� �ܴ� �츮 �Ʊ�.."));
	_infancyText3.push_back(make_pair((int)IDNUM::FATHER," �ٽ� ������"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," ���� ����� �� ��Ҿ��"));
	_infancyText3.push_back(make_pair((int)IDNUM::FATHER," ����"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," �׻� ���� ���� �� ������ ���ڳ׿�"));
									  
	_infancyText4.push_back(make_pair((int)IDNUM::MOM," ���õ� �ϱ⸦ ���� �ǰ���?"));
	_infancyText4.push_back(make_pair((int)IDNUM::FATHER," ��, ���� �Ϸ�� �ٽ� ���� �����״ϱ�"));
	_infancyText4.push_back(make_pair((int)IDNUM::MOM," ���õ� ���� �Ϸ翴���"));
	_infancyText4.push_back(make_pair((int)IDNUM::FATHER, " ��Ű� ���� �����̾�..."));
	_infancyText4.push_back(make_pair((int)IDNUM::MOM, " END "));

#pragma endregion Infancy Vector
}

void InfancyText::Update()
{
	if (_isPanelCheck)
	{
		switch (_selectText)
		{
		case 0:
		{
			_tempText->GetTextInfo()->text = _infancyText1[_nowText].second;
		}
		break;
		case 1:
		{
			_tempText->GetTextInfo()->text = _infancyText2[_nowText].second;
		}
		break;
		case 2:
		{
			_tempText->GetTextInfo()->text = _infancyText3[_nowText].second;
		}
		break;
		case 3:
		{
			_tempText->GetTextInfo()->text = _infancyText4[_nowText].second;
		}
		break;

		}
	}
}

int InfancyText::ReturnTalker1(int nowtalker)
{
	if (_infancyText1.size() <= nowtalker)
		return 0;
	return _infancyText1[nowtalker].first;
}

int InfancyText::ReturnTalker2(int nowtalker)
{
	if (_infancyText2.size() <= nowtalker)
		return 0;
	return _infancyText2[nowtalker].first;
}

int InfancyText::ReturnTalker3(int nowtalker)
{
	if (_infancyText3.size() <= nowtalker)
		return 0;
	return _infancyText3[nowtalker].first;
}

int InfancyText::ReturnTalker4(int nowtalker)
{
	if (_infancyText4.size() <= nowtalker)
		return 0;
	return _infancyText4[nowtalker].first;
}

