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
	_youthText1.push_back(make_pair((int)IDNUM::MOM ,"무엇이든 처음은 힘들어"));
	_youthText1.push_back(make_pair((int)IDNUM::FATHER, "그러니 완벽하지 않아도 된단다."));
	_youthText1.push_back(make_pair((int)IDNUM::DODO, "다시 저어볼게요.."));

	_youthText2.push_back(make_pair((int)IDNUM::ETC, "(노젓기) - 터치 후 드래그"));
	_youthText2.push_back(make_pair((int)IDNUM::MOM, "잘했어!"));
	_youthText2.push_back(make_pair((int)IDNUM::FATHER, "그렇지!"));
	
	_youthText3.push_back(make_pair((int)IDNUM::DODO, "지금 흐르는 물체는 뭐죠?"));
	_youthText3.push_back(make_pair((int)IDNUM::MOM, "꿈이란다"));
	_youthText3.push_back(make_pair((int)IDNUM::DODO, "꿈이 뭐죠?"));
	_youthText3.push_back(make_pair((int)IDNUM::FATHER, "어떠한 일에 책임을 지는 것이지"));
	_youthText3.push_back(make_pair((int)IDNUM::MOM, "꼭 꿈을 이루지 못해도 괜찮아"));
	_youthText3.push_back(make_pair((int)IDNUM::FATHER, "다만, 꿈을 꾼다면 그 일에 책임이 필요해"));
	_youthText3.push_back(make_pair((int)IDNUM::ETC, "별 아이템을 획득하세요"));
	_youthText3.push_back(make_pair((int)IDNUM::ETC, "END Phase"));

	_youthText4.push_back(make_pair((int)IDNUM::ETC, "    7년후"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "가슴을 활짝펴고 살렴"));
	_youthText4.push_back(make_pair((int)IDNUM::DODO, "저 혼자서도 잘할 수 있을까요?"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "응! 잘 해낼 수 있어"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "설령, 자신의 나약함이나 무능함에"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "좌절하고 쓰러지더라도"));
	_youthText4.push_back(make_pair((int)IDNUM::FATHER, "마음을 소중히하며 앞을 향해 나아가렴"));
	_youthText4.push_back(make_pair((int)IDNUM::MOM, "사랑한단다 얘야"));
	_youthText4.push_back(make_pair((int)IDNUM::DODO, "저도 사랑해요"));
	_youthText4.push_back(make_pair((int)IDNUM::MOM, "건강히 지내야한다."));
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

