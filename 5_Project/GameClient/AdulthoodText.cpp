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
	_adulthoodBaseText1.push_back(make_pair((int)IDNUM::DODO, " �Ϸ����� ���� �ٶ���� �Ѵٴ�"));
	_adulthoodBaseText1.push_back(make_pair((int)IDNUM::DODO, " ���� �����κ��� �����ο��� �� ������?"));
	_adulthoodBaseText1.push_back(make_pair((int)IDNUM::DODO, " ���� ������ ��°� �ƴѵ�..."));
	
	// 9
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " ����.. � �� �ȹ��� ����?"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO,	 " �� ���� �������� �ؾ�����?"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " ���� ������"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " ���� ������ �ƹ��͵� �� ���� ����"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO,	 " ������ �߿��� �� ������?"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " ������"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �ְ���"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " �װͺ��� ������ ���� �ָ�"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �踦 ���ش� ����"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO,    " (���� �ǳ��ش�)"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::SEAGULL, " �ʵ� �� ���� �����ϰ� �ɰž�"));
	_adulthoodBaseText2.push_back(make_pair((int)IDNUM::DODO, " END PHASE "));

	// 10
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �ϳ� ����� �����ϰ� �ֱ�"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " �ʸ�ŭ �ٸ� ����� "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " �� ���� �����ϱ� ����"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ������ ������ ����"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::DODO,	 " �� ���� ���ǵ��� ��� ���ؼ� "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::DODO,	 " ��¿ �� ���ݾƿ�... "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ������ ���޾ұ�"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ����. �� ������ ���� �̱�� ������"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �شٸ� ���̾�"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::DODO,	 " ������, ���� ���� ���°ɿ�"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ������"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ���� ����� ���� ������ �ٲ� �� �־�"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " �� �÷��� �̰ܳ� �� �ִ� "));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " ���� ���� ���� �� �־�	"));
	_adulthoodBaseText3.push_back(make_pair((int)IDNUM::SEAGULL, " � ���� �ŷ��� �ϰھ�?"));
	
	// 11-1
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " ������! Ź���� �����̾�!"));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " �� ���� �޾�!"));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " ������ �̰� Ⱦ���߱���! "));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �� �� ū ���ڰ� �ɰž�.."));
	_adulthoodChoiceText1.push_back(make_pair((int)IDNUM::SEAGULL, " END"));

	// 11-2
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::DODO,	 " �ƴ�, �̰� �� �� ����"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " ���Ǵٰ�? "));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " �� ������ ���̸� ������ �� �� �ִµ�?"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::DODO,	 " ������ ������ ���� �� �����Ѱ�"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " � ���̿� ���� ���� �ϴ±�"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " �ΰ�� ������ ���ġ �����ϱ�"));
	_adulthoodChoiceText2.push_back(make_pair((int)IDNUM::SEAGULL, " END"));

	// 12
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �ϳ� ���� �ް� �ֱ�"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " �� �ҿ뵹�̴� �ʸ� ���� ��ų��ŭ"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " ������ ��Ȥ�� �������"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " �ѹ� ���������� ���� ���� ����"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " ������, ���� ���� ������ �ȸ�"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " �̷� ���浵 ���� �̰ܳ� �� ����"));
	_adulthoodBaseText4.push_back(make_pair((int)IDNUM::SEAGULL, " �? ���� �ŷ��� �ϰھ�?"));
	
	// 13-1
	_adulthoodChoiceText3.push_back(make_pair((int)IDNUM::DODO,	 " �ƴ�, �̰� �� �� ����"));
	_adulthoodChoiceText3.push_back(make_pair((int)IDNUM::SEAGULL, " �׷� �󸶳� ��ƿ �� �ֳ� ���ڰ�"));
	_adulthoodChoiceText3.push_back(make_pair((int)IDNUM::SEAGULL, " End "));

	// 13-2
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::DODO,	 " �˾Ҿ�. ���� �ȵǰھ�.."));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " ������! Ź���� �����̾�!"));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " �� ���� �޾�!"));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " ������ �̰� Ⱦ���߱��� "));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �� �� ū ���ڰ� �ɰž�.."));
	_adulthoodChoiceText4.push_back(make_pair((int)IDNUM::SEAGULL, " End "));
	
	// 14
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " ������ �� ���� ���� �پ���� �� ���ڱ���"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " �̰� ���� �Ѱ��ΰž�"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " ������� ��� �������� �Ѱ踦 ������ �־�"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " �̷� ���� �Ѿ�߸� �� �����Ŷ� �����ϴ�?"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " �� ��!"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " ������, ���� ���� ������ �Ǵٸ�"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " �̷� ���浵 ���� ���� �̰ܳ� �� ����"));
	_adulthoodBaseText5.push_back(make_pair((int)IDNUM::SEAGULL, " �? ���� �ŷ��� �ϰھ�?"));

	// 15-1
	_adulthoodChoiceText5.push_back(make_pair((int)IDNUM::DODO,	  " �ƴ�, �̰� �� �� ����?"));
	_adulthoodChoiceText5.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �����?"));
	
	// 15-2
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::DODO,	  " �˾Ҿ�. ���� �ȵǰھ�.."));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " ������! Ź���� �����̾�!"));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " �� ���� �޾�!"));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " ������ �̰� Ⱦ���߱��� "));
	_adulthoodChoiceText6.push_back(make_pair((int)IDNUM::SEAGULL, " ���� �� �� ū ���ڰ� �ɰž�.."));

	// 16
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::SEAGULL, " ũũŪ"));
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::SEAGULL, " �̹����� ����.."));
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::DODO, " ..."));
	_adulthoodBaseText6.push_back(make_pair((int)IDNUM::DODO, " ������ �ƹ��͵� ������ �� ������"));
	
	 // 17-1 BAD Ending
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " ���ش� �?"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::DODO,	 " ���� ������ ������ �����ϰ� �ǿ�"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " �׷���"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " �Ѷ��� ���� �踦 ź ���ػ翴��?"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " ������ �븦 ���� ������ ���ư���"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " �ٸ� ���� ������ �ؼ� "));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " ��Ƴ��� ���� �ö󰡰�"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " ������ �����ع�����"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " �׸��� �̰��� ������Ⱦ�.."));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " �� �����ڵ��� �̹� �ٵ� �� �����Ĺ�����"));
	_adulthoodChoiceText7.push_back(make_pair((int)IDNUM::Turtle, " END "));

	// 18-1 BAD Ending
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " ��� ���� ���ھ�?"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " �츮�� ���� �� ��ü�ε�"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " ���ڴٰ� ����������"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " ��� ���� ��� �ӿ����� ���� �� �־�"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " �� ��� �� ������ ���鼭"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " ���� �㸶�� ��� ��ȸ�ϰ� �־�"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " ���ظ� ������ �ʰ� ���ư������.."));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " ����ٰ� ���߿� �������� �ʾҴ����"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " �����ϸ� �ű⼭ ��������� ��"));
	_adulthoodChoiceText8.push_back(make_pair((int)IDNUM::Turtle, " END"));

	// 19-1 BAD Ending
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " ó���� ������ ���� ���� �����߾�"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " ������"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " �� �����ڵ��� ���� ����ġ�鼭"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " �ڽ��� �λ��� ��� "));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " ��ǥ�� �̷�� �� ���鼭"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " �� ������ ���� �Ҿ������⸸ ��"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " �̹� �� ���� �����̾������� ��԰� ������"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::DODO,	  "   .."));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " �ʵ� ���� ����� �� ����"));
	_adulthoodChoiceText9.push_back(make_pair((int)IDNUM::Turtle, " END"));

	// 17-2 TRUE Ending
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " ���ش� �?"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " ���� ������ ��Ⱑ �ʹ� ������"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " �׷���.."));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " ������ �ְŵ�"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " �ǰ��ϰ� ǳ���� �Ļ絵"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " ��� ���� �ʿ�����"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " �װ͵��� �տ� �������μ�"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " ������� �����ϸ鼭 �����Ӱ� �� �� �־�"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " ��� ���� ���ھ�?"));
	_adulthoodChoiceText10.push_back(make_pair((int)IDNUM::Turtle, " END"));

	// 18-2
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " ���� ������� ���� �����ϱ� ����"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " �� �뿹�� �Ǿ������"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " ���� ���� ����"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " ���� ��°� �ð��� ����ϸ鼭"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " �λ��� �Һ�����"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " ������."));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " �λ��̶� ���� ������ "));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " ���� ���� ���� �־��� �ð��ϸ� ���ݾ�?"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " �� �׷�?"));
	_adulthoodChoiceText11.push_back(make_pair((int)IDNUM::Turtle, " END "));

	// 19-2
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " ������ ��ƿ� �ʸ� ������"));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " ���� �������� ���� ��"));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " ����� ��Ⱑ �ʿ��� ���̾���"));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " �������� ���ؿ��� ���� �ູ�� ã��.."));
	_adulthoodChoiceText12.push_back(make_pair((int)IDNUM::Turtle, " END "));

	// 20
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, " �ʴ� ����� ����� ��������"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, " ����ְ� ��ƿԱ���"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, " �ʿ��� �Ѱ��� �ҿ��� �������.."));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " ...."));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " ���� ���� �������� �������"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " �ƹ��� ���� �赵 ������ ������ �� �����"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, "            ��..���� "));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::DODO2, " ��ġ�� �ű� �� ���� �ູ�� ���մϴ�"));
	_adulthoodBaseText7.push_back(make_pair((int)IDNUM::Star, "          �˾Ҵ�.."));

	// 21
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " ��..�����"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " �ϴÿ��� ū ���� �������� �Ծ��"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " Ȥ�� ��ġ�� �ʾҾ��?"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " ��..�� �������ϴ�"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " �������� ����?"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " ��..��������!"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " �Ͼ�..�����̴�"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::Lucy, " ���� ��ö�� �ؿ�"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " ���� ������� �մϴ�"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " Ȥ��.. �������� ���� �����ص� �ɱ��?"));
	_adulthoodBaseText8.push_back(make_pair((int)IDNUM::DODO, " ��������!"));

	// 22
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::Lucy, " ���� �뷡�Ҹ��׿�"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::DODO, " ���� �Ƹ��ٿ���"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::DODO, " (������ ������)"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::Lucy, " (�ͼ�ġ ���� �����̴�)?"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::DODO, " (� ����ϱ�?)?"));
	_adulthoodBaseText9.push_back(make_pair((int)IDNUM::Lucy, " (�� �˾ư��� �ʹ�)"));

	// 23
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " �������� �׷�����?"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " ��..�׷��� ���� ���� �о����� ������!"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " ����������"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " ...."));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (�����ϴ� å���� ����)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (������ �븦 ���� ���� �������� �Ȱ���)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (���� �� �����ְ� �ʹ�)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (���ϸ��� ������ �Բ� ����� �ʹ�)?"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (�̰� �� ����ΰɱ�?)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (���� �� ����� �θ��� �ʹ�)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (Ư������ �ʴ��� �Բ� �ð��� ������ �ʹ�)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::DODO, " (����ó���� ������ �ʹ�)"));
	_adulthoodBaseText10.push_back(make_pair((int)IDNUM::Lucy, " (�ɴٹ��� ���� �ɴٹ� �ް� ���� ������)"));
	

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

//---------------------------------------------- �б� ���
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
