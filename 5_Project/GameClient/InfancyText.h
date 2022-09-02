#pragma once
#include "MonoBehaviour.h"

class Text;

class InfancyText : public MonoBehaviour
{
public:
	InfancyText(shared_ptr<GameObject> gameObject);
	virtual ~InfancyText();


private:
	shared_ptr<Text> _tempText;

	// Infancy Text
	std::vector<pair<int, std::string>> _infancyText1;
	std::vector<pair<int, std::string>> _infancyText2;
	std::vector<pair<int, std::string>> _infancyText3;
	std::vector<pair<int, std::string>> _infancyText4;


	Vector4 _color;

public:

	bool _isPanelCheck = false;

	int _nowText = 0;;
	int _selectText = 0;

public:

	virtual void Awake() override;
	virtual void Update() override;

	int Text1Size() { return _infancyText1.size(); }
	int Text2Size() { return _infancyText2.size(); }
	int Text3Size() { return _infancyText3.size(); }
	int Text4Size() { return _infancyText4.size(); }

	// 누가 말하는지 리턴해줌
	int ReturnTalker1(int nowtalker);
	int ReturnTalker2(int nowtalker);
	int ReturnTalker3(int nowtalker);
	int ReturnTalker4(int nowtalker);
	
};

