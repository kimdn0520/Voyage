#pragma once
#include "MonoBehaviour.h"

class Transform;
class Animator;
class Audio;

class SoundManager : public MonoBehaviour
{
public:
	SoundManager(shared_ptr<GameObject> gameObject);
	virtual ~SoundManager();

private:
	shared_ptr<Audio>	   _audio;
	shared_ptr<Transform>  _transform;
	shared_ptr<GameObject> _menu;
	shared_ptr<GameObject> _settingPanel;
	shared_ptr<GameObject> _controlPanel;
	shared_ptr<GameObject> _returnBtn;
	shared_ptr<GameObject> _newGameBtn;
	shared_ptr<GameObject> _titleBtn;
	shared_ptr<GameObject> _optionBtn;
	shared_ptr<GameObject> _gameExitBtn;
	shared_ptr<GameObject> _settingBtn;
	shared_ptr<GameObject> _controlBtn;
	shared_ptr<GameObject> _getheart;

	unordered_map<ItemType, int> items;

	bool _bBgmPlaying = false;

public:
	virtual void Update() override;

	void SetMenuPanel(shared_ptr<GameObject> menu) { _menu = menu; }

	void SetReturnBtn(shared_ptr<GameObject> returnBtn) { _returnBtn = returnBtn; }

	void SetNewGameBtn(shared_ptr<GameObject> newGameBtn) { _newGameBtn = newGameBtn; }

	void SetTitleBtn(shared_ptr<GameObject> titleBtn) { _titleBtn = titleBtn; }

	void SetOptionBtn(shared_ptr<GameObject> optionBtn) { _optionBtn = optionBtn; }

	void SetGameExitBtn(shared_ptr<GameObject> gameExitBtn) { _gameExitBtn = gameExitBtn; }

	void SetSettingPanel(shared_ptr<GameObject> settingPanel) { _settingPanel = settingPanel; }

	void SetControlPanel(shared_ptr<GameObject> controlPanel) { _controlPanel = controlPanel; }

	void SetSettingBtn(shared_ptr<GameObject> settingBtn) { _settingBtn = settingBtn; }

	void SetControlBtn(shared_ptr<GameObject> controlBtn) { _controlBtn = controlBtn; }

	void GetHeartGauge(shared_ptr<GameObject> getheart) { _getheart = getheart; }

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

	void GetHeart();

	void GetMoney();
	
	void GetNote();

	void GetRose();

	void TitleSound();

	void GetBoat();

	void Talk();

	
};

