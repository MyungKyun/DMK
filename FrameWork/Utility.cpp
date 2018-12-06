#include "stdafx.h"

void ConvertCharToTchar(CHAR* src, TCHAR* dest, size_t destLen)
{
#ifdef UNICODE
	if (destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (Int)destLen - 1);
#endif // UNICODE

}


void ConvertTcharToChar(TCHAR* src, CHAR* dest, size_t destLen)
{
#ifdef UNICODE
	if (destLen < 1)
	{
		return;
	}

	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (Int)destLen, NULL, FALSE);
#endif
}


void ConvertCharToWide(const CHAR* src, WCHAR* dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}

	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (Int)destLen - 1);
}


void ConvertWideToChar(const WCHAR* src, CHAR* dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}

	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (Int)destLen, NULL, FALSE);
}

Bool ExistDirectory(const WString& path)
{
	DWORD dwAttrib = ::GetFileAttributes(path.c_str());
	if (dwAttrib == 0xFFFFFFFF || 
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
	{
		return false;
	}

	return true;
}
