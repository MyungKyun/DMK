#pragma once
#define SPDLOG_WCHAR_FILENAMES
class AsyncLog : public spdlog::logger
{
	
	concurrency::concurrent_queue<AsyncLogMsg>  msgQue_;

protected:
	
	Void sink_it_(spdlog::details::log_msg& msg) override;
	Void flush_() override;

public:
	
	AsyncLog(const std::string& loggerName, spdlog::sinks_init_list sinksInitList);
	
};


class Logger : public Singleton<Logger>
{
	friend class Singleton<Logger>;
	std::thread						thread_;
	std::atomic_bool				stop_;
	std::shared_ptr<spdlog::logger> logger_;

	enum
	{
		LOG_FILE_SIZE = 1024 * 1024 * 3,
		MAX_LOG_FILE_COUNT	= 9999
	};

public:

	~Logger();
	Bool		Setup(const String& filePath);
	Void		Flush();

	template<typename ...Args>
	Void	Trace(Args... args) { logger_->trace(args...); }

	template<typename ...Args>
	Void	Info(Args... args)  { logger_->info(args...); }

	template<typename ...Args>
	Void	Debug(Args...args)  { logger_->debug(args...); }

	template<typename ...Args>
	Void	Warn(Args... args)  { logger_->warn(args...); }

	template<typename ...Args>
	Void	Error(Args... args) { logger_->error(args...); }

	template<typename ...Args>
	Void	Critic(Args... args) { logger_->critical(args...); }
};

#define		__FILENAME__		(strrchr(__FILE__,'\\') ? strrchr(__FILE__,'\\') + 1 : __FILE__)
#define		LOG_ERROR(...)		Singleton<Logger>::GetInstance().Error(__FILENAME__, __LINE__, __VA_ARGS__)
#define		LOG_DEBUG(...)		Singleton<Logger>::GetInstance().Debug(__FILENAME__, __LINE__, __VA_ARGS__)
#define		LOG_INFO(...)		Singleton<Logger>::GetInstance().Info(__FILENAME__, __LINE__, __VA_ARGS__)
#define		LOG_CRITIC(...)		Singleton<Logger>::GetInstance().Critic(__FILENAME__, __LINE__, __VA_ARGS__)
#define		LOG_WARN(...)		Singleton<Logger>::GetInstance().Warn(__FILENAME__, __LINE__, __VA_ARGS__)
#define		LOG_TRACE(...)		Singleton<Logger>::GetInstance().Trace(__FILENAME__, __LINE__, __VA_ARGS__)
#define		GLogger				Singleton<Logger>::GetInstance()