#pragma once

class Server 
{
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