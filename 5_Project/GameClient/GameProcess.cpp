#include "pch.h"
#include "GameProcess.h"
#include "NewbieEngineFunc.h"
#include "SceneManager.h"
#include "TestScene.h"
#include "DataManager.h"
#include "YJScene.h"
#include "Infancy.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "GameManager.h"
#include "EventMachine.h"
#include "YouthScene.h"
#include "AdulthoodScene.h"
#include "OldAgeScene.h"
#include "BadEndingScene.h"
#include "HappyEndingScene.h"

WindowInfo GameProcess::_windowInfo;
WindowInfo GameProcess::resizeInfo;

GameProcess::GameProcess()
{}

GameProcess::~GameProcess()
{}

void GameProcess::InitializeWindows(WindowInfo& windowInfo)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX = 0, posY = 0;

	// �� ��ü�� ���� �ܺ� �����͸� �����ɴϴ�.
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� �����ɴϴ�.
	_hInstance = GetModuleHandle(NULL);

	// ���ø����̼� �̸� ����
	_applicationName = L"JWEngine";

	// �⺻ �������� Windows Ŭ������ ������.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ���� ���
	RegisterClassEx(&wc);

	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)GetSystemMetrics(SM_CXSCREEN);
		dmScreenSettings.dmPelsHeight = (unsigned long)GetSystemMetrics(SM_CYSCREEN);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Change the display settings to full screen. ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN); // Set the position of the window to the top left corner. posX = posY = 0; } else { // If windowed then set it to 800x600 resolution. screenWidth = 800; screenHeight = 600; // Place the window in the middle of the screen. posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2; posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2; }
	}
	else
	{
		// Place the window in the middle of the screen. 
		posX = (GetSystemMetrics(SM_CXSCREEN) - windowInfo.width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - windowInfo.height) / 2;
	}

	// ȭ�� �������� â�� ����� �ڵ��� �����ɴϴ�.
	_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, _applicationName, _applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY,
		windowInfo.width, windowInfo.height,
		NULL, NULL, _hInstance, NULL
	);

	windowInfo.hwnd = _hWnd;

	// â�� ȭ�鿡 ���� ���� ��Ŀ���� �����մϴ�.
	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);

	// ������ â �缳��
	RecalcWindowSize();

	// ���콺 Ŀ���� ����ϴ�.
	//ShowCursor(false); 

	return;
}

void GameProcess::Run()
{
	MSG msg;

	// �޽��� ���� �ʱ�ȭ 
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			if (!SceneManager::GetInstance()->isPause)
			{
				GameManager::GetInstance()->Update();
				EventMachine::GetInstance()->Update();
			}

			EngineUpdate();

			EngineRender();
		}
	}
}

void GameProcess::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode. 
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window. 
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	// Remove the application instance. 
	UnregisterClass(_applicationName, _hInstance);
	_hInstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

void GameProcess::Init()
{
	_windowInfo.width = 1920;
	_windowInfo.height = 1080;
	_windowInfo.windowed = true;
	_windowInfo.hwnd = _hWnd;

	InitializeWindows(_windowInfo);

	EngineInit(_windowInfo);

	GameManager::GetInstance()->Init();

	{
		SceneManager::GetInstance()->InputScene<TestScene>("TestScene");
		SceneManager::GetInstance()->InputScene<YJScene>("YJScene");

		SceneManager::GetInstance()->InputScene<LoadingScene>("LoadingScene");
		SceneManager::GetInstance()->InputScene<TitleScene>("TitleScene");
		SceneManager::GetInstance()->InputScene<Infancy>("Infancy");
		SceneManager::GetInstance()->InputScene<YouthScene>("YouthScene");
		SceneManager::GetInstance()->InputScene<AdulthoodScene>("AdulthoodScene");
		SceneManager::GetInstance()->InputScene<OldAgeScene>("OldAgeScene");
		SceneManager::GetInstance()->InputScene<BadEndingScene>("BadEndingScene");
		SceneManager::GetInstance()->InputScene<HappyEndingScene>("HappyEndingScene");

		SceneManager::GetInstance()->LoadScene("TitleScene");			// ���⼭ � Scene�� load�Ҳ��� ����
	}
}

void GameProcess::Shutdown()
{
	EngineShutdown();

	DataManager::GetInstance()->Release();

	EventMachine::GetInstance()->Release();

	ShutdownWindows();
}

void GameProcess::RecalcWindowSize()
{
	RECT nowRect;
	DWORD _style = (DWORD)GetWindowLong(_hWnd, GWL_STYLE);
	DWORD _exstyle = (DWORD)GetWindowLong(_hWnd, GWL_EXSTYLE);

	GetWindowRect(_hWnd, &nowRect);

	RECT newRect;
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = _windowInfo.width;
	newRect.bottom = _windowInfo.height;

	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	// Ŭ���̾�Ʈ �������� ���� ũ��� �� Ŀ�� �Ѵ�. (�ܰ���, Ÿ��Ʋ ��)
	int _newWidth = (newRect.right - newRect.left);
	int _newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(_hWnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
		_newWidth, _newHeight, SWP_SHOWWINDOW);
}

LRESULT GameProcess::MessageHandler(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// �ٸ� ��� �޽����� ���ø����̼ǿ��� ������� �ʱ� ������ �⺻ �޽��� �ڵ鷯�� ���۵˴ϴ�.
		default:
		{
			return DefWindowProc(hWnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK GameProcess::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_SIZE:
		{
			if (wparam != SIZE_MINIMIZED)
			{
				ResizeWindow(LOWORD(lparam), HIWORD(lparam));
				resizeInfo.width = LOWORD(lparam);
				resizeInfo.height = HIWORD(lparam);
				//_windowInfo.width = LOWORD(lparam);
				//_windowInfo.height = HIWORD(lparam);
			}

			return 0;
		}
		// Check if the window is being destroyed. 
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed. 
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// All other messages pass to the message handler in the system class. 
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}