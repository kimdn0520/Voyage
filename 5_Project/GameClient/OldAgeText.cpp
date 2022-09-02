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
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 해가 곧 뜰 것 같아요"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::DODO, " 할멈 옷 따뜻하게 감싸시우.."));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 영감.. 하나 고백할게 있어요"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::DODO, " 뭔데 그려"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 그 때 우리 처음 만난 날"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 사실 별에게 소원을 빈 당신을"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 몰래 보고 있었어요"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 내가 당신에게 행복을"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 주었는지 모르겠어요"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 난 아마 오래 살 수 없을거예요"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 강하고 마음씨 착한 당신의 아내로"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 살 수 있어서 행복했어요"));
	_oldAgeText1.push_back(make_pair((int)IDNUM::Lucy, " 미안해요 당신을 두고 떠나서.."));

	// 25
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " 으아아아아아아!!"));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " 안돼.."));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " 안돼..."));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " 안돼...."));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " 이럴 수는 없어"));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " 왜...왜 모든게 사라지는거야"));
	_oldAgeText2.push_back(make_pair((int)IDNUM::DODO, " END "));

	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 내가 사랑하는 가족도"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 내가 벌어 놓은 돈도"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 나의 푸르른 꿈도"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 나의 사랑하는 아내마저"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 결국 모든건 다 끝나버리잖아야"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 이러면 너무 무의미한거 아니야?"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 너무나도 허무해..이대로 끝낼 수는 없어"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 괜찮아요 여보"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 난 정말 행복했어요"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 그래도.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 노쇠하는 것도"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 죽는 것도	"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 인간의 아름다움이예요"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 죽기 때문에"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 당신과의 평범한 하루가 너무나도 소중했고"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 가치있는 순간일 수 있었어요"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 미안해.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 미안해 할 필요 없어요"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 사랑해요"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " ....."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 나도.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 나도 사랑해"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " 여보,,그동안 함께 해줘서 고마워.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ....."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::Lucy, " ....."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " (눈물을 흘리며) 여보.."));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 나야말로 당신의 남편이었어서"));
	_oldAgeText3.push_back(make_pair((int)IDNUM::DODO, " 정말로 행복했소"));
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
