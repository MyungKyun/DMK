#pragma once

extern thread_local		   DWORD		LThreadId;
extern thread_local		   Short	LDeptNumber;

Void Init_TLS(DWORD threadId, Short deptNumber);