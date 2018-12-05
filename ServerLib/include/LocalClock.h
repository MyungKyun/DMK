#pragma once
#include "stdafx.h"
#include <sqltypes.h>

class LocalClock
{
	time_point_t	now_;
	second_t		seconds_;
	milli_t			milliseconds_;
	milli_t			diff_;

public:

	static LocalClock Now()
	{
		auto now = std::chrono::system_clock::now();
		auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		auto sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
		auto diff = milli - sec;

		return LocalClock(now, sec, milli, diff);
	}

	static void	SystemTimeToTm(const SYSTEMTIME& st, std::tm& out)
	{
		out.tm_year = st.wYear - 1900;
		out.tm_mon = st.wMonth - 1;
		out.tm_mday = st.wDay;
		out.tm_hour = st.wHour;
		out.tm_min = st.wMinute;
		out.tm_sec = st.wSecond;
		out.tm_wday = st.wDayOfWeek;
		out.tm_yday = 0;
		out.tm_isdst = -1;
	}

	static void TimeStampToTm(const TIMESTAMP_STRUCT& ts, std::tm& out)
	{
		out.tm_year = ts.year - 1900;
		out.tm_mon = ts.month - 1;
		out.tm_mday = ts.day;
		out.tm_hour = ts.hour;
		out.tm_min = ts.minute;
		out.tm_sec = ts.second;
		out.tm_wday = 0;
		out.tm_yday = 0;
		out.tm_isdst = -1;
	}

	static void TmToSystemTime(const std::tm& tm, SYSTEMTIME& out)
	{
		out.wYear = static_cast<WORD>(tm.tm_year + 1900);
		out.wMonth = static_cast<WORD>(tm.tm_mon + 1);
		out.wDay = static_cast<WORD>(tm.tm_mday);
		out.wHour = static_cast<WORD>(tm.tm_hour);
		out.wMinute = static_cast<WORD>(tm.tm_min);
		out.wSecond = static_cast<WORD>(tm.tm_sec);
		out.wMilliseconds = 0;
		out.wDayOfWeek = static_cast<WORD>(tm.tm_wday);
	}

	static void TmToTimeStamp(const std::tm& tm, TIMESTAMP_STRUCT& ts)
	{
		ts.year = static_cast<SQLSMALLINT>(tm.tm_year + 1900);
		ts.month = static_cast<SQLSMALLINT>(tm.tm_mon + 1);
		ts.day = static_cast<SQLSMALLINT>(tm.tm_mday);
		ts.hour = static_cast<SQLSMALLINT>(tm.tm_hour);
		ts.minute = static_cast<SQLSMALLINT>(tm.tm_min);
		ts.second = static_cast<SQLSMALLINT>(tm.tm_sec);
		ts.fraction = 0;
	}


	LocalClock(time_point_t& now, second_t& sec, milli_t& milli, milli_t& diff)
	{
		now_ = now;
		seconds_ = sec;
		milliseconds_ = milli;
		diff_ = diff;
	}

	explicit LocalClock(long long seconds)
	{
		seconds_ = second_t(seconds);
		milliseconds_ = milli_t(0);
	}

	explicit LocalClock(std::tm* tm)
	{
		auto tt = ::mktime(tm);
		seconds_ = std::chrono::duration_cast<second_t>(std::chrono::system_clock::from_time_t(tt).time_since_epoch());
		milliseconds_ = milli_t(0);
	}

	explicit LocalClock(std::tm& tm)
	{
		auto tt = ::mktime(&tm);
		seconds_ = std::chrono::duration_cast<second_t>(std::chrono::system_clock::from_time_t(tt).time_since_epoch());
		milliseconds_ = milli_t(0);
	}
	
	std::tm	GetTm()
	{
		std::tm tm;
		auto tt = static_cast<time_t>(seconds_.count());
		::localtime_s(&tm, &tt);

		return tm;
	}

	LocalClock	Tomorrow()
	{
		auto tm = GetTm();
		tm.tm_mday += 1;
		return LocalClock(&tm);
	}

	LocalClock	Yesterday()
	{
		auto tm = GetTm();
		tm.tm_mday -= 1;
		return LocalClock(&tm);
	}

	std::wstring	ToString()
	{
		auto st = ToSystemTime();
		std::array<wchar_t, 64>	buf;
		swprintf_s(buf.data(), buf.size(), L"%04u-%02u-%02u %02u:%02u:%02u",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		return buf.data();
	}

	std::wstring	DateToString()
	{
		auto st = ToSystemTime();
		std::array<wchar_t, 64>	buf;
		swprintf_s(buf.data(), buf.size(), L"%04u-%02u-%02u", st.wYear, st.wMonth, st.wDay);
		return buf.data();
	}

	std::wstring	TimeToString()
	{
		auto st = ToSystemTime();
		std::array<wchar_t, 64> buf;
		swprintf_s(buf.data(), buf.size(), L"%02u:%02u:%02u", st.wHour, st.wMinute, st.wSecond);
		return buf.data();
	}

	std::wstring	DateTimeToString()
	{
		auto st = ToSystemTime();
		std::array<wchar_t, 64>	buf;
		swprintf_s(buf.data(), buf.size(), L"%04u_%02u_%02u_%02u_%02u_%02u_%llu",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, GetDiffseconds());
		return buf.data();
	}

	SYSTEMTIME	ToSystemTime()
	{
		auto tm = GetTm();

		SYSTEMTIME sys;
		sys.wYear = static_cast<WORD>(tm.tm_year + 1900);
		sys.wMonth = static_cast<WORD>(tm.tm_mon + 1);
		sys.wDay = static_cast<WORD>(tm.tm_mday);
		sys.wHour = static_cast<WORD>(tm.tm_hour);
		sys.wMinute = static_cast<WORD>(tm.tm_min);
		sys.wSecond = static_cast<WORD>(tm.tm_sec);
		sys.wMilliseconds = 0;
		sys.wDayOfWeek = static_cast<WORD>(tm.tm_wday);

		return sys;
	}

	TIMESTAMP_STRUCT ToTimeStamp()
	{
		auto tm = GetTm();

		TIMESTAMP_STRUCT ts;

		ts.year = static_cast<SQLSMALLINT>(tm.tm_year + 1900);
		ts.month = static_cast<SQLSMALLINT>(tm.tm_mon + 1);
		ts.day = static_cast<SQLSMALLINT>(tm.tm_mday);
		ts.hour = static_cast<SQLSMALLINT>(tm.tm_hour);
		ts.minute = static_cast<SQLSMALLINT>(tm.tm_min);
		ts.second = static_cast<SQLSMALLINT>(tm.tm_sec);
		ts.fraction = 0;

		return ts;
	}

	second_t GetSourceSeconds()			{ return seconds_; }
	milli_t	 GetSourceMilliseconds()	{ return milliseconds_; }

	long long GetSeconds() { return seconds_.count(); }
	long long GetMilliseconds() { return milliseconds_.count(); }
	long long GetDiffseconds() { return diff_.count(); }	// now 타임이 포함된 생성자가 호출되었을때 사용해야함

};
