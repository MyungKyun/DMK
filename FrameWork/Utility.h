#pragma once
#include "stdafx.h"
#include <algorithm>

#define __W(x)				L##x
#define _W(x)				__W(x)

#define snprintf(dest, format, ...)		_snprintf_s(dest.data(), dest.size(), _TRUNCATE, format, __VA_ARGS__)
#define snwprintf(dest , format, ...)	_snwprintf_s(dest.data(), dest.size(), _TRUNCATE, format, __VA_ARGS__)


void ConvertCharToTchar(CHAR* src, TCHAR* dest, size_t destLen);
void ConvertTcharToChar(TCHAR* src, CHAR* dest, size_t destLen);
void ConvertCharToWide(const CHAR* src, WCHAR* dest, size_t destLen);
void ConvertWideToChar(const WCHAR* src, CHAR* dest, size_t destLen);
	


class IdGenerator : public Singleton<IdGenerator>
{
	std::atomic<UInt64>		sessionIdGen_{ 0 };

public:

	UInt64				SessionIdGenerate()
	{
		return sessionIdGen_.fetch_add(1);
	}
};

#define		GIDGen		Singleton<IdGenerator>::GetInstance()



Bool ExistDirectory(const WString& path);
