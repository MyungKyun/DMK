#pragma once

class Server 
{
	enum Count 
	{
		//юс╫ц
		TOTAL_ACCEPT_COUNT	= 100,
	};

protected:

	WString											name_;
	Iocp											iocp_;
	NetworkDepartmentManager						netDeptManger_;
		
public:

	Server(const WString& name);
	virtual ~Server();

	Bool					Setup();
	virtual Void			Run();
	virtual Void			ShutDown();

private:

	Bool startup();
};