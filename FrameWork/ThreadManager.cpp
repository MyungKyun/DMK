#include "stdafx.h"

std::condition_variable	 hasPacketData_;
std::atomic_bool				needNotifyPacketInTheQueue_ = false;
ThreadManager::~ThreadManager()
{

}


Void ThreadManager::PushThread(Thread* thread)
{
	//threadMap_.emplace(thread->GetId(), thread);
	threadGroup_[thread->GetDepartment()].emplace(thread->GetId(), thread);
}

Void	ThreadManager::RemoveThread(Int id)
{
	auto found = threadMap_.find(id);
	if (found == threadMap_.cend())
	{
		return;
	}

	threadMap_.erase(found);
}

Thread* ThreadManager::GetThread(Int id)
{
	if (threadMap_.empty())
	{
		return nullptr;
	}

	auto found = threadMap_.find(id);
	if (found == threadMap_.cend())
	{
		return nullptr;
	}

	return found->second;
}

//Void	ThreadManager::WaitGroup(UShort department)
//{
//	decltype(auto) threadsMap = threadGroup_[department];
//	for ( auto& i : threadsMap)
//	{
//		if (nullptr == i.second)
//		{
//			continue;
//		}
//		
//		i.second->Wait(&hasPacketData_);
//	}
//}
//
//Void	ThreadManager::OnNeedNotifyPacketInTheQueue()
//{
//	needNotifyPacketInTheQueue_ = true;
//}
//
//Void	ThreadManager::EmptyPacketQueue()
//{
//	needNotifyPacketInTheQueue_.store(false);
//}
//
//Void		ThreadManager::NeedNotify()
//{
//	if (false == needNotifyPacketInTheQueue_.load())
//	{
//		needNotifyPacketInTheQueue_.store(true);
//	}
//	NotifyAll();
//}
//
//Void	ThreadManager::NotifyAll()
//{
//	hasPacketData_.notify_all();
//}

Int	ThreadManager::Gen()
{
	return latesThreadId_++;
}