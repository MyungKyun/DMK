#include "stdafx.h"

Bool DBErrorMessage::CheckError(SQLSMALLINT handleType, SQLHANDLE handle, SQLRETURN ret)
{
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		return true;
	}

	SQLSMALLINT rec = 0;

	while (true)
	{
		WChar state[SQL_SQLSTATE_SIZE + 1] = { 0, };
		SQLINTEGER	nativeError = 0;
		WChar errMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
		SQLSMALLINT msgLen = 0;

		SQLRETURN result = ::SQLGetDiagRecW(handleType, handle, rec, state, &nativeError, errMsg, _countof(errMsg), &msgLen);
		if (SQL_NO_DATA == result)
		{
			break;
		}

		if (result != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}

		LOG_ERROR(L"State: %s, nativeError: %d, ErrorMessage: %s\r\n", state, nativeError, errMsg);
	}

	if (SQL_SUCCEEDED(ret))
	{
		return true;
	}

	return false;
}