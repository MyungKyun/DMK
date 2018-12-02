#include "stdafx.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <filesystem>

AsyncLog::AsyncLog(const std::string& loggerName, spdlog::sinks_init_list sinksInitList)
	: spdlog::logger(loggerName, sinksInitList.begin(), sinksInitList.end())
{

}


Void AsyncLog::flush_()
{
	AsyncLogMsg lmsg;
	while (msgQue_.try_pop(lmsg))
	{
		spdlog::details::log_msg  copymsg;
		lmsg.FillupLogMsg(copymsg, const_cast<String*>(&name_));
		

		for (auto& sink : sinks_)
		{
			if (sink->should_log(copymsg.level))
			{
				try
				{
					sink->log(copymsg);
				}

				SPDLOG_CATCH_AND_HANDLE
			}
		}

		for (auto& sink : sinks_)
		{
			try
			{
				sink->flush();
			}

			SPDLOG_CATCH_AND_HANDLE
		}
	}
}

Void AsyncLog::sink_it_(spdlog::details::log_msg& msg)
{
	try
	{
		msgQue_.push(std::move(AsyncLogMsg(msg)));
	}
	catch (const std::exception& e)
	{
		default_err_handler_(e.what());
	}
	catch (...)
	{
		default_err_handler_("Unknown Exception in AsyncLog" + name_);
		throw;
	}
}


//////////////////////////////////////////////////////////////////////////////
//
// class Logger

Logger::~Logger()
{
	stop_ = true;
	thread_.join();
}

Bool Logger::Setup(const String& filePath)
{
	auto fullPath = std::experimental::filesystem::path(filePath);
	if (fullPath.empty())
	{
		return false;
	}
	
	auto fileName = fullPath.filename().string();
	fileName = fileName.substr(0, fileName.find_last_of('.'));

	auto path = fullPath.parent_path();
	
	try
	{
		path += "\\Logs";
		std::experimental::filesystem::create_directory(path);
		path += ("\\" + fileName);
		std::experimental::filesystem::create_directory(path);
		path += "\\";
	}
	catch (std::exception& e)
	{
		e.what();
		return false;
	}

	auto stdoutSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_st>();
	auto stringFilePath = path.string() + fileName;
	std::array<wchar_t, 64> buf;
	
	ConvertCharToWide(stringFilePath.c_str(), buf.data(), 64);
	WString	saveLogFilePath(buf.data());
	saveLogFilePath += L".log";
	
	//뒤에 0 은 무시하자.. 로그 파일을 얼마나 만들지 개수인데, 아무숫자나 넣어도 무한대로 만들게 수정해두었다.
	auto rotatingSink = std::make_shared<spdlog::sinks::rotating_file_sink_st>(saveLogFilePath, LOG_FILE_SIZE, 0);

	stdoutSink->set_level(spdlog::level::trace);
	rotatingSink->set_level(spdlog::level::trace);

	auto logger = std::make_shared<AsyncLog>("AsyncLogger", spdlog::sinks_init_list{ stdoutSink, rotatingSink });
	logger->set_level(spdlog::level::trace);
	logger->set_pattern("%^[%Y-%m-%d %H:%M:%S][%l][tid:%t][%s,%#] %v%$");

	logger_ = logger;
	thread_ = std::thread(&Logger::Flush, this);

	return true;
}

Void Logger::Flush()
{
	while (false == stop_)
	{
		logger_->flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}