#include "pch.h"
#include "AdulthoodText.h"
#include "Text.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GameProcess.h"

AdulthoodText::AdulthoodText(shared_ptr<GameObject> gameObject)
	:MonoBehaviour(gameObject),
	_tempText(gameObject->GetComponent<Text>()),
	_color(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
{

}

AdulthoodText::~AdulthoodText()
{

}

void AdulthoodText::Awake()
{
	_tempText->GetTextInfo()->color = _color;
	_tempText->GetTextInfo()->scale = 2.F;

#pragma region TEXT
	
	// 8
	_adulthoodBaseText1.push_back(make_pair((int)IDNUM::DODO, " 하루종일 돈만 바라봐야 한다니"));
	_adulthoodBaseText1.push_back(make_pair((int)IDNUM::DODO, " 언제 돈으로부터 자유로워질 수 있을까?"));
	_adulthoodBaseText1.push_back(make_pair((int)IDNUM::DODO, " 돈을 버려고 사는게 아닌데..."));
	
	// 9
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 어이.. 어서 돈 안벌고 뭐해?"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO,	 " 이 일을 언제까지 해야하죠?"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 죽을 때까지"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 돈이 없으면 아무것도 할 수가 없어"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO,	 " 돈보다 중요한 건 없나요?"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 하하하"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 돈이 최고지"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 그것보다 나한테 돈을 주면"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 멋진 배를 구해다 주지"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO,    " (돈을 건네준다)"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " 너도 곧 돈을 좋아하게 될거야"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO, " END PHASE "));

	// 10
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 하하 꽤나 힘들게 경쟁하고 있군"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 너만큼 다른 사람도 "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 더 많이 소유하기 위해"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 끝없이 경쟁을 하지"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::DODO,	 " 더 좋은 물건들을 얻기 위해선 "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::DODO,	 " 어쩔 수 없잖아요... "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 이제야 깨달았군"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 좋아. 이 경쟁을 쉽게 이기게 해주지"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 돈을 준다면 말이야"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::DODO,	 " 하지만, 나는 돈이 없는걸요"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 괜찮아"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 너의 어린시절 꿈을 돈으로 바꿀 수 있어"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 이 시련을 이겨낼 수 있는 "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 힘도 쉽게 얻을 수 있어	"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " 어때 나와 거래를 하겠어?"));
	
	// 11-1
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " 하하핫! 탁월한 선택이야!"));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " 이 약을 받아!"));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " 낄낄낄 이거 횡재했구만! "));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " 이제 난 더 큰 부자가 될거야.."));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " END"));

	// 11-2
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::DODO,	 " 아니, 이건 팔 수 없어"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " 안판다고? "));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " 이 세상은 돈이면 뭐든지 할 수 있는데?"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::DODO,	 " 나에겐 돈보다 꿈이 더 소중한걸"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " 어린 아이와 같은 말을 하는군"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " 두고봐 현실은 녹록치 않으니까"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " END"));

	// 12
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 하하 꽤나 고난을 겪고 있군"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 그 소용돌이는 너를 집어 삼킬만큼"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 위험한 유혹과 고뇌이지"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 한번 빠져버리면 헤어나올 수도 없어"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 하지만, 너의 꿈을 나에게 팔면"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 이런 역경도 쉽게 이겨낼 수 있지"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " 어때? 나와 거래를 하겠어?"));
	
	// 13-1
	_adulthoodChoiceText3.push_back(make_pair((int)IDNUM::DODO,	 " 아니, 이건 팔 수 없어"));
	_adulthoodChoiceText3.push_back(make_pair((int)IDNUM::SEAGULL, " 그래 얼마나 버틸 수 있나 보자고"));
	_adulthoodChoiceText3.push_back(make_pair((int)IDNUM::SEAGULL, " End "));

	// 13-2
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::DODO,	 " 알았어. 역시 안되겠어.."));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " 하하핫! 탁월한 선택이야!"));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " 이 약을 받아!"));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " 낄낄낄 이거 횡재했구만 "));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " 이제 난 더 큰 부자가 될거야.."));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " End "));
	
	// 14
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 하하하 이 벽은 절대 뛰어넘을 수 없겠구만"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 이게 너의 한계인거야"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 사람들은 모두 저마다의 한계를 가지고 있어"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 이런 벽을 넘어뜨릴 수 있을거라 생각하니?"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 꿈 깨!"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 하지만, 너의 꿈을 나에게 판다면"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 이런 역경도 아주 쉽게 이겨낼 수 있지"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " 어때? 나와 거래를 하겠어?"));

	// 15-1
	_adulthoodChoiceText5.push_back(make_pair((int)IDNUM::DODO,	  " 아니, 이건 팔 수 없어?"));
	_adulthoodChoiceText5.push_back(make_pair((int)IDNUM::SEAGULL, " 정말 어리석군?"));
	
	// 15-2
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::DODO,	  " 알았어. 역시 안되겠어.."));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " 하하핫! 탁월한 선택이야!"));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " 이 약을 받아!"));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " 낄낄낄 이거 횡재했구만 "));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " 이제 난 더 큰 부자가 될거야.."));

	// 16
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::SEAGULL, " 크크큭"));
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::SEAGULL, " 이번에야 말로.."));
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::DODO, " ..."));
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::DODO, " 죽으면 아무것도 가져갈 수 없구나"));
	
	 // 17-1 BAD Ending
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 항해는 어때?"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::DODO,	 " 꿈이 없으니 뭐든지 포기하게 되요"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 그렇군"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 한때는 나도 배를 탄 항해사였어?"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 열심히 노를 젓고 앞으로 나아가고"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 다른 배들과 경쟁을 해서 "));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 살아남기 위해 올라가고"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 하지만 포기해버렸지"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 그리곤 이곳에 멈춰버렸어.."));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " 내 경쟁자들은 이미 다들 날 지나쳐버렸지"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " END "));

	// 18-1 BAD Ending
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 잠깐 위를 보겠어?"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 우리는 별이 그 자체로도"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 예쁘다고 생각하지만"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 사실 별은 어둠 속에서만 빛날 수 있어"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 난 사실 이 별들을 보면서"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 매일 밤마다 계속 후회하고 있어"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 항해를 멈추지 않고 나아갔더라면.."));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 힘들다고 도중에 포기하지 않았더라면"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " 포기하면 거기서 멈춰버리게 돼"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " END"));

	// 19-1 BAD Ending
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 처음에 포기할 때는 쉽게 생각했어"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 하지만"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 내 경쟁자들이 나를 지나치면서"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 자신의 인생을 살고 "));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 목표를 이루는 걸 보면서"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 내 마음은 점점 불안해지기만 해"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 이미 내 꿈이 무엇이었는지도 까먹고 말았지"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::DODO,	  "   .."));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " 너도 나와 비슷한 것 같군"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " END"));

	// 17-2 TRUE Ending
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 항해는 어때?"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 돈이 없으니 살기가 너무 힘들어요"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 그렇군.."));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 쾌적한 주거도"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 건강하고 풍성한 식사도"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 모두 돈이 필요하지"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 그것들을 손에 넣음으로서"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 사람들은 독립하면서 자유롭게 살 수 있어"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " 잠깐 위를 보겠어?"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " END"));

	// 18-2
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 많은 사람들은 돈을 소유하기 위해"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 돈 노예가 되어버리지"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 돈을 벌기 위해"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 많은 노력과 시간을 희생하면서"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 인생을 소비하지"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 하지만."));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 인생이란 것이 끝없이 "));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 돈을 벌기 위해 주어진 시간일리 없잖아?"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " 안 그래?"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " END "));

	// 19-2
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " 열심히 살아온 너를 응원해"));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " 꿈을 포기하지 않은 건"));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " 상당한 용기가 필요한 일이었어"));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " 앞으로의 항해에서 너의 행복을 찾길.."));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " END "));

	// 20
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, " 너는 위험과 곤란을 무릎쓰며"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, " 용기있게 살아왔구나"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, " 너에게 한가지 소원을 들어주지.."));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " ...."));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " 어마어마한 돈도 언젠가는 사라져요"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " 아무리 좋은 배도 죽으면 가져갈 수 없어요"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, "            저..저는 "));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " 가치를 매길 수 없는 행복을 원합니다"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, "          알았다.."));

	// 21
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " 저..저기요"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " 하늘에서 큰 별이 떨어져서 왔어요"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " 혹시 다치진 않았어요?"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " 아..네 괜찮습니다"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " 괜찮은거 맞죠?"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " 무..물론이죠!"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " 하아..다행이다"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " 저는 루시라고 해요"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " 저는 도도라고 합니다"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " 혹시.. 괜찮으면 같이 항해해도 될까요?"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " 물론이죠!"));

	// 22
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::Lucy, " 좋은 노래소리네요"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::DODO, " 정말 아름다워요"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::DODO, " (마음이 떨린다)"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::Lucy, " (익숙치 않은 떨림이다)?"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::DODO, " (어떤 사람일까?)?"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::Lucy, " (더 알아가고 싶다)"));

	// 23
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " 아하하하 그래서요?"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " 하..그래서 제가 힘껏 밀어줬지 뭐예요!"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " 아하하하하"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " ...."));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (좋아하는 책부터 음식)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (심지어 노를 젓는 작은 습관까지 똑같다)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (조금 더 같이있고 싶다)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (매일매일 웃으며 함께 떠들고 싶다)?"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (이건 내 욕심인걸까?)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (조금 더 욕심을 부리고 싶다)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (특별하지 않더라도 함께 시간을 보내고 싶다)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (지금처럼만 보내고 싶다)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (꽃다발을 전달 꽃다발 받고 고개를 끄덕임)"));
	

