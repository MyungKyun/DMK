#pragma once

class ThreadDepartment
{

	Short									deptNumber_;
	Iocp*									iocp_;
	std::vector<std::unique_ptr<Thread>>	threads_;
	std::atomic_bool						isStop_;

public:

	ThreadDepartment(Short deptNum, Iocp* iocp);
	~ThreadDepartment();

	template <typename ThreadType, typename... Args>
	Bool	 Start(Int threadCount, Args&&... args)
	{
		if (0 >= threadCount)
		{
			::SYSTEM_INFO info;
			::GetSystemInfo(&info);

			threadCount = info.dwNumberOfProcessors;
		}

		for (Int i = 0; i < threadCount; ++i)
		{
			threads_.push_back(std::make_unique<ThreadType>(std::forward<Args>(args)...));
		}

		for (auto& thread : threads_)
		{
			auto future = std::async(std::launch::async, [&]() {thread->Initialize(); });
			future.get();
		}

		isStop_ = false;

		return true;
	}
};
