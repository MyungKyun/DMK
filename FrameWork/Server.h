#pragma once

class Server 
{
	enum Count 
	{
		//�ӽ�
		TOTAL_ACCEPT_COUNT	= 100,

		MAX_THREAD_COUNT = 64,
	};

protected:

	Iocp											iocp_;
	DWORD											startThreadCount_;    // �����带 � �������� ���� �켱�� �ý����ھ� ������..
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