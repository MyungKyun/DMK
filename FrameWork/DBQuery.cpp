#include "stdafx.h"

DBQuery::DBQuery()
	: queryStatement_(L"")
{

}

DBQuery::DBQuery(const WString& queryStatement)
	: queryStatement_(queryStatement)
{

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

	queryStatement_.clear();
	params_.clear();
}

Bool		DBQuery::Execute(DBConnection* const connection)
{
	if (nullptr == connection)
	{
		return false;
	}

	hstmt_ = connection->GetHstmt();
	
	if (false == preparingParams(connection))
	{
		LOG_ERROR(L"DBQuery Execute => bindParams Failed.");
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

Bool		DBQuery::preparingParams(DBConnection* const connection)
{
	if (params_.size() <= 0)
	{
		return false;
	}

	if (hstmt_ == NULL)
	{
		return false;
	}

	SQLUSMALLINT index = 1;
	for (auto& p : params_)
	{
		std::wcout << *(static_cast<wchar_t*>(p.paramValuePtr_)+1) << std::endl;

		auto ret = ::SQLBindParameter(hstmt_, index++, p.ioType_, p.cType_, p.sType_, p.columnSize_,
			p.decimalDigits_, p.paramValuePtr_, p.paramValueSize_, &p.strLen_or_lnd_);

		if (false == DBErrorMessage::CheckError(connection, SQL_HANDLE_STMT, hstmt_, ret))
		{
			return false;
		}
	}

	return true;
}
