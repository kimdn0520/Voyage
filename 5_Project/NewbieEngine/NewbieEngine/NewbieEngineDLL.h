#pragma once

#ifdef NewbieEngine_EXPORTS
#define NewbieEngine_DLL __declspec(dllexport)
#else
#define NewbieEngine_DLL __declspec(dllimport)
#endif