
#include "stdafx.h"

class TestServer : public Server
{

	SessionPool*			sessionPool_;

public:
	TestServer(PacketProcess* contentLogicProcess)
		: Server(contentLogicProcess)
	{

	}

	Bool	Setup() 
	{
		sessionPool_ = new SessionPool;
		if (false == Server::Setup(sessionPool_))
		{
			return false;
		}

		return true;
	}

	~TestServer(){}
};


struct Data
{
	int a;
};

int main()
{
	std::shared_ptr<TestServer> server = std::make_shared<TestServer>(nullptr);
	
	if (false == server->Setup())
	{
		return -1;
	}

	server->Run();

	Sleep(INFINITE);
    return 0;
}

