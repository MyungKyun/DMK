#pragma once
#include "spdlog/spdlog.h"

class AsyncLogMsg
{

	enum AsyncMsgType
	{
		LOG,
	};


	spdlog::level::level_enum				logLevel_;
	DWORD									threadId_;
	std::chrono::system_clock::time_point	time_;
	String									text_;
	UDLong									msgId_;
	AsyncMsgType							msgType_;
	std::string								fileName_;
	UInt									fileLine_;

public:

	AsyncLogMsg() {}
	AsyncLogMsg(AsyncLogMsg&&) = default;
	AsyncLogMsg& operator=(AsyncLogMsg&& rhs) = default;
	AsyncLogMsg(const AsyncLogMsg&) = default;
	AsyncLogMsg& operator=(const AsyncLogMsg& rhs) = default;

	~AsyncLogMsg() {}
	AsyncLogMsg(AsyncMsgType msgType);
	AsyncLogMsg(const spdlog::details::log_msg& lmsg);
	

	Void FillupLogMsg(spdlog::details::log_msg& msg, String* loggerName);

	spdlog::level::level_enum GetLogLevel() { return logLevel_; }
};