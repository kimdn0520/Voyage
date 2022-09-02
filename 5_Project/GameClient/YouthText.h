#pragma once
#include "MonoBehaviour.h"

class Text;

class YouthText : public MonoBehaviour
{
public:
	YouthText(shared_ptr<GameObject> gameObject);
	virtual ~YouthText();


private:
	shared_ptr<Text> _tempText;


	// Infancy Text
	std::vector<pair<int, std::string>> _youthText1;
	std::vector<pair<int, std::string>> _youthText2;
	std::vector<pair<int, std::string>> _youthText3;
	std::vector<pair<int, std::string>> _youthText4;

	Vector4 _color;

public:

	bool _isPanelCheck = false;

	int _nowText = 0;
	int _selectText = 0;

public:

	virtual void Awake() override;
	virtual void Update() override;

	int Text1Size() { return _youthText1.size(); }
	int Text2Size() { return _youthText2.size(); }
	int Text3Size() { return _youthText3.size(); }
	int Text4Size() { return _youthText4.size(); }


	// 누가 말하는지 리턴해줌
	int ReturnTalker1(int nowtalker);
	int ReturnTalker2(int nowtalker);
	int ReturnTalker3(int nowtalker);
	int ReturnTalker4(int nowtalker);
};

