#include "stdafx.h"
#include "spdlog/spdlog.h"
#include "spdlog/details/log_msg.h"

AsyncLogMsg::AsyncLogMsg(const spdlog::details::log_msg& lmsg)
	: logLevel_(lmsg.level)
	, time_(lmsg.time)
	, fileName_(lmsg.source.filename)
	, fileLine_(lmsg.source.line)
	, msgId_(lmsg.msg_id)
	, msgType_(AsyncMsgType::LOG)
	, threadId_(static_cast<DWORD>(lmsg.thread_id))
	, text_(lmsg.payload.data(),lmsg.payload.size())
{

}


AsyncLogMsg::AsyncLogMsg(AsyncMsgType msgType)
	: logLevel_(spdlog::level::info)	
	, threadId_(0)
	, msgId_(0)
{

}

Void AsyncLogMsg::FillupLogMsg(spdlog::details::log_msg& msg, String* loggerName)
{
	msg.logger_name = loggerName;
	msg.time = time_;
	msg.thread_id = threadId_;
	msg.level = logLevel_;
	const_cast<spdlog::string_view_t&>(msg.payload) = text_;
	msg.msg_id = msgId_;
	msg.source.filename = fileName_.c_str();
	msg.source.line = fileLine_;
}


