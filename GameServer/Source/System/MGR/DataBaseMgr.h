#pragma once

// SQLConnect_ref.cpp
// compile with: odbc32.lib
#include <windows.h>
#include <sqlext.h>
#include <iostream>

void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	SQLCHAR       wszMessage[1000];
	SQLCHAR       wszState[SQL_SQLSTATE_SIZE + 1];


	if (RetCode == SQL_INVALID_HANDLE)
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated.. 
		WCHAR state[SQL_SQLSTATE_SIZE + 1];
		//mbtowc(state, wszState, sizeof(wszState));
		if (wcsncmp(state, L"01004", 5))
		{
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}

}

class DataBaseMgr
{
public:
	static void Test()
	{
		SQLHENV henv;
		SQLHDBC hdbc;
		SQLHSTMT hstmt;
		SQLRETURN retcode;

		int char_id = 0;
		wchar_t char_name[100] = { 0, };
		int char_level = 0;
		float posX= 0, posY= 0;
		int exp = 0;
		int atk = 0;
		int def = 0;

		SQLINTEGER  pIndicators[8];

		setlocale(LC_ALL, "korean");

		SQLCHAR * OutConnStr = (SQLCHAR *)malloc(255);
		SQLSMALLINT * OutConnStrLen = (SQLSMALLINT *)malloc(255);

		// Allocate environment handle
		retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

		// Set the ODBC version environment attribute
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

			// Allocate connection handle
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

				// Set login timeout to 5 seconds
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

					// Connect to data source
					retcode = SQLConnect(hdbc, (SQLCHAR*)"odbc2", SQL_NTS, (SQLCHAR*)"sa", SQL_NTS, (SQLCHAR*)"gamedb", SQL_NTS);

					// Allocate statement handle
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

						// Process data
						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

							std::cout << "Connection Success \n";

							retcode = SQLExecDirect(hstmt,
								// ID -> 1, Name -> 2, CLevel -> 3 column
								//(SQLCHAR*)"INSERT INTO dbo.exp_user_table VALUES (2, 'Hello', 1, 2.2, 3.3, 3, 5, 8)",
								//(SQLCHAR*)"SELECT ID, Nick, CLevel, XPos, YPos, EXP, Attack, Defence FROM dbo.exp_user_table",
								//(SQLCHAR*)"SELECT ID, Nick, CLevel, XPos, YPos, EXP, Attack, Defence FROM dbo.exp_user_table",
								//(SQLCHAR*)"EXEC dbo.SELECT_HIGHLEVEL 10",
								(SQLCHAR*)"SELECT * FROM dbo.user_table WHERE Nick = 'Hello'",
								SQL_NTS);

							

							if (retcode != SQL_ERROR)
							{

								// Retrieve data from row set.
								SQLBindCol(hstmt, 1, SQL_C_LONG, (SQLPOINTER)&char_id, sizeof(char_id),
									&pIndicators[0]);

								// Retrieve data from row set.
								SQLBindCol(hstmt, 2, SQL_C_WCHAR, (SQLPOINTER)char_name, sizeof(char_name),
									&pIndicators[1]);

								// Retrieve data from row set.
								SQLBindCol(hstmt, 3, SQL_C_LONG, (SQLPOINTER)&char_level, sizeof(char_level),
									&pIndicators[2]);

								SQLBindCol(hstmt, 4, SQL_C_FLOAT, (SQLPOINTER)&posX, sizeof(posX),
									&pIndicators[3]);

								SQLBindCol(hstmt, 5, SQL_C_FLOAT, (SQLPOINTER)&posY, sizeof(posY),
									&pIndicators[4]);

								SQLBindCol(hstmt, 6, SQL_C_LONG, (SQLPOINTER)&exp, sizeof(exp),
									&pIndicators[5]);

								SQLBindCol(hstmt, 7, SQL_C_LONG, (SQLPOINTER)&atk, sizeof(atk),
									&pIndicators[6]);

								SQLBindCol(hstmt, 8, SQL_C_LONG, (SQLPOINTER)&def, sizeof(def),
									&pIndicators[7]);

								while (SQLFetch(hstmt) == SQL_SUCCESS)
								{
									std::wcout << "char_id: " << char_id << "\n";
									std::wcout << "char_name: " << char_name << "\n";
									std::wcout << "pos: " << posX << ", " << posY << "\n";
									std::wcout << "exp: " << exp << "\n";
									std::wcout << "atk: " << atk << "\n";
									std::wcout << "def: " << def << "\n";
								}
							}
							else
							{
								// Error Handle
								HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
							}

							SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
						}

						SQLDisconnect(hdbc);
					}
					else
					{
						HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
					}

					SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
				}
			}
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
		}
	}

private:
};
/*
int main() {
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	SQLRETURN retcode;

	int char_id = 0;
	wchar_t char_name[100] = { 0, };
	int char_level = 0;

	SQLINTEGER  pIndicators[3];

	setlocale(LC_ALL, "korean");

	SQLCHAR * OutConnStr = (SQLCHAR *)malloc(255);
	SQLSMALLINT * OutConnStrLen = (SQLSMALLINT *)malloc(255);

	// Allocate environment handle
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

		// Allocate connection handle
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"odbc2", SQL_NTS, (SQLWCHAR*)L"sa", SQL_NTS, (SQLWCHAR*)L"gamedb", SQL_NTS);

				// Allocate statement handle
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					// Process data
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

						std::cout << "Connection Success \n";

						retcode = SQLExecDirect(hstmt,
							// ID -> 1, Name -> 2, CLevel -> 3 column
							//(SQLWCHAR*)L"SELECT ID, Name, CLevel FROM dbo.user_table",
							(SQLWCHAR*)L"EXEC dbo.SELECT_HIGHLEVEL 10",
							SQL_NTS);

						if (retcode != SQL_ERROR)
						{

							// Retrieve data from row set.
							SQLBindCol(hstmt, 1, SQL_C_LONG, (SQLPOINTER)&char_id, sizeof(char_id),
								&pIndicators[0]);

							// Retrieve data from row set.
							SQLBindCol(hstmt, 2, SQL_C_WCHAR, (SQLPOINTER)char_name, sizeof(char_name),
								&pIndicators[1]);

							// Retrieve data from row set.
							SQLBindCol(hstmt, 3, SQL_C_LONG, (SQLPOINTER)&char_level, sizeof(char_level),
								&pIndicators[2]);

							while (SQLFetch(hstmt) == SQL_SUCCESS)
							{
								std::wcout << "char_id: " << char_id << "\n";
								std::wcout << "char_name: " << char_name << "\n";
								std::wcout << "char_level: " << char_level << "\n";
							}
						}
						else
						{
							// Error Handle
							HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
						}

						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}

					SQLDisconnect(hdbc);
				}
				else
				{
					HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
				}

				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
}
*/