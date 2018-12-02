#pragma once
class DBConnection;
class DBErrorMessage
{
public:

	static Bool CheckError(DBConnection* const connection, SQLSMALLINT handleType, SQLHANDLE handle, SQLRETURN ret);
};