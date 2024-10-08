/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLFRHAN.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLFREEHANDLE
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLFreeHandle"

#define TABLE	""
#define SQLSTMT1 "SHOW TABLES;"
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLFreeHandle:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLFreeHandle(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;

   SQLHSTMT                hstmt1;
   SQLHSTMT                hstmt2;
   SQLHSTMT                hstmt3;
/* ------------------------------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.

	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (HENV)");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_VERSION : 'SQL_OV_ODBC3' \r\n");

	/* ----------------------------------------------------------------- */

	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			                  lpSrvr->szValidLogin0, SQL_NTS,
			                  lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	/* ----------------------------------------------------------------- */

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, &hstmt1);
	DIAGRECCHECK(SQL_HANDLE_STMT, hstmt1, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, &hstmt2);
	DIAGRECCHECK(SQL_HANDLE_STMT, hstmt2, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, &hstmt3);
	DIAGRECCHECK(SQL_HANDLE_STMT, hstmt3, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	/* ----------------------------------------------------------------- */

	szLogPrintf(lpSrvr,FALSE,"\t ExecStatement : '%s' \r\n", SQLSTMT1);

   rc = SQLExecDirect(hstmt1, SQLSTMT1, SQL_NTS);
   DIAGRECCHECK(SQL_HANDLE_STMT, hstmt1, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

   rc = SQLExecDirect(hstmt2, SQLSTMT1, SQL_NTS);
   DIAGRECCHECK(SQL_HANDLE_STMT, hstmt2, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

   rc = SQLExecDirect(hstmt3, SQLSTMT1, SQL_NTS);
   DIAGRECCHECK(SQL_HANDLE_STMT, hstmt3, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* ----------------------------------------------------------------- */

	rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
	DIAGRECCHECK(SQL_HANDLE_STMT, hstmt1, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");

	rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt2);
	DIAGRECCHECK(SQL_HANDLE_STMT, hstmt2, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");

	rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt3);
	DIAGRECCHECK(SQL_HANDLE_STMT, hstmt3, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");

	/* ----------------------------------------------------------------- */

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
