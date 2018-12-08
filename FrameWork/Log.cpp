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
	using namespace std::experimental;

	auto fullPath = filesystem::path(filePath);
	if (fullPath.empty())
	{
		return false;
	}

	auto fileName = fullPath.filename().wstring();
	fileName = fileName.substr(0, fileName.find_last_of('.'));

	WChar curDir[256] = { 0, };
	::GetCurrentDirectoryW(256, curDir);

	auto curLogPath = filesystem::current_path().wstring();
	curLogPath += L"\\Logs";

	auto logDirectoryPath = filesystem::path(curLogPath);

	if (false == filesystem::exists(logDirectoryPath))
	{
		if (false == filesystem::create_directory(logDirectoryPath))
		{
			return false;
		}
	}
	
	auto date = LocalClock::Now().DateToString();

	auto srcLogPath = curLogPath + L"\\" + fileName;

	auto curDateLogPath = srcLogPath + L"_" + date;

	auto logFileNameDirectoryPath = filesystem::path(curDateLogPath);

	if (false == filesystem::exists(logFileNameDirectoryPath))
	{
		if (false == filesystem::create_directory(logFileNameDirectoryPath))
		{
			return false;
		}
	}

	auto finalLogPath = logFileNameDirectoryPath.wstring();
	finalLogPath += L"\\";
	finalLogPath += fileName;
	finalLogPath += L".log";

	

	auto stdoutSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_st>();
	auto rotatingSink = std::make_shared<spdlog::sinks::rotating_file_sink_st>(finalLogPath, 100, srcLogPath, curDateLogPath, fileName);

#ifdef _DEBUG
	stdoutSink->set_level(spdlog::level::trace);
	rotatingSink->set_level(spdlog::level::trace);
#else
	rotatingSink->set_level(spdlog::level::trace);
#endif

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