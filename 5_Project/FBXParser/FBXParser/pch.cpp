// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.

std::wstring s2ws(const std::string& s)
{
	std::wstring message_w;     
    message_w.assign(s.begin(), s.end());

    return message_w;
}

std::string ws2s(const std::wstring& s)
{
    int len;
    int slength = static_cast<int>(s.length()) + 1;
    len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    std::string r(len, '\0');
    ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);

    return r;
}
