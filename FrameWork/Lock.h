#pragma once

//
//class Lock
//{
//private:
//	Lock_t  mtx_;
//	DLong	lockId_;
//	Int		threadId_;
//
//	WString		LockfileName_;
//	Int			LockLine_;
//
//public:
//
//	Lock();
//	~Lock();
//	
//	Void DoLock(LPCWSTR fileName, Int lineNo);
//	Void DoUnlock();
//	
//	Void SetThreadId(Int id);
//
//
//	UDLong GetLockId() const;
//	Int GetThreadId() const;
//	Lock_t& GetMutex();	
//};

#define		SCOPED_LOCK(lock)			if(std::lock_guard<std::recursive_mutex> lock(lock); true)

#define		DECLARE_LOCK		std::shared_mutex	lock
#define		WRITE_LOCK			std::unique_lock<std::shared_mutex> writeLock(lock)
#define		READ_LOCK			std::shared_lock<std::shared_mutex> readLock(lock)