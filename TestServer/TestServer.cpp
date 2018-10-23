// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

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
		sessionPool_ = new SessionPool();
		if (false == Server::Setup(sessionPool_))
		{
			return false;
		}

		return true;
	}

	~TestServer() 
	{
		delete sessionPool_;
	}
};


class TestLogicProcess : public PacketProcess
{
public:
	
	TestLogicProcess() 
	{
		RegisterHandler();
	}
	~TestLogicProcess() {}
	
private:

	Void RegisterHandler() final
	{
		handlerTable_.emplace(100, &TestLogicProcess::handler_Msg_Process);
	}

	static Void	handler_Msg_Process(Byte* buffer, Int bytes)
	{
		MessagePacket* msg = reinterpret_cast< MessagePacket*>(buffer);

		std::cout << msg->val << std::endl;
	}

};

int main()
{

	std::shared_ptr<TestServer> server = std::make_shared<TestServer>(new TestLogicProcess());
	
	if (false == server->Setup())
	{
		return -1;
	}

	server->Run();

	Sleep(INFINITE);

    return 0;
}

