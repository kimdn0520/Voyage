#pragma once

//#define  _FMODAUDIO_DLL 
#ifdef _JSON_DLL
// DLL ����
// DLL �� ����� ����  �Լ�/Ŭ�������� "export" �� ����.
#define _JSONDLL __declspec(dllexport)
//#define _TEMPLATE


#else
#define _JSONDLL __declspec(dllimport)
//#define _TEMPLATE extern
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "FmodAudioEngine_Debug.lib")		//64bit + debug

#else
#pragma comment(lib, "FmodAudioEngine_Release.lib")		//64bit.


#endif
#endif
#endif