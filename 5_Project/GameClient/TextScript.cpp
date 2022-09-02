#include "pch.h"
#include "TextScript.h"
#include "Text.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "Transform.h"
#include "GameProcess.h"
#include "InputManager.h"
#include "GraphicsEngineManager.h"
#include "GameManager.h"

TextScript::TextScript(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_text(gameObject->GetComponent<Text>()), _color(Vector4( 1.0f, 1.0f, 1.0f, 1.0f ))
{}

TextScript::~TextScript()
{}

void TextScript::SetTransform(shared_ptr<Transform> boat)
{
	_boatTransform = boat;
}

void TextScript::Awake()
{
	_text->GetTextInfo()->color = _color;
	_text->GetTextInfo()->scale = 2.0f;		// 텍스트 크기조절 안해주면 default로 1.0f 들어감

}

void TextScript::Update()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F1))
	{
		_isDebug ? _isDebug = false : _isDebug = true;
		GraphicsEngineManager::GetInstance()->ColDebugSwitch();
		_text->GetTextInfo()->text = "";
	}

	if(_isDebug)
	{ 
		_text->GetTextInfo()->text = "[NewBie Engine Demo]\nFPS : " + to_string(TimeManager::GetInstance()->GetFps())
									+ "\nHeartItemCnt : " + to_string(GameManager::GetInstance()->items[ItemType::Heart])
									+ "\nStarItemCnt : " + to_string(GameManager::GetInstance()->items[ItemType::Star])
									+ "\nBigStarItemCnt : " + to_string(GameManager::GetInstance()->items[ItemType::BigStar])
									+ "\nMoneyItemCnt : " + to_string(GameManager::GetInstance()->items[ItemType::Money])
									+ "\nRoseItemCnt : " + to_string(GameManager::GetInstance()->items[ItemType::Rose])
									+ "\nBoat Speed : " + to_string(GameManager::GetInstance()->speed)
									+ "\nGameSpeed : " + to_string(GameManager::GetInstance()->scriptSpeed);

									/*+"\nposition x : " + to_string(_boatTransform->GetWorldPosition().x)
									+ "\nposition y : " + to_string(_boatTransform->GetWorldPosition().y)
									+ "\nposition z : " + to_string(_boatTransform->GetWorldPosition().z)*/
	}
}
