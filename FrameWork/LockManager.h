#pragma once

class Lock;

class LockManager : public Singleton<LockManager>
{
private:
	DLong lockIdSeed_ = 0;

public:

	const Lock* SearchLockCycle(Lock* newLock) const;
	const Lock* CheckDeadLock(Lock* lock) const;


	DLong LockManager::GenId()
	{
		DLong id = lockIdSeed_++;
		return id;
	}
};
