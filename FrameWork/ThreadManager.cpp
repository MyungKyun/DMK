#include "stdafx.h"

ThreadManager::~ThreadManager()
{

}

Bool	ThreadManager::AddDepartment(Short deptNum, Iocp* iocp)
{
	if (0 > deptNum || deptNum >= MAX_DEPT)
	{
		return false;
	}

	if (nullptr != threadDept_[deptNum])
	{
		return false;
	}

	ThreadDepartment* dept = new ThreadDepartment(deptNum, iocp);
	threadDept_[deptNum] = dept;
	return true;
}


ThreadDepartment*		ThreadManager::GetDepartment(Short deptNum) const
{
	if (0 > deptNum || deptNum >= MAX_DEPT)
	{
		return nullptr;
	}

	return threadDept_[deptNum];
}
