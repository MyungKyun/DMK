#include "stdafx.h"

__declspec(thread)	Int	LThreadId = -1;

Void Init_TLS(Int threadId)
{
	LThreadId = threadId;
}