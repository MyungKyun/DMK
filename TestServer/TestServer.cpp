// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "playerInfo_generated.h"
#include "QueryTest.h"


class DBThread : public Thread
{

	DBConnection*	connection_;
	
public:

	static Short	GetDepartmentNumber()
	{
		return DB_PROCESSING_DEPT;
	}

	DBThread(DWORD id, Iocp* iocp, const String& name)
		: Thread(id, name, DB_PROCESSING_DEPT)
		, connection_(nullptr)
	{

	}

	Bool Setup() final
	{
		if (false == __super::Setup())
		{
			return false;
		}
		
		connection_ = new DBConnection();
		if (nullptr == connection_)
		{
			return false;
		}

		connection_->Connect(Singleton<DBEnv>::GetInstance().GetEnv());
		return true;
	}

	Void Run() final
	{

	}

	Void ShutDown() final
	{

	}

};

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

		GThreadManager.AddDepartment(NETWORK_IO_PROCESSING_DEPT, &iocp_);
		GThreadManager.Start<NetworkIoThread>(0, "NetworkIoThread");
		
		//GThreadManager.AddDepartment(DB_PROCESSING_DEPT, nullptr);
		//GThreadManager.Start<DBThread>(5, "DBThread");

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
		auto name = playerdata->name()->c_str();
		wchar_t name_[20] = { 0, };
		ConvertCharToWide(const_cast<char*>(name), name_, strlen(name));
		LOG_INFO(L"[Server] recv data :: name : {0}, level : {1} ", name_, level);
		
		MksBuilder echoBuilder;
		auto echoData = CreatePlayerInfo(echoBuilder.Get(), echoBuilder.Get().CreateString(playerdata->name()), playerdata->level());
		echoBuilder.Get().Finish(echoData);

		SendToSession(session, 100, &echoBuilder);

		auto now = LocalClock::Now().ToString();

		return true;
	}
};

int main(int argc, CHAR* argv[])
{
	
	GLogger.Setup(argv[0]);
		
	//LOG_INFO(L"..........Test Server Start..........");
	//LOG_INFO(L"Start Test Logging...");

	/////////// DB Connection Test////////////////////////
	// 연결이 정상적으로 이루어진다. 우선 연결만 확인.
	DBConnection con;
	con.Connect(Singleton<DBEnv>::GetInstance().GetEnv());
	WString oldId(L"k2345");
	WString newId(L"lkju45");
	   
	TestStringQuery query(oldId, newId);
	query.Execute(&con);
	//////////////////////////////////////////////////////
	
	std::shared_ptr<TestServer> server = std::make_shared<TestServer>(new TestPacketDispathcer);

	if (false == server->Setup())
	{
		return -1;
	}

	server->Run();

	Sleep(INFINITE);

    return 0;
}

