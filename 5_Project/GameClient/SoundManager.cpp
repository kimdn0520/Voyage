#include "pch.h"
#include "SoundManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Camera.h"
#include "Button.h"
#include "Audio.h"
#include "GameManager.h"

SoundManager::SoundManager(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()),
	_audio(gameObject->GetComponent<Audio>())
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Update()
{

}



void SoundManager::OnPanel()
{
	/*if (_menu->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		
	}
	else if ((_menu->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		
	}*/
}

void SoundManager::Return()
{
	if (_returnBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_returnBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::NewGame()
{
	if (_newGameBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
	/*	_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_newGameBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::GoTitle()
{
	if (_titleBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_titleBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::Option()
{
	if (_optionBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_optionBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::ExitGame()
{
	if (_gameExitBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_gameExitBtn->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::SettingBtnOn()
{
	if (_settingPanel->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_settingPanel->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::ControlBtnOn()
{
	if (_controlPanel->GetComponent<Button>()->GetStatus() == ButtonStatus::PRESSED)
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
	else if ((_controlPanel->GetComponent<Button>()->GetStatus() == ButtonStatus::HIGHLIGHT))
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::GetHeart()
{
	if (items[ItemType::Heart])
	{
		/*_audio->PlayEvent("event:/SFX/SFX_Item");*/
	}
}

void SoundManager::GetMoney()
{
	if (GameManager::GetInstance()->items[ItemType::Money])
	{
		/*_audio->PlayEvent("event:/SFX/SFX_Item");*/
	}
}

void SoundManager::GetNote()
{
	if (items[ItemType::Note])
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/

	}

}

void SoundManager::GetRose()
{
	if (items[ItemType::Rose])
	{
		/*_audio->PlayEvent("event:/SFX/SFX_WaveSound");*/
	}
}

void SoundManager::TitleSound()
{
	/*_audio->PlayEvent("event:/SFX/SFX_Stream",true);*/
}

void SoundManager::GetBoat()
{
	/*_audio->PlayEvent(("event:/SFX/SFX")*/
}

void SoundManager::Talk()
{
	/*_audio->PlayEvent("event:/SFX/SFX_Talk");*/
}
