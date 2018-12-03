#include "stdafx.h"

DBQuery::DBQuery()
	: queryStatement_(L"")
	, paramIdx_(1)
	, columnIdx_(1)
{

}

DBQuery::DBQuery(const WString& queryStatement)
	: queryStatement_(L"")
	, paramIdx_(1)
	, columnIdx_(1)
{
	queryStatement_ = L"{call " + queryStatement + L"}";
}

DBQuery::~DBQuery()
{
	if (hstmt_ != SQL_NULL_HSTMT)
	{
		SQLRETURN ret = ::SQLCloseCursor(hstmt_);
		try
		{
			ret = ::SQLFreeStmt(hstmt_, SQL_UNBIND);
			ret = ::SQLFreeStmt(hstmt_, SQL_RESET_PARAMS);
			ret = ::SQLFreeStmt(hstmt_, SQL_CLOSE);
		}
		catch (std::exception& e)
		{
			LOG_ERROR("SQLFreeStmt{0}", e.what());
		}
	}

	paramIdx_ = 1;
	columnIdx_ = 1;
	queryStatement_.clear();
}

Bool		DBQuery::Execute(DBConnection* const connection)
{
	if (nullptr == connection)
	{
		return false;
	}
	
	if (false == PreparingParams(connection))
	{
		return false;
	}

	// query execute
	SQLRETURN ret = ::SQLExecDirectW(hstmt_, const_cast<WChar*>(queryStatement_.c_str()), SQL_NTSL);
	if (ret != SQL_NO_DATA && false == DBErrorMessage::CheckError(connection, SQL_HANDLE_STMT, hstmt_, ret))
	{
		LOG_ERROR(L"Query Execute Failed. Query:{0}", queryStatement_.c_str());
		return false;
	}
	
	SQLSMALLINT count = 0;
	ret = ::SQLNumResultCols(hstmt_, &count);
	if (ret != SQL_SUCCESS)
	{
		return false;
	}

	ret = ::SQLFetch(hstmt_);

	if (false == GetColumnData())
	{
		LOG_ERROR(L"GetColumnData() Failed. Query:{}", queryStatement_.c_str());
		return false;
	}
	
	return true;
}

Bool		DBQuery::PreparingParams(DBConnection* const connection)
{
	hstmt_ = connection->GetHstmt();

	if (hstmt_ == nullptr)
	{
		LOG_ERROR(L"SQL STMT is NULL");
		return false;
	}

	return true;
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, Char& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &val, sizeof(Short), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, Byte& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &val, sizeof(Short), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, Short& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &val, sizeof(Short), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, UShort& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &val, sizeof(UShort), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, Int& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_SLONG, SQL_INTEGER, 0, 0, &val, sizeof(Int), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, UInt& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_ULONG, SQL_INTEGER, 0, 0, &val, sizeof(UInt), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, Int64& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &val, sizeof(Int64), nullptr);
}

SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, UInt64& val)
{
	return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &val, sizeof(UInt64), nullptr);
}


SQLRETURN DBQuery::BindParam(SQLSMALLINT ioType, WChar* val, Int len)
{
	SQLLEN strlen_or_lnd = 0;
	if (ioType == SQL_PARAM_INPUT)
	{
		strlen_or_lnd = SQL_NTSL;
		return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, val, 0, &strlen_or_lnd);
	}
	else
	{
		return ::SQLBindParameter(hstmt_, paramIdx_++, ioType, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, val, len, nullptr);
	}
}

//////////////////////////////////////
// GetColumnData

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, Char& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(Char), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, Byte& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(Byte), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, Short& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(Short), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, UShort& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(UShort), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, Int& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(Int), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, UInt& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(UInt), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, Int64& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(Int64), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, UInt64& out)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)&out, sizeof(UInt64), &strlen_or_lnd);
}

SQLRETURN DBQuery::GetColData(SQLSMALLINT cType, WChar* out, Int strSize)
{
	SQLLEN strlen_or_lnd;
	return ::SQLGetData(hstmt_, columnIdx_++, cType, (SQLPOINTER)out, strSize, &strlen_or_lnd);
}