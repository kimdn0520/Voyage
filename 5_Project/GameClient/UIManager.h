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

	// �޴��г� Ȱ��ȭ/��Ȱ��ȭ
	void OnPanel();

	// �޴��г� ���ư��� ��ư�� ��������
	void Return();

	// �޴��г� ������ ��ư�� ��������
	void NewGame();

	// �޴��г� Ÿ��Ʋ�� ��ư�� ��������
	void GoTitle();

	// �޴��г� �ɼ� ��ư�� ��������
	void Option();

	// �޴��г� ������ ��ư�� ��������
	void ExitGame();

	// �ɼ� ��ư�� ������ �ɼ��гο��� Setting��ư�� ��������
	void SettingBtnOn();

	// �ɼ� ��ư�� ������ �ɼ��гο��� Control��ư�� ��������
	void ControlBtnOn();

public:
	virtual void Awake() override;
	virtual void Update() override;
};

