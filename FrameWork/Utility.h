#pragma once
#include "stdafx.h"
#include <algorithm>

#define __W(x)				L##x
#define _W(x)				__W(x)

#define snprintf(dest, format, ...)		_snprintf_s(dest.data(), dest.size(), _TRUNCATE, format, __VA_ARGS__)
#define snwprintf(dest , format, ...)	_snwprintf_s(dest.data(), dest.size(), _TRUNCATE, format, __VA_ARGS__)


void convertCharToTchar(CHAR* src, TCHAR* dest, size_t destLen);
void convertTcharToChar(TCHAR* src, CHAR* dest, size_t destLen);
void convertCharToWide(const CHAR* src, WCHAR* dest, size_t destLen);
void convertWideToChar(const WCHAR* src, CHAR* dest, size_t destLen);
	
