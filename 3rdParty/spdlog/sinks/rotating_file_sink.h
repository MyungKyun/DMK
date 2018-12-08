//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#ifndef SPDLOG_H
#error "spdlog.h must be included before this file."
#endif

#include "spdlog/details/file_helper.h"
#include "spdlog/details/null_mutex.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/sinks/base_sink.h"

#include <cerrno>
#include <chrono>
#include <ctime>
#include <mutex>
#include <string>
#include <tuple>

namespace spdlog {
namespace sinks {

//
// Rotating file sink based on size
//
template<typename Mutex>
class rotating_file_sink final : public base_sink<Mutex>
{
public:
    rotating_file_sink(filename_t base_filename, std::size_t max_size, std::size_t max_files)
        : base_filename_(std::move(base_filename))
        , max_size_(max_size)
        , max_files_(max_files)
    {
        file_helper_.open(calc_filename(base_filename_, 0));
        current_size_ = file_helper_.size(); // expensive. called only once
    }

	rotating_file_sink(filename_t base_filename, std::size_t max_size, std::wstring sourceDirPath, std::wstring curDateDirPath, std::wstring fileName)
		: base_filename_(std::move(base_filename))
		, max_size_(max_size)
		, max_files_(0)
		, srcDirPath_(std::move(sourceDirPath))
		, dateDirPath_(std::move(curDateDirPath))
		, fileName_(std::move(fileName))
	{
		file_helper_.open(calc_filename(base_filename_, 0));
		current_size_ = file_helper_.size(); // expensive. called only once		
		nextRotationTime_ = nextRotationTime();

		time_t time_tomorrow = std::chrono::system_clock::to_time_t(nextRotationTime_);
		std::tm tomorrowDate = spdlog::details::os::localtime(time_tomorrow);

	}

    // calc filename according to index and file extension if exists.
    // e.g. calc_filename("logs/mylog.txt, 3) => "logs/mylog.3.txt".
    static filename_t calc_filename(const filename_t &filename, std::size_t index)
    {
		auto dateTime = LocalClock::Now().DateTimeToString();
        typename std::conditional<std::is_same<filename_t::value_type, char>::value, fmt::memory_buffer, fmt::wmemory_buffer>::type w;

		filename_t basename, ext;
		std::tie(basename, ext) = details::file_helper::split_by_extenstion(filename);
		basename += (L"_" + dateTime);
		fmt::format_to(w, SPDLOG_FILENAME_T("{}{}"), basename, ext);


   //     if (index != 0u)
   //     {
   //         filename_t basename, ext;
   //         std::tie(basename, ext) = details::file_helper::split_by_extenstion(filename);
			////basename += (L"_" + dateTime);
   //         fmt::format_to(w, SPDLOG_FILENAME_T("{}.{}{}"), basename, index, ext);
   //     }
   //     else
   //     {
			////filename_t basename, ext;
			////std::tie(basename,ext) = details::file_helper::split_by_extenstion(filename);
			////basename += (L"_" + dateTime);
   //         fmt::format_to(w, SPDLOG_FILENAME_T("{}"), filename);
   //     }
        return fmt::to_string(w);
    }

protected:
    void sink_it_(const details::log_msg &msg) override
    {
        fmt::memory_buffer formatted;
        sink::formatter_->format(msg, formatted);
        current_size_ += formatted.size();

		auto now = std::chrono::system_clock::now();
		if (now >= nextRotationTime_)
		{
			rotate_dirctory_file_();
			current_size_ = formatted.size();
			nextRotationTime_ = nextRotationTime();
		}
        else if ( current_size_ > max_size_)
        {
            rotate_();
            current_size_ = formatted.size();
        }
        file_helper_.write(formatted);
    }

    void flush_() override
    {
        file_helper_.flush();
    }

private:
    // Rotate files:
    // log.txt -> log.1.txt
    // log.1.txt -> log.2.txt
    // log.2.txt -> log.3.txt
    // log.3.txt -> delete
    void rotate_()
    {
		using namespace std::experimental;
        using details::os::filename_to_str;
		
        file_helper_.close();			   		
		filename_t target = calc_filename(base_filename_, 0);
		
        //for (auto i = max_files_; i > 0; --i)
        //{
        //    filename_t src = calc_filename(base_filename_, i - 1);
        //    if (!details::file_helper::file_exists(src))
        //    {
        //        continue;
        //    }
        //    filename_t target = calc_filename(base_filename_, i);

        //    if (!rename_file(src, target))
        //    {
        //        // if failed try again after a small delay.
        //        // this is a workaround to a windows issue, where very high rotation
        //        // rates can cause the rename to fail with permission denied (because of antivirus?).
        //        details::os::sleep_for_millis(100);
        //        if (!rename_file(src, target))
        //        {
        //            file_helper_.reopen(true); // truncate the log file anyway to prevent it to grow beyond its limit!
        //            current_size_ = 0;
        //            throw spdlog_ex(
        //                "rotating_file_sink: failed renaming " + filename_to_str(src) + " to " + filename_to_str(target), errno);
        //        }
        //    }
        //}
		
       // file_helper_.reopen(true);

		file_helper_.reopen(target, true);
    }

	void rotate_dirctory_file_()
	{
		using namespace std::experimental;
		using details::os::filename_to_str;
		file_helper_.close();

		auto curPath = filesystem::current_path();

		// 디렉터리 날짜
		auto now = std::chrono::system_clock::now();
		auto sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
		std::tm nowTm;
		time_t tt = static_cast<time_t>(sec.count());
		::localtime_s(&nowTm, &tt);

		std::array<wchar_t, 64> buf;
		swprintf_s(buf.data(), buf.size(), L"%04u-%02u-%02u", nowTm.tm_year + 1900, nowTm.tm_mon + 1, nowTm.tm_mday);

		std::wstring newDate(buf.data());
		auto newPath = srcDirPath_ + L"_" + newDate;
		auto newDirPath = filesystem::path(newPath);
		auto oldDirPaht = filesystem::path(dateDirPath_);

		if (false == filesystem::equivalent(oldDirPaht, newDirPath))
		{
			if (false == filesystem::create_directory(newDirPath))
			{
				LOG_ERROR(L"Log directory create failed. Path => {}", newDirPath.wstring().c_str());
				return;
			}

			newPath += L"\\" + fileName_ + L".log";
			base_filename_ = newPath;
			dateDirPath_ = newDirPath;
		}

		filename_t target = calc_filename(base_filename_, 0);

		file_helper_.reopen(target, true);
	}

	std::chrono::system_clock::time_point	nextRotationTime()
	{
		auto now = std::chrono::system_clock::now();
		//time_t tnow = std::chrono::system_clock::to_time_t(now);
		//std::tm date = spdlog::details::os::localtime(tnow);

		auto tomorrow = now + std::chrono::hours(24);
		time_t time_tomorrow = std::chrono::system_clock::to_time_t(tomorrow);
		std::tm tomorrowDate = spdlog::details::os::localtime(time_tomorrow);
		tomorrowDate.tm_hour = 0;
		tomorrowDate.tm_min = 0;
		tomorrowDate.tm_sec = 0;
		

		auto rotation_time = std::chrono::system_clock::from_time_t(std::mktime(&tomorrowDate));
		if (rotation_time > now)
		{
			return rotation_time;
		}

		return rotation_time + std::chrono::hours(24);
	}

    // delete the target if exists, and rename the src file  to target
    // return true on success, false otherwise.
    bool rename_file(const filename_t &src_filename, const filename_t &target_filename)
    {
        // try to delete the target file in case it already exists.
        (void)details::os::remove(target_filename);
        return details::os::rename(src_filename, target_filename) == 0;
    }

	
	std::wstring	srcDirPath_, dateDirPath_, fileName_;
    filename_t base_filename_;
    std::size_t max_size_;
    std::size_t max_files_;
    std::size_t current_size_;
    details::file_helper file_helper_;
	std::chrono::system_clock::time_point	nextRotationTime_;
};

using rotating_file_sink_mt = rotating_file_sink<std::mutex>;
using rotating_file_sink_st = rotating_file_sink<details::null_mutex>;

} // namespace sinks

//
// factory functions
//

template<typename Factory = default_factory>
inline std::shared_ptr<logger> rotating_logger_mt(
    const std::string &logger_name, const filename_t &filename, size_t max_file_size, size_t max_files)
{
    return Factory::template create<sinks::rotating_file_sink_mt>(logger_name, filename, max_file_size, max_files);
}

template<typename Factory = default_factory>
inline std::shared_ptr<logger> rotating_logger_st(
    const std::string &logger_name, const filename_t &filename, size_t max_file_size, size_t max_files)
{
    return Factory::template create<sinks::rotating_file_sink_st>(logger_name, filename, max_file_size, max_files);
}
} // namespace spdlog
