#pragma once

class Server 
{
	enum Count 
	{
		//юс╫ц
		TOTAL_ACCEPT_COUNT	= 100,
	};

protected:

	Iocp											iocp_;
	PacketProcess*									contentLogicProcess_ = nullptr;

	NetworkDepartmentManager						netDeptManger_;
		
public:

	Server(PacketProcess* contentLogicProcess);
	~Server();

	Bool			Setup(SessionPool* sessionPool);
	Void			Run();
	Void			ShutDown();

private:

	Bool startup();
};