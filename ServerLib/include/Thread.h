#pragma once

namespace
{
	const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)  
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.  
		LPCSTR szName; // Pointer to name (in user addr space).  
		DWORD dwThreadID; // Thread ID (-1=caller thread).  
		DWORD dwFlags; // Reserved for future use, must be zero.  
	} THREADNAME_INFO;
#pragma pack(pop) 


	Void	SetThreadName(DWORD threadId, LPCSTR name)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = name;
		info.dwThreadID = static_cast<DWORD>(-1);
		info.dwFlags = 0;

		__try 
		{
			::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) 
		{
		}
	}
}




class Thread
{
	std::thread			thread_;
	String				name_;
	Short				deptNumber_;
	DWORD				id_;
protected:

	Bool				isStop_ = false;

public:
	
	Thread(DWORD id, const String& threadName, Short deptNum);
	virtual			~Thread();


	Void			Initialize();
	virtual Bool	Setup();
	virtual Void	Run() = 0;
	virtual Void	ShutDown() = 0;

	Short			GetDepartmentNumber();
	String			GetName();
	DWORD			GetId();
private:

	static Void ThreadRun(Thread* thread);
	
};