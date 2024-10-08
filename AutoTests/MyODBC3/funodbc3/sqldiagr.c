/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDIADR.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGETDIAGREC
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLGetDiagRec"

#define TABLE 	 "TABERROR"
#define SQLSTMT1 "SELECT * FROM DUAL"

#define MAX_NAME_LEN 50
#define ERROR_TEXT_LEN 511
#define MAX_LONG     120

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLGetDiagRec:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetDiagRec(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	SQLCHAR                 create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	SQLCHAR                 drop[MAX_NAME_LEN+1];

	SQLCHAR                 tabname[MAX_NAME_LEN];

	SQLCHAR 		Sqlstate[MAX_NAME_LEN];
	SQLINTEGER 		NativeError;
	SQLCHAR 		MessageText[ERROR_TEXT_LEN+1];
	SQLSMALLINT 		BufferLength = ERROR_TEXT_LEN;
	SQLSMALLINT 		TextLength;
	SQLSMALLINT 		RecNum;


	SQLCHAR 		buffer[1024];

/* ------------------------------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	/* - Connect ------------------------------------------------------- */
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (HENV)");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_VERSION : 'SQL_OV_ODBC3' \r\n");

	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			                  lpSrvr->szValidLogin0, SQL_NTS,
			                  lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	/* ----------------------------------------------------------------- */

	/* --- Create Table ------------------------------------------------ */
	lstrcpy(tabname,TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (TYP_CHAR CHAR(60) )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLPrepare(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLPrepare");

	rc = SQLExecute(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecute");

	/* --- SQLError   ------------------------------------------------- */
	rc = SQLPrepare(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLPrepare");

	rc = SQLExecute(*phstmt);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLExecute -> retcode: %d\r\n", rc);
	if (rc != SQL_SUCCESS) {
		RecNum = 1;
		while (rc != SQL_NO_DATA_FOUND) {
			rc = SQLGetDiagRec(SQL_HANDLE_STMT, *phstmt, RecNum, Sqlstate,
				&NativeError, MessageText, BufferLength, &TextLength);
			szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagRec -> retcode: %d\r\n", rc);

			if (rc == SQL_SUCCESS) {
				sprintf(buffer,"Sqlstate	: %s",Sqlstate);
				szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
				sprintf(buffer,"NativeError	: %ld",NativeError);
				szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
				sprintf(buffer,"MessageText	: %s", MessageText);
				szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
				sprintf(buffer,"BufferLength	: %d",BufferLength);
				szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
				sprintf(buffer,"TextLength	: %d",TextLength);
				szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
			}
			RecNum++;
		}
	}

	/* --- Drop Table ------------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLPrepare(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLPrepare");

	rc = SQLExecute(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecute");

	/* - Disconnect ---------------------------------------------------- */
	rc = SQLFreeHandle(SQL_HANDLE_STMT, *phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");

	rc = SQLDisconnect(*phdbc);
   	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDisconnect");

	rc = SQLFreeHandle(SQL_HANDLE_DBC, *phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HDBC)");

	rc = SQLFreeHandle(SQL_HANDLE_ENV, *phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HENV)");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------- */
