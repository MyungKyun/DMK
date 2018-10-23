#pragma once



class Thread
{
	std::thread*				thread_ = nullptr;
	Lock*						lock_ = nullptr;
	Int							threadId_ = 0;
	UShort						department_;
	
public:

	Thread(std::thread* thread, UShort department);
	~Thread();

	Void				SetLock(Lock* lock);
	const Lock*			GetLock()	const;
	Int					GetId()		const;

	UShort				GetDepartment();
};


#define _CREATE_THREAD(className, proc, dept) (new Thread(new std::thread(&className##::##proc,this), dept ));