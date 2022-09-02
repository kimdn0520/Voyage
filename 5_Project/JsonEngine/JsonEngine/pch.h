// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H


#include "Rapidjson/rapidjson.h"
#include "Rapidjson/document.h"
#include "Rapidjson/prettywriter.h"		// for stringify JSON
#include "Rapidjson/filereadstream.h"	// fopen입출력을 위해..
#include "Rapidjson/filewritestream.h"
#include "Rapidjson/error/error.h"		// 에러판단을 위해서
#include "Rapidjson/error/en.h"
#include "Rapidjson/encodedstream.h"



// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

// C++ / STL
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <array>

#include <Windows.h>
#include <tchar.h>
#endif //PCH_H
