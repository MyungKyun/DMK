#include "stdafx.h"
#include "TestServerConfig.h"


Bool	TestServerConfig::Load(const WString& file)
{
	Parser::Json json;
	if (false == json.Read(file))
	{
		LOG_ERROR(L" Could not Read File. FileName => {}", file.c_str());
		return false;
	}

	auto& doc = json.GetDocument();
	Parser::Json::Value	root = doc["TestServer"];
	serverId_ = root["ServerId"].GetInt();
	auto tempIp  = root["Ip"].GetString();

	WChar wip[64] = { 0, };
	ConvertCharToWide(tempIp, wip, _countof(wip));
	ip_ = wip;

	Parser::Json::Value acceptGorup = root["AcceptGroup"];
	clientAcceptCount_ = acceptGorup["ClientAccept"].GetInt();
	gameServerAcceptCount_ = acceptGorup["ServerAccept"].GetInt();
	
	Parser::Json::Value portGroup = root["PortGroup"];
	clientPort_ = portGroup["ClientPort"].GetInt();
	gameServerPort_ = portGroup["ServerPort"].GetInt();

	Parser::Json::Value threadCounts = root["Thread"];
	ioThreadCount_ = threadCounts["IoCount"].GetInt();
	dbThreadCount_ = threadCounts["DBCount"].GetInt();

	return true;
}

Int		TestServerConfig::GetServerId()
{
	return serverId_;
}

Int		TestServerConfig::GetClientPort()
{
	return clientPort_;
}

Int		TestServerConfig::GetGameServerPort()
{
	return gameServerPort_;
}

Int		TestServerConfig::GetIoThreadCount()
{
	return ioThreadCount_;
}

Int		TestServerConfig::GetDBThreadCount()
{
	return dbThreadCount_;
}

Int			TestServerConfig::GetClientCount()
{
	return clientAcceptCount_;
}

Int		TestServerConfig::GetServerCount()
{
	return gameServerAcceptCount_;
}

WString	TestServerConfig::GetIp()
{
	return ip_;
}