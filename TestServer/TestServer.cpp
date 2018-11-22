// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "playerInfo_generated.h"


class TestServer : public Server
{

	SessionPool*			sessionPool_;

public:
	TestServer(Dispatcher* packetDispatcher)
		: Server(packetDispatcher)
	{

	}

	Bool	Setup()
	{
		sessionPool_ = new SessionPool();
		
		if (false == Server::Setup(sessionPool_))
		{
			return false;
		}

		GThreadManager.AddDepartment(NetworkIoThread::GetDepartmentNumber(), &iocp_);
		GThreadManager.Start<NetworkIoThread>(0, "NetworkIoThread", &iocp_);


		return true;
	}

	~TestServer() 
	{
		delete sessionPool_;
	}
};

class TestPacketDispathcer : public Dispatcher
{
public:
	
	TestPacketDispathcer()
	{
		InitHandlers(&handler_default);

		Register(100, &handler_Msg_Process);
	}
	~TestPacketDispathcer() {}

private:

	static Bool  handler_default(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes)
	{
		return true;
	}
	
	static Bool	handler_Msg_Process(std::shared_ptr<Session> session, const PacketHeader* header,  const Byte* buf, Int transferredBytes)
	{
		auto playerdata = flatbuffers::GetRoot<PlayerInfo>(buf);

		Int level = playerdata->level();


		printf("[ %s,%d ]\n", playerdata->name()->c_str(), playerdata->level());

		MksBuilder echoBuilder;
		auto echoData = CreatePlayerInfo(echoBuilder.Get(), echoBuilder.Get().CreateString(playerdata->name()), playerdata->level());
		echoBuilder.Get().Finish(echoData);


		SendToSession(session, 100, &echoBuilder);
		return true;
	}
};





int main()
{

	std::shared_ptr<TestServer> server = std::make_shared<TestServer>(new TestPacketDispathcer);
	
	if (false == server->Setup())
	{
		return -1;
	}

	server->Run();

	Sleep(INFINITE);

    return 0;
}