#pragma endregion Adulthood Vector

}

void AdulthoodText::Update()
{
	if (_isPanelCheck)
	{
		switch (_selectText)
		{
		case 0:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText1[_nowText].second;
		}
		break;
		case 1:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText2[_nowText].second;
		}
		break;
		case 2:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText3[_nowText].second;
		}
		break;
		case 3:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText4[_nowText].second;
		}
		break;
		case 4:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText5[_nowText].second;
		}
		break;
		case 5:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText6[_nowText].second;
		}
		break;
		case 6:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText7[_nowText].second;
		}
		break;
		case 7:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText9[_nowText].second;
		}
		break;
		case 8:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText10[_nowText].second;
		}
		break;
		case 9:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText1[_nowText].second;
		}
		break;
		case 10:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText2[_nowText].second;
		}
		break;
		case 11:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText3[_nowText].second;
		}
		break;
		case 12:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText4[_nowText].second;
		}
		break;
		case 13:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText5[_nowText].second;
		}
		break;
		case 14:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText6[_nowText].second;
		}
		break;
		case 15:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText7[_nowText].second;
		}
		break;
		case 16:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText8[_nowText].second;
		}
		break;
		case 17:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText9[_nowText].second;
		}
		break;
		case 18:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText10[_nowText].second;
		}
		break;
		case 19:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText11[_nowText].second;
		}
		break;
		case 20:
		{
			_tempText->GetTextInfo()->text = _adulthoodChoiceText12[_nowText].second;
		}
		break;
		case 21:
		{
			_tempText->GetTextInfo()->text = _adulthoodBaseText8[_nowText].second;
		}
		break;
		}
	}
}

int AdulthoodText::ReturnTalker1(int nowtalker)
{
	if (_adulthoodBaseText1.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText1[nowtalker].first;
}

int AdulthoodText::ReturnTalker2(int nowtalker)
{
	if (_adulthoodBaseText2.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText2[nowtalker].first;
}

int AdulthoodText::ReturnTalker3(int nowtalker)
{
	if (_adulthoodBaseText3.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText3[nowtalker].first;
}

int AdulthoodText::ReturnTalker4(int nowtalker)
{
	if (_adulthoodBaseText4.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText4[nowtalker].first;
}

int AdulthoodText::ReturnTalker5(int nowtalker)
{
	if (_adulthoodBaseText5.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText5[nowtalker].first;
}

int AdulthoodText::ReturnTalker6(int nowtalker)
{
	if (_adulthoodBaseText6.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText6[nowtalker].first;
}

int AdulthoodText::ReturnTalker7(int nowtalker)
{
	if (_adulthoodBaseText7.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText7[nowtalker].first;
}

int AdulthoodText::ReturnTalker8(int nowtalker)
{
	if (_adulthoodBaseText8.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText8[nowtalker].first;
}

int AdulthoodText::ReturnTalker9(int nowtalker)
{
	if (_adulthoodBaseText9.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText9[nowtalker].first;
}

//---------------------------------------------- 분기 대사
int AdulthoodText::ReturnTalker10(int nowtalker)
{
	if (_adulthoodBaseText10.size() <= nowtalker)
		return 0;

	return _adulthoodBaseText10[nowtalker].first;
}

int AdulthoodText::ReturnTalker11(int nowtalker)
{
	if (_adulthoodChoiceText1.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText1[nowtalker].first;
}

int AdulthoodText::ReturnTalker12(int nowtalker)
{
	if (_adulthoodChoiceText2.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText2[nowtalker].first;
}

int AdulthoodText::ReturnTalker13(int nowtalker)
{
	if (_adulthoodChoiceText3.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText3[nowtalker].first;
}

int AdulthoodText::ReturnTalker14(int nowtalker)
{
	if (_adulthoodChoiceText4.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText4[nowtalker].first;
}

int AdulthoodText::ReturnTalker15(int nowtalker)
{
	if (_adulthoodChoiceText5.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText5[nowtalker].first;
}

int AdulthoodText::ReturnTalker16(int nowtalker)
{
	if (_adulthoodChoiceText6.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText6[nowtalker].first;
}

int AdulthoodText::ReturnTalker17(int nowtalker)
{
	if (_adulthoodChoiceText7.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText7[nowtalker].first;
}

int AdulthoodText::ReturnTalker18(int nowtalker)
{
	if (_adulthoodChoiceText8.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText8[nowtalker].first;
}

int AdulthoodText::ReturnTalker19(int nowtalker)
{
	if (_adulthoodChoiceText9.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText9[nowtalker].first;
}

int AdulthoodText::ReturnTalker20(int nowtalker)
{
	if (_adulthoodChoiceText10.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText10[nowtalker].first;
}

int AdulthoodText::ReturnTalker21(int nowtalker)
{
	if (_adulthoodChoiceText11.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText11[nowtalker].first;
}

int AdulthoodText::ReturnTalker22(int nowtalker)
{
	if (_adulthoodChoiceText12.size() <= nowtalker)
		return 0;

	return _adulthoodChoiceText12[nowtalker].first;
}
