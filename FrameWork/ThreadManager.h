#pragma once

enum
{
	NETWORK_PROCESSING_DEPT = 0,
	PACKET_PROCESSING_DEPT = 1,

	MAX_DEPT,
};



class ThreadManager : public Singleton<ThreadManager>
{

	using ThreadMap = std::unordered_map<UDLong, Thread*>;
	ThreadMap	threadMap_;
	Int			latesThreadId_ = 0;

	using ThreadGroup = std::array<ThreadMap, MAX_DEPT>;
	ThreadGroup			threadGroup_;

	

public:
	
	~ThreadManager();

	Void			PushThread(Thread* thread);
	Void			RemoveThread(Int id);
	
	Thread*			GetThread(Int id);
	Int				Gen();

};