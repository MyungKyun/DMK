#pragma once
#include <sql.h>
#include <sqlext.h>

class DBConnection
{
	SQLHDBC			hdbc_;
	SQLHSTMT		hstmt_;

	//임시 커넥션 스트링
	WString			connectionStr_;

public:

	DBConnection();
	~DBConnection();

	Bool		Connect(SQLHENV	henv);
	Void		Disconnect();
};


class DBEnv : public Singleton<DBEnv>
{

	SQLHENV		henv_;

public:
	DBEnv() 
	{
		SQLRETURN ret = ::SQLAllocEnv(&henv_);
		if (false == DBErrorMessage::CheckError(SQL_HANDLE_ENV, henv_, ret))
		{
			LOG_CRITIC(L"SQL_HANDLE_ENV allocate falied.");
			assert(false);
		}

		ret = ::SQLSetEnvAttr(henv_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

		Int version = 0;
		ret = ::SQLGetEnvAttr(henv_, SQL_ATTR_ODBC_VERSION, &version, 0, 0);
		if (false == DBErrorMessage::CheckError(SQL_HANDLE_ENV, henv_, ret))
		{
			LOG_INFO(L"SQLSetEnv Failed. : SQL_ODBC_VERSION=>{0}", version);
		}
	}
	~DBEnv() {}

	SQLHENV GetEnv()
	{
		return henv_;
	}

};
