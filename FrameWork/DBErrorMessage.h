#pragma once

class DBErrorMessage
{
public:

	static Bool CheckError(SQLSMALLINT handleType, SQLHANDLE handle, SQLRETURN ret);
};