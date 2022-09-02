#pragma once

//#define  _FMODAUDIO_DLL 
#ifdef _FMODAUDIO_DLL
// DLL 제작
// DLL 을 만들기 위해  함수/클래스등을 "export" 로 선언.
#define _FMODAUDIODLL __declspec(dllexport)
//#define _TEMPLATE


#else
#define _FMODAUDIODLL __declspec(dllimport)
//#define _TEMPLATE extern
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "FmodAudioEngine_Debug.lib")		//64bit + debug

#else
#pragma comment(lib, "FmodAudioEngine_Release.lib")		//64bit.


#endif
#endif
#endif