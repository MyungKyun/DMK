#include "stdafx.h"

ThreadDepartment::ThreadDepartment(Short deptNum, Iocp* iocp)
	: deptNumber_(deptNum)
	, iocp_(iocp ? iocp : nullptr)
	, threadCount_(0)
{
	
}

ThreadDepartment::~ThreadDepartment()
{

}