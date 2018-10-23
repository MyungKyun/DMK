#include "stdafx.h"

void convertCharToTchar(CHAR* src, TCHAR* dest, size_t destLen)
{
#ifdef UNICODE
	if (destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (Int)destLen - 1);
#endif // UNICODE

}


void convertTcharToChar(TCHAR* src, CHAR* dest, size_t destLen)
{
#ifdef UNICODE
	if (destLen < 1)
	{
		return;
	}

	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (Int)destLen, NULL, FALSE);
#endif
}


void convertCharToWide(CHAR* src, WCHAR* dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}

	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (Int)destLen - 1);
}


void convertWideToChar(const WCHAR* src, CHAR* dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}

	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (Int)destLen, NULL, FALSE);
}