#pragma once
#include "stdafx.h"

const Lock* LockManager::SearchLockCycle(Lock * newLock) const
{
	//const Thread* thread = ThreadManager::GetInstance()->GetThread(LThreadId);
	//if (thread == nullptr)
	//{
	//	return nullptr;
	//}

	//std::vector<const Lock*> trace;
	//trace.push_back(newLock);

	//const Lock* deadLock = nullptr;
	//while (1)
	//{
	//	const Lock* threadLock = thread->GetLock();
	//	if (threadLock == nullptr)
	//	{
	//		break;
	//	}

	//	if (threadLock->GetLockId() == trace[0]->GetLockId())
	//	{
	//		deadLock = threadLock;
	//		break;
	//	}

	//	trace.push_back(threadLock);
	//	thread = ThreadManager::GetInstance()->GetThread(threadLock->GetThreadId());

	//	if (!thread)
	//	{
	//		break;
	//	}

	//	trace.empty();
	//	return deadLock;
	//}

	return nullptr;
}

const Lock* LockManager::CheckDeadLock(Lock* lock) const
{
	/*const Lock* deadLock = SearchLockCycle(lock);
	if (deadLock)
	{
		return deadLock;
	}
*/
	return nullptr;
}



