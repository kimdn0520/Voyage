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
	// 체력게이지 update
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
				// 움직이게하고 버튼들을 끄고 이벤트머신의 값을 바꿔준다.
				SceneManager::GetInstance()->isPause = false;
				_yesBtn->SetActive(false);
				_noBtn->SetActive(false);
				EventMachine::GetInstance()->isSelectedYes = true;
			}
			else if (_noBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSEDUP)
			{
				// 움직이게하고 버튼들을 끄고 이벤트머신의 값을 바꿔준다.
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

	// 메뉴가 활성화 상태라면
	if (_menu->activeSelf)
	{
		Return();

		NewGame();

		GoTitle();

		Option();

		ExitGame();
	}

	// 옵션 패널이 활성화 상태라면
	if (_settingPanel->activeSelf)
	{
		// esc 누르면 옵션패널을 끄고 메뉴패널을 활성화 시킴
		if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::ESCAPE))
		{
			_settingPanel->SetActive(false);

			OnPanel();
		}

		// TODO : 여러 버튼들 상호작용
		ControlBtnOn();
	}

	// 컨트롤 패널이 활성화 상태라면
	if (_controlPanel->activeSelf)
	{
		// esc 누르면 컨트롤패널을 끄고 메뉴패널을 활성화 시킴
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
		// 다음에 로드할 씬이름을 Set해주고
		SceneManager::GetInstance()->SetLoadSceneName("Infancy");

		// 로딩씬으로 넘어간다.
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
		// 다음에 로드할 씬이름을 Set해주고
		SceneManager::GetInstance()->SetLoadSceneName("TitleScene");

		// 로딩씬으로 넘어간다.
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