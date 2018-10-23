#pragma once


class Lock
{
private:
	Lock_t  mtx_;
	DLong	lockId_;
	Int		threadId_;

	WString		LockfileName_;
	Int			LockLine_;

public:

	Lock();
	~Lock();
	
	Void DoLock(LPCWSTR fileName, Int lineNo);
	Void DoUnlock();
	
	Void SetThreadId(Int id);


	UDLong GetLockId() const;
	Int GetThreadId() const;
	Lock_t& GetMutex();	
};
