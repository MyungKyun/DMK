#pragma once

class TestServerConfig
{

	Int			serverId_;
	WString		ip_;
	Int			clientPort_;
	Int			gameServerPort_;
	Int			ioThreadCount_;
	Int			dbThreadCount_;
	Int			clientAcceptCount_;
	Int			gameServerAcceptCount_;

public:

	TestServerConfig() = default;
	~TestServerConfig() = default;

	Bool	Load(const WString& file);

	Int			GetServerId();
	Int			GetClientCount();
	Int			GetServerCount();
	Int			GetClientPort();
	Int			GetGameServerPort();
	Int			GetIoThreadCount();
	Int			GetDBThreadCount();
	WString		GetIp();
};
