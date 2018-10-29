#pragma once

class ThreadManager : public Singleton<ThreadManager>
{

	using ThreadMap = std::unordered_map<UDLong, Thread*>;
	ThreadMap	threadMap_;
	//Int			latesThreadId_ = 0;
	std::atomic_ulong	latestThreadId_;

	//using ThreadGroup = std::array<ThreadMap, MAX_DEPT>;
	//ThreadGroup			threadGroup_;

	ThreadDepartment*		threadDept_[MAX_DEPT];

public:
	
	~ThreadManager();

	Bool	 AddDepartment(Short deptNum, Iocp* iocp);

	template <typename ThreadType, typename... Args>
	Bool	  Start(int threadCount, Args&&... args)
	{
		int deptNum = ThreadType::GetDepartmentNumber();
		ThreadDepartment*	dept = GetDepartment(deptNum);
		assert(dept != nullptr);

		if (false == dept->Start<ThreadType>(threadCount, std::forward<Args>(args)...))
		{
			return false;
		}

		return true;
	}


	ThreadDepartment*		GetDepartment(Short deptNum) const;
};

#define		GThreadManager		Singleton<ThreadManager>::GetInstance()