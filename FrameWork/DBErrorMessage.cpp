#include "stdafx.h"

Bool DBErrorMessage::CheckError(DBConnection* const connection, SQLSMALLINT handleType, SQLHANDLE handle, SQLRETURN ret)
{
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		return true;
	}


	static const wchar_t DISCONNECT_ERROR_STATE[][SQL_SQLSTATE_SIZE + 1] =
	{
		L"08S01",	// Communication link failure
		L"08003",	// Connection not open
		L"08007",	// Connection failure during transaction
	};

	SQLSMALLINT rec = 0;
	Bool disconnect = false;
	while (true)
	{
		WChar state[SQL_SQLSTATE_SIZE + 1] = { 0, };
		SQLINTEGER	nativeError = 0;
		WChar errMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
		SQLSMALLINT msgLen = 0;

		SQLRETURN result = ::SQLGetDiagRecW(handleType, handle, ++rec, state, &nativeError, errMsg, _countof(errMsg), &msgLen);
		if (SQL_NO_DATA == result)
		{
			break;
		}

		if (result != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}

		if (false == disconnect)
		{
			for (int i = 0; i < _countof(DISCONNECT_ERROR_STATE); ++i)
			{
				if (::wcscmp(DISCONNECT_ERROR_STATE[i], state) == 0)
				{
					disconnect = true;
					break;
				}
			}
		}
		
		LOG_ERROR("State: {0}, nativeError: {1}", state, nativeError);
	}

	if (true == disconnect)
	{
		if (connection != nullptr)
		{
			connection->Disconnect();
		}
	}

	if (SQL_SUCCEEDED(ret))
	{
		return true;
	}

	return false;
}