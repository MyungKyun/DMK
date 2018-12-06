#pragma once

class Server 
{
protected:

	WString											name_;
	Iocp											iocp_;
	NetworkDepartmentManager						netDeptManager_;
		
public:

	Server(const WString& name);
	virtual ~Server();

	Bool					Setup(Int argc, Char* argv[]);
	virtual Void			Run();
	virtual Void			ShutDown();

private:

	Bool startup();
};