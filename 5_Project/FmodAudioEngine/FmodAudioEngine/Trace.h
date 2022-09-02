#pragma once

#include <malloc.h>
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <debugapi.h>

// https://jhnyang.tistory.com/298
// 앞에 있는 ...의  __VA_ARGS__ 가변 인수 매크로 

# define _TRACE(x,...) Trace(x,##__VA_ARGS__)
# define _TRACE_LOG Trace("File : %s " ,__FILE__); Trace("Func : %s ", __FUNCTION__);


inline void Trace(char const* const format, ...)
{
	// https://jhnyang.tistory.com/293
	// 가변 인자 함수 
	va_list args;
	int     len;
	char* buffer;

	// retrieve the variable arguments
	va_start(args, format);


	len = _vscprintf(format, args) // _vscprintf doesn't count
		+ 1; // terminating '\0'

	buffer = (char*)malloc(len * sizeof(char));

	if (0 != buffer)
	{
		vsprintf_s(buffer, len, format, args); // C4996
		// Note: vsprintf is deprecated; consider using vsprintf_s instead
		puts(buffer);

		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");

		free(buffer);
	}
	va_end(args);
}