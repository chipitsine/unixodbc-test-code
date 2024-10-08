/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDIAGF.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGETDIAGFIELD
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLGetDiagField"
#define TABLE 	 "TABERRORFIELD"
#define SQLSTMT1 "SELECT * FROM DUAL"

#define MAX_NAME_LEN 50
#define ERROR_TEXT_LEN 511
#define MAX_LONG     120

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLGetDiagField:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetDiagField(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	SQLCHAR                 create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	SQLCHAR                 drop[MAX_NAME_LEN+1];
	SQLCHAR                 tabname[MAX_NAME_LEN];
	/*
	SQLCHAR 		Sqlstate[MAX_NAME_LEN];
	SQLINTEGER 		NativeError;
	SQLCHAR 		MessageText[ERROR_TEXT_LEN+1];
	SQLSMALLINT 		TextLength;
	*/
	SQLSMALLINT 		BufferLength = ERROR_TEXT_LEN;
	SQLSMALLINT 		StringLength;
	SQLCHAR 		szCharPtr[MAX_NAME_LEN];
	SQLINTEGER		szIntPtr;
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
			/* ***** SQL_DIAG_SQLSTATE ---------- */
			BufferLength=ERROR_TEXT_LEN;
			rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_SQLSTATE,
					&szCharPtr, BufferLength, &StringLength);
			szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);

			if (rc == SQL_SUCCESS) {
			 /* ***** SQL_DIAG_SQLSTATE ---------- */
			 sprintf(buffer,"SQL_DIAG_SQLSTATE	: (%d) %s",StringLength, szCharPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_NATIVE ------------ */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_NATIVE,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_NATIVE  	: (%d) %ld",StringLength, szIntPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_MESSAGE TEXT ---------- */
			 BufferLength=ERROR_TEXT_LEN;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_MESSAGE_TEXT,
					&szCharPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_MESSAGE_TEXT	: (%d) %s",StringLength, szCharPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_CLASS_ORIGIN ---------- */
			 BufferLength=ERROR_TEXT_LEN;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_CLASS_ORIGIN,
					&szCharPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_CLASS_ORIGIN	: (%d) %s",StringLength, szCharPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_COLUMN_NUMBER ------------ */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_COLUMN_NUMBER,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_COLUMN_NUMBER	: (%d) ",StringLength);
			 switch (szIntPtr) {
			   case (SQL_COLUMN_NUMBER_UNKNOWN) : lstrcat(buffer, "SQL_COLUMN_NUMBER_UNKNOWN"); break;
			   case (SQL_NO_COLUMN_NUMBER)	    : lstrcat(buffer, "SQL_NO_COLUMN_NUMBER"); break;
			   default			    : lstrcat(buffer, "?????");
			 }
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_CONNECTION_NAME ---------- */
			 BufferLength=ERROR_TEXT_LEN;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_CONNECTION_NAME,
					&szCharPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_CONNECTION_NAME: (%d) %s",StringLength, szCharPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_ROW_NUMBER ------------ */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_ROW_NUMBER,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_ROW_NUMBER	: (%d) ",StringLength);
			 switch (szIntPtr) {
			   case (SQL_ROW_NUMBER_UNKNOWN) : lstrcat(buffer, "SQL_ROW_NUMBER_UNKNOWN"); break;
			   case (SQL_NO_ROW_NUMBER)      : lstrcat(buffer, "SQL_NO_ROW_NUMBER"); break;
			   default			 : lstrcat(buffer, "?????");
			 }
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_SERVER_NAME ---------- */
			 BufferLength=ERROR_TEXT_LEN;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_SERVER_NAME,
					&szCharPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_SERVER_NAME	: (%d) %s",StringLength, szCharPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			 /* ***** SQL_DIAG_SUBCLASS_ORIGIN ---------- */
			 BufferLength=ERROR_TEXT_LEN;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_SUBCLASS_ORIGIN,
					&szCharPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_SUBCLASS_ORIGIN: (%d) %s",StringLength, szCharPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

 			/* ***** SQL_DIAG_CURSOR_ROW_COUNT ---------- */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_CURSOR_ROW_COUNT,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_CURSOR_ROW_COUNT: (%d) %d",StringLength, szIntPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

 			/* ***** SQL_DIAG_DYNAMIC_FUNCTION ---------- */
			 BufferLength=ERROR_TEXT_LEN;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_DYNAMIC_FUNCTION,
					&szCharPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_DYNAMIC_FUNCTION: ");
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			/* ***** SQL_DIAG_DYNAMIC_FUNCTION_CODE ---------- */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_DYNAMIC_FUNCTION_CODE,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_DYNAMIC_FUNCTION_CODE:");
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			/* ***** SQL_DIAG_NUMBER ---------- */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_NUMBER,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_NUMBER  	: (%d) %ld",StringLength, szIntPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			/* ***** SQL_DIAG_RETURNCODE ---------- */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_NUMBER,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_RETURNCODE  	: (%d) %ld",StringLength, szIntPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);

			/* ***** SQL_DIAG_ROW_COUNT ---------- */
			 BufferLength=0;
			 rc = SQLGetDiagField(SQL_HANDLE_STMT, *phstmt, RecNum, SQL_DIAG_ROW_COUNT,
					&szIntPtr, BufferLength, &StringLength);
			 szLogPrintf(lpSrvr, FALSE,"\t\t SQLGetDiagField -> retcode: %d\r\n", rc);
			 sprintf(buffer,"SQL_DIAG_ROW_COUNT  	: (%d) %d",StringLength, szIntPtr);
			 szLogPrintf(lpSrvr, FALSE,"\t\t\t -> %s\r\n", buffer);
			}

			if (rc == SQL_ERROR) break;
			  else	RecNum++;
		}
	}

	/* --- Drop Table ------------------------------------------------- */
	lstrcpy(drop,"DROP TABLE "TABLE);
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
