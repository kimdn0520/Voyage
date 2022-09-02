#include "pch.h"
#include "UIManager.h"
#include "Button.h"
#include "InputManager.h"
#include "Panel.h"
#include "GameObject.h"
#include "GameProcess.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "GameManager.h"
#include "EventMachine.h"
#include "EVENT_TITLE.h"

UIManager::UIManager(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject)
{}

UIManager::~UIManager()
{}

void UIManager::Awake()
{
	
}

void UIManager::Update()
{
	// ü�°����� update
	_heartGaugePanel->GetComponent<Panel>()->SetOffset
	(
		0 + 120.f,
		0 + 120.f + ((GameProcess::_windowInfo.width / 2 - 746.f) / 30) * GameManager::GetInstance()->items[ItemType::Heart],
		GameProcess::_windowInfo.height / 2 + 450.f,
		GameProcess::_windowInfo.height / 2 + 480.f
	);
	_heartGaugePanel->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
	_heartGaugePanel->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);


	if (_yesBtn != nullptr && _noBtn != nullptr)
	{
		if (_yesBtn->activeSelf && _noBtn->activeSelf)
		{
			if (_yesBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
			{
				// �����̰��ϰ� ��ư���� ���� �̺�Ʈ�ӽ��� ���� �ٲ��ش�.
				SceneManager::GetInstance()->isPause = false;
				_yesBtn->SetActive(false);
				_noBtn->SetActive(false);
				EventMachine::GetInstance()->isSelectedYes = true;
			}
			else if (_noBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
			{
				// �����̰��ϰ� ��ư���� ���� �̺�Ʈ�ӽ��� ���� �ٲ��ش�.
				SceneManager::GetInstance()->isPause = false;
				_yesBtn->SetActive(false);
				_noBtn->SetActive(false);
				EventMachine::GetInstance()->isSelectedNo = true;
			}
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::ESCAPE) && !_settingPanel->activeSelf && !_controlPanel->activeSelf)
	{
		OnPanel();
	}

	// �޴��� Ȱ��ȭ ���¶��
	if (_menu->activeSelf)
	{
		Return();

		NewGame();

		GoTitle();

		Option();

		ExitGame();
	}

	// �ɼ� �г��� Ȱ��ȭ ���¶��
	if (_settingPanel->activeSelf)
	{
		// esc ������ �ɼ��г��� ���� �޴��г��� Ȱ��ȭ ��Ŵ
		if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::ESCAPE))
		{
			_settingPanel->SetActive(false);

			OnPanel();
		}

		// TODO : ���� ��ư�� ��ȣ�ۿ�
		ControlBtnOn();
	}

	// ��Ʈ�� �г��� Ȱ��ȭ ���¶��
	if (_controlPanel->activeSelf)
	{
		// esc ������ ��Ʈ���г��� ���� �޴��г��� Ȱ��ȭ ��Ŵ
		if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::ESCAPE))
		{
			_controlPanel->SetActive(false);

			OnPanel();
		}

		SettingBtnOn();
	}
}

void UIManager::OnPanel()
{
	_menu->activeSelf ? _menu->SetActive(false) : _menu->SetActive(true);

	if(_menu->activeSelf)
		SceneManager::GetInstance()->isPause = true;
	else
		SceneManager::GetInstance()->isPause = false;
}

void UIManager::Return()
{
	if (_returnBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{ 
		_menu->SetActive(false);
		SceneManager::GetInstance()->isPause = false;
	}
}

void UIManager::NewGame()
{
	if (_newGameBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("Infancy");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");

		SceneManager::GetInstance()->isPause = false;

		GameManager::GetInstance()->ClearData();

		EventMachine::GetInstance()->ChangeState(EventMachine::TITLE);

		EventMachine::GetInstance()->ResetMachine();
	}
}

void UIManager::GoTitle()
{
	if (_titleBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("TitleScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");

		SceneManager::GetInstance()->isPause = false;

		GameManager::GetInstance()->ClearData();

		EventMachine::GetInstance()->ChangeState(EventMachine::TITLE);

		EventMachine::GetInstance()->ResetMachine();
	}
}

void UIManager::Option()
{
	if (_optionBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{
		OnPanel();

		_settingPanel->SetActive(true);

		SceneManager::GetInstance()->isPause = true;
	}
}

void UIManager::ExitGame()
{
	if (_gameExitBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{
		DestroyWindow(GameProcess::_windowInfo.hwnd);
	}
}

void UIManager::SettingBtnOn()
{
	if (_backBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{
		_settingPanel->SetActive(true);

		_controlPanel->SetActive(false);
	}
}

void UIManager::ControlBtnOn()
{
	if (_nextBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
	{
		_controlPanel->SetActive(true);
		
		_settingPanel->SetActive(false);
	}
}