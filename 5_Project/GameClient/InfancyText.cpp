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
	_infancyText1.push_back(make_pair((int)IDNUM::MOM," 오늘은 물살이 잔잔하네요"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER," 늘 오늘만 같으면 좋겠어"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER," 도도는 아직도 잘도 자는군"));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM," ..."));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM, " 꼭 천사 같아요"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER, " 그렇지.. 우리에겐 너무 소중한 아이야"));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM, " 기적이죠"));
	_infancyText1.push_back(make_pair((int)IDNUM::FATHER, " 나아가볼까"));
	_infancyText1.push_back(make_pair((int)IDNUM::MOM, " 마우스를 클릭해 이동해보렴."));
									  
	_infancyText2.push_back(make_pair((int)IDNUM::MOM," 하트을 먹으면 체력을 키울수있단다"));
	_infancyText2.push_back(make_pair((int)IDNUM::FATHER," 훌륭한 사람이 되지 않아도 괜찮아."));
	_infancyText2.push_back(make_pair((int)IDNUM::MOM," 건강하게만 자라주렴"));
	_infancyText2.push_back(make_pair((int)IDNUM::FATHER," 정말 사랑한단다 얘야"));
									  
	_infancyText3.push_back(make_pair((int)IDNUM::FATHER," 이런.. 아이가 깨버렸군"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," 배를 흔들어서 다시 재워야겠어요"));
	_infancyText3.push_back(make_pair((int)IDNUM::ETC," (Q,E를 이용하여 배를 흔들어주세요)"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM ," 자장 자장 우리 아기..."));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," 잘도 잔다 우리 아기.."));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," 자장 자장 우리 아기.."));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," 잘도 잔다 우리 아기.."));
	_infancyText3.push_back(make_pair((int)IDNUM::FATHER," 다시 잠들었어"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," 정말 당신을 꼭 닮았어요"));
	_infancyText3.push_back(make_pair((int)IDNUM::FATHER," 고마워"));
	_infancyText3.push_back(make_pair((int)IDNUM::MOM," 항상 같이 있을 수 있으면 좋겠네요"));
									  
	_infancyText4.push_back(make_pair((int)IDNUM::MOM," 오늘도 일기를 띄우는 건가요?"));
	_infancyText4.push_back(make_pair((int)IDNUM::FATHER," 응, 오늘 하루는 다시 오지 않을테니까"));
	_infancyText4.push_back(make_pair((int)IDNUM::MOM," 오늘도 좋은 하루였어요"));
	_infancyText4.push_back(make_pair((int)IDNUM::FATHER, " 당신과 아이 덕분이야..."));
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

