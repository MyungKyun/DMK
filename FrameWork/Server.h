#pragma once

class Server 
{
	enum Count 
	{
		//임시
		TOTAL_ACCEPT_COUNT	= 100,

		MAX_THREAD_COUNT = 64,
	};

protected:

	Iocp											iocp_;
	DWORD											startThreadCount_;    // 스레드를 몇개 생성할지 변수 우선은 시스템코어 개수로..
	PacketProcess*									contentLogicProcess_ = nullptr;
	ServerNetWorkDepartment*						serverNetDept_ = nullptr;
	
public:

	Server(PacketProcess* contentLogicProcess);
	~Server();

	Bool			Setup(SessionPool* sessionPool);
	Void			Run();
	Void			ShutDown();

private:

	Bool startup();
	Void threadRun();
};