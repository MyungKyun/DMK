#include "stdafx.h"

DBConnection::DBConnection()
{
	// TODO::파일에서 ConnectionString  관련 내용을 가져와 설정하게 하도록하자

	WString	Server(L"127.0.0.1");
	WString port(L"9500");
	WString db(L"GameDB");
	WString uid(L"testServer");
	WString pwd(L"1q2w3e4r");

	std::array<wchar_t, 128> buf;
	::swprintf_s(buf.data(), buf.size(), L"Driver={SQL Server Native Client 11.0};Server=%s,%s;DataBase=%s;Uid=%s;Pwd=%s", Server.c_str(), port.c_str(), db.c_str(), uid.c_str(), pwd.c_str());

	connectionStr_ = buf.data();
}

DBConnection::~DBConnection()
{

}


Bool DBConnection::Connect(SQLHENV henv)
{
	
	SQLRETURN ret = ::SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc_);
	if (false == DBErrorMessage::CheckError(nullptr, SQL_HANDLE_ENV, henv, ret))
	{
		Disconnect();
		return false;
	}

	SQLWCHAR outConnectStr[1024] = { 0, };
	SQLSMALLINT outConnectLen = 0;

	ret = ::SQLDriverConnectW(hdbc_, nullptr, (SQLWCHAR*)connectionStr_.c_str(), (SQLSMALLINT)connectionStr_.length(), 
		outConnectStr, _countof(outConnectStr), &outConnectLen, SQL_DRIVER_NOPROMPT);

	if (false == DBErrorMessage::CheckError(nullptr, SQL_HANDLE_DBC, hdbc_, ret))
	{
		Disconnect();
		return false;
	}

	ret = ::SQLAllocHandle(SQL_HANDLE_STMT, hdbc_, &hstmt_);
	if (false == DBErrorMessage::CheckError(nullptr, SQL_HANDLE_DBC, hdbc_, ret))
	{
		Disconnect();
		return false;
	}
	LOG_INFO(L"DBThreadID connect : tid{}, dept{}", LThreadId, LDeptNumber);

	return true;
}

Void DBConnection::Disconnect()
{
	if (hstmt_ != SQL_NULL_HSTMT)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, hstmt_);
		hstmt_ = SQL_NULL_HSTMT;
	}

	if (hdbc_ != SQL_NULL_HDBC)
	{
		::SQLFreeHandle(SQL_HANDLE_DBC, hdbc_);
		hdbc_ = SQL_NULL_HDBC;
	}
}


SQLHSTMT	DBConnection::GetHstmt() const
{
	return hstmt_;
}

