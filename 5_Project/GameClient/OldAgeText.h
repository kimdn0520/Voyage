#pragma once
#include "MonoBehaviour.h"

class Text;

class OldAgeText : public MonoBehaviour
{
public:
	OldAgeText(shared_ptr<GameObject> gameObject);
	virtual ~OldAgeText();


private:
	shared_ptr<Text> _tempText;

	// OldAge Text
	std::vector<pair<int, std::string>> _oldAgeText1;
	std::vector<pair<int, std::string>> _oldAgeText2;
	std::vector<pair<int, std::string>> _oldAgeText3;
	std::vector<pair<int, std::string>> _oldAgeText4;


	Vector4 _color;

public:

	bool _isPanelCheck = false;

	int _nowText = 0;
	int _selectText = 0;

public:

	virtual void Awake() override;
	virtual void Update() override;

	int Text1Size() { return _oldAgeText1.size(); }
	int Text2Size() { return _oldAgeText2.size(); }
	int Text3Size() { return _oldAgeText3.size(); }
	int Text4Size() { return _oldAgeText4.size(); }

	// 누가 말하는지 리턴해줌
	int ReturnTalker1(int nowtalker);
	int ReturnTalker2(int nowtalker);
	int ReturnTalker3(int nowtalker);




};