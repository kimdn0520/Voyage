#pragma once

//#define  _FMODAUDIO_DLL 
#ifdef _FMODAUDIO_DLL
// DLL ����
// DLL �� ����� ����  �Լ�/Ŭ�������� "export" �� ����.
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