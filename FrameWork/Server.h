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
	Dispatcher*										testDispatcher_;
	NetworkDepartmentManager						netDeptManger_;
		
public:

	Server(Dispatcher* packetDispatcher);
	~Server();

	Bool			Setup(SessionPool* sessionPool);
	Void			Run();
	Void			ShutDown();

private:

	Bool startup();
};