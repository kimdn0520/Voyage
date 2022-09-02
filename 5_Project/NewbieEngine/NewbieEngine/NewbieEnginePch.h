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
	HWND	hwnd;		// 출력 윈도우 핸들
	__int32	width;  // 너비
	__int32	height; // 높이
	bool	windowed;		// 창모드 or 전체화면
};
						
