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
	virtual ~Server();

	Bool					Setup(SessionPool* sessionPool);
	virtual Void			Run();
	virtual Void			ShutDown();

private:

	Bool startup();
};