#include "pch.h"
#include "GameManager.h"
#include "InputManager.h"

shared_ptr<GameManager> GameManager::gameManager = nullptr;

GameManager::GameManager()
{}

GameManager::~GameManager()
{}

shared_ptr<GameManager> GameManager::GetInstance()
{
	if (gameManager == nullptr)
		gameManager = make_shared<GameManager>();

	return gameManager;
}

void GameManager::Init()
{
	items[ItemType::Heart] = 5;
	items[ItemType::Star] = 0;
	items[ItemType::BigStar] = 0;
	items[ItemType::Money] = 0;
	items[ItemType::Note] = 0;
	items[ItemType::Rose] = 0;

	speed = 20.f;
}

void GameManager::ClearData()
{
	items.clear();
}

void GameManager::Update()
{
	speed = 20.f + items[ItemType::Star];

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::KEY_1))
	{
		scriptSpeed -= 1.0f;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::KEY_2))
	{
		scriptSpeed += 1.0f;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::KEY_3))
	{
		items[ItemType::Heart] = 30;
	}
}

