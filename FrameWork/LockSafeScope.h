#pragma once


class Lock;


class LockSafeScope
{
private:
	Lock* lock_;

public:

	LockSafeScope() {};
	~LockSafeScope();
	LockSafeScope(Lock* lock, LPCWSTR fileName, Int lineNo);

};

#define LOCK_SCOPE(lock)		LockSafeScope	__lockSafe(&lock, _W(__FILE__), __LINE__);


