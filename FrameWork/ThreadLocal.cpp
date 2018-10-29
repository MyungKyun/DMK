#include "stdafx.h"

thread_local	DWORD	LThreadId = -1;
thread_local    Short	LDeptNumber = -1;

Void Init_TLS(DWORD threadId, Short deptNumber)
{
	LThreadId = threadId;
	LDeptNumber = deptNumber;
}