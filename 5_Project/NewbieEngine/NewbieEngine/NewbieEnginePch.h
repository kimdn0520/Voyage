#pragma once

// include
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <algorithm> 

#include "SimpleMath.h"

// using
using namespace DirectX;
using namespace std;
using namespace SimpleMath;

// lib

// const
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

struct WindowInfo
{
	HWND	hwnd;		// ��� ������ �ڵ�
	__int32	width;  // �ʺ�
	__int32	height; // ����
	bool	windowed;		// â��� or ��üȭ��
};
						
