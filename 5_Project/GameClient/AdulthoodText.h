#pragma once
#include "MonoBehaviour.h"

class Text;

class AdulthoodText : public MonoBehaviour
{
public:
	AdulthoodText(shared_ptr<GameObject> gameObject);
	virtual ~AdulthoodText();

private:
	shared_ptr<Text> _tempText;


	// Adulthood Text
	// 선택지에 상관없이 나오는 애들
	std::vector < pair<int, std::string>> _adulthoodBaseText1;
	std::vector < pair<int, std::string>> _adulthoodBaseText2;
	std::vector < pair<int, std::string>> _adulthoodBaseText3;
	std::vector < pair<int, std::string>> _adulthoodBaseText4;
	std::vector < pair<int, std::string>> _adulthoodBaseText5;
	std::vector < pair<int, std::string>> _adulthoodBaseText6;
	std::vector < pair<int, std::string>> _adulthoodBaseText7;
	std::vector < pair<int, std::string>> _adulthoodBaseText8;
	std::vector < pair<int, std::string>> _adulthoodBaseText9;
	std::vector < pair<int, std::string>> _adulthoodBaseText10;
	

	// 갈매기에게 꿈을 판 경우 나오는 텍스트 선택 텍스트
	std::vector < pair<int, std::string>> _adulthoodChoiceText1;
	std::vector < pair<int, std::string>> _adulthoodChoiceText2;
	std::vector < pair<int, std::string>> _adulthoodChoiceText3;
	std::vector < pair<int, std::string>> _adulthoodChoiceText4;
	std::vector < pair<int, std::string>> _adulthoodChoiceText5;
	std::vector < pair<int, std::string>> _adulthoodChoiceText6;
	std::vector < pair<int, std::string>> _adulthoodChoiceText7;
	std::vector < pair<int, std::string>> _adulthoodChoiceText8;
	std::vector < pair<int, std::string>> _adulthoodChoiceText9;
	std::vector < pair<int, std::string>> _adulthoodChoiceText10;
	std::vector < pair<int, std::string>> _adulthoodChoiceText11;
	std::vector < pair<int, std::string>> _adulthoodChoiceText12;

	Vector4 _color;


public:

	bool _isPanelCheck = false;

	int _nowText = 0;;
	int _selectText = 0;

public:

	virtual void Awake() override;
	virtual void Update() override;

	int Text1Size() { return _adulthoodBaseText1.size(); }
	int Text2Size() { return _adulthoodBaseText2.size(); }
	int Text3Size() { return _adulthoodBaseText3.size(); }
	int Text4Size() { return _adulthoodBaseText4.size(); }
	int Text5Size() { return _adulthoodBaseText5.size(); }
	int Text6Size() { return _adulthoodBaseText6.size(); }
	int Text7Size() { return _adulthoodBaseText7.size(); }
	int Text8Size() { return _adulthoodBaseText8.size(); }
	int Text9Size() { return _adulthoodBaseText9.size(); }
	int Text10Size() { return _adulthoodBaseText10.size(); }

	int Text11Size() { return _adulthoodChoiceText1.size(); }
	int Text12Size() { return _adulthoodChoiceText2.size(); }
	int Text13Size() { return _adulthoodChoiceText3.size(); }
	int Text14Size() { return _adulthoodChoiceText4.size(); }
	int Text15Size() { return _adulthoodChoiceText5.size(); }
	int Text16Size() { return _adulthoodChoiceText6.size(); }
	int Text17Size() { return _adulthoodChoiceText7.size(); }
	int Text18Size() { return _adulthoodChoiceText8.size(); }
	int Text19Size() { return _adulthoodChoiceText9.size(); }
	int Text20Size() { return _adulthoodChoiceText10.size(); }
	int Text21Size() { return _adulthoodChoiceText11.size(); }
	int Text22Size() { return _adulthoodChoiceText12.size(); }

	// 누가 말하는지 리턴해줌
	int ReturnTalker1(int nowtalker);
	int ReturnTalker2(int nowtalker);
	int ReturnTalker3(int nowtalker);
	int ReturnTalker4(int nowtalker);
	int ReturnTalker5(int nowtalker);
	int ReturnTalker6(int nowtalker);
	int ReturnTalker7(int nowtalker);
	int ReturnTalker8(int nowtalker);
	int ReturnTalker9(int nowtalker);
	int ReturnTalker10(int nowtalker);

	int ReturnTalker11(int nowtalker);
	int ReturnTalker12(int nowtalker);
	int ReturnTalker13(int nowtalker);
	int ReturnTalker14(int nowtalker);
	int ReturnTalker15(int nowtalker);
	int ReturnTalker16(int nowtalker);
	int ReturnTalker17(int nowtalker);
	int ReturnTalker18(int nowtalker);
	int ReturnTalker19(int nowtalker);
	int ReturnTalker20(int nowtalker);
	int ReturnTalker21(int nowtalker);
	int ReturnTalker22(int nowtalker);
};

