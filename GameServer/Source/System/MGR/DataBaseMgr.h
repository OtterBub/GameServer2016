#pragma once

// SQLConnect_ref.cpp
// compile with: odbc32.lib
#include "../../Common.h"

void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode);

class DataBaseMgr
{
public:
	static DataBaseMgr& GetInstance()
	{
		static DataBaseMgr instance;
		return instance;
	}

	void Init();

	void Disconnect();

	int SearchNick(const std::string nick, unsigned int index);
	SQLRETURN ThereIsQuery(std::string str);
	SQLRETURN Query(std::string str);
	SQLRETURN Query(std::string str, unsigned int index);

private:
	DataBaseMgr() {}
	DataBaseMgr(const DataBaseMgr& temp) {}

	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	SQLRETURN retcode;

	bool mNodata;
};

#define DBMGR DataBaseMgr::GetInstance()