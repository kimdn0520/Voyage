#pragma once
#include "MonoBehaviour.h"

class Transform;
class Animator;

class UIManager : public MonoBehaviour
{
public:
	UIManager(shared_ptr<GameObject> gameObject);
	virtual ~UIManager();

private:
	shared_ptr<GameObject> _menu;
	shared_ptr<GameObject> _settingPanel;
	shared_ptr<GameObject> _controlPanel;
	shared_ptr<GameObject> _heartGaugePanel;

	shared_ptr<GameObject> _returnBtn;
	shared_ptr<GameObject> _newGameBtn;
	shared_ptr<GameObject> _titleBtn;
	shared_ptr<GameObject> _optionBtn;
	shared_ptr<GameObject> _gameExitBtn;
	shared_ptr<GameObject> _backBtn;
	shared_ptr<GameObject> _nextBtn;

	shared_ptr<GameObject> _yesBtn;
	shared_ptr<GameObject> _noBtn;

public:
	void SetMenuPanel(shared_ptr<GameObject> menu) { _menu = menu; }

	void SetReturnBtn(shared_ptr<GameObject> returnBtn) { _returnBtn = returnBtn; }

	void SetNewGameBtn(shared_ptr<GameObject> newGameBtn) { _newGameBtn = newGameBtn; }

	void SetTitleBtn(shared_ptr<GameObject> titleBtn) { _titleBtn = titleBtn; }

	void SetOptionBtn(shared_ptr<GameObject> optionBtn) { _optionBtn = optionBtn; }

	void SetGameExitBtn(shared_ptr<GameObject> gameExitBtn) { _gameExitBtn = gameExitBtn; }
	
	void SetSettingPanel(shared_ptr<GameObject> settingPanel) { _settingPanel = settingPanel; }

	void SetControlPanel(shared_ptr<GameObject> controlPanel) { _controlPanel = controlPanel; }

	void SetSettingBtn(shared_ptr<GameObject> settingBtn) { _backBtn = settingBtn; }

	void SetControlBtn(shared_ptr<GameObject> controlBtn) { _nextBtn = controlBtn; }

	void SetHeartGauge(shared_ptr<GameObject> heartGaugePanel) { _heartGaugePanel = heartGaugePanel; }

	void SetYesBtn(shared_ptr<GameObject> yesBtn) { _yesBtn = yesBtn; }

	void SetNoBtn(shared_ptr<GameObject> noBtn) { _noBtn = noBtn; }

	// 메뉴패널 활성화/비활성화
	void OnPanel();

	// 메뉴패널 돌아가기 버튼을 눌렀을때
	void Return();

	// 메뉴패널 새게임 버튼을 눌렀을때
	void NewGame();

	// 메뉴패널 타이틀로 버튼을 눌렀을때
	void GoTitle();

	// 메뉴패널 옵션 버튼을 눌렀을때
	void Option();

	// 메뉴패널 나가기 버튼을 눌렀을때
	void ExitGame();

	// 옵션 버튼을 누른뒤 옵션패널에서 Setting버튼을 눌렀을때
	void SettingBtnOn();

	// 옵션 버튼을 누른뒤 옵션패널에서 Control버튼을 눌렀을때
	void ControlBtnOn();

public:
	virtual void Awake() override;
	virtual void Update() override;
};

