/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLNATIV.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLNativeSql
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLNativeSql"

#define MAX_NAME_LEN 80
#define ERROR_TEXT_LEN 511

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLNativeSql:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLNativeSql(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR 	szSqlStrIn[STR_LEN],
		szSqlStr[STR_LEN];

	SDWORD 	pcbSqlStr;

	SDWORD 	cbSqlStrIn,
		cbSqlStrMax;

	UCHAR sqlstate[10];
	DWORD esq_sql_code;
	UCHAR error_txt[ERROR_TEXT_LEN+1];
	SWORD len_error_txt = ERROR_TEXT_LEN;
	SWORD used_error_txt;
	UCHAR buffer[1024];
/* ---------------------------------------------------------------------har- */

	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	rc = SQLAllocEnv(phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocEnv");

	rc = SQLAllocConnect(*phenv, phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLConnect");

/* ------------------------------------------------------------------------- */
	/* ***************************************************************** */
	/* *** I. SQLNativSql ********************************************** */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLNativeSql -> '\r\n");

	lstrcpy(szSqlStrIn, "CREATE TABLE TEST (KEY INTEGER, NAME CHAR(30))");
	cbSqlStrMax = lstrlen(szSqlStrIn) + 1;
	cbSqlStrIn = SQL_NTS;
	szLogPrintf(lpSrvr, FALSE,"\t IN => SqlStrIn   : '%s'\r\n",szSqlStrIn);
	szLogPrintf(lpSrvr, FALSE,"\t       cbSqlStrIn : %d \r\n",cbSqlStrIn);
	szLogPrintf(lpSrvr, FALSE,"\t       cbSqlStrMax: %d \r\n", cbSqlStrMax);

	rc = SQLNativeSql(*phdbc, szSqlStrIn, cbSqlStrIn, szSqlStr,
				cbSqlStrMax, &pcbSqlStr);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLNativeSql");

	szLogPrintf(lpSrvr, FALSE,"\t OUT => SqlStr    : '%s'\r\n",szSqlStr);
	szLogPrintf(lpSrvr, FALSE,"\t        pcbSqlStr : %d '\r\n",pcbSqlStr);

	/* ***************************************************************** */
	/* *** II. SQLNativSql ********************************************* */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tII.) SQLNativeSql -> (Data trucated)'\r\n");

	lstrcpy(szSqlStrIn, "CREATE TABLE TEST (KEY INTEGER, NAME CHAR(30))");
	cbSqlStrMax = lstrlen(szSqlStrIn) - 5;
	cbSqlStrIn = SQL_NTS;
	szLogPrintf(lpSrvr, FALSE,"\t IN => SqlStrIn   : %s'\r\n",szSqlStrIn);
	szLogPrintf(lpSrvr, FALSE,"\t       cbSqlStrIn : %d \r\n",cbSqlStrIn);
	szLogPrintf(lpSrvr, FALSE,"\t       cbSqlStrMax: %d \r\n", cbSqlStrMax);

	rc = SQLNativeSql(*phdbc, szSqlStrIn, cbSqlStrIn, szSqlStr,
				cbSqlStrMax, &pcbSqlStr);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLNativeSql");

	szLogPrintf(lpSrvr, FALSE,"\t OUT => SqlStr    : '%s'\r\n",szSqlStr);
	szLogPrintf(lpSrvr, FALSE,"\t        pcbSqlStr : %d '\r\n",pcbSqlStr);

	/* ***************************************************************** */
	/* *** III. SQLNativSql ******************************************** */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tIII.) SQLNativeSql -> (Error)'\r\n");

	lstrcpy(szSqlStrIn, "CREATE TABLE TEST (KEY INTEGER, NAME CHAR(30))");
	cbSqlStrMax = SQL_NTS;
	cbSqlStrIn = SQL_NTS;
	szLogPrintf(lpSrvr, FALSE,"\t IN => SqlStrIn   : %s'\r\n",szSqlStrIn);
	szLogPrintf(lpSrvr, FALSE,"\t       cbSqlStrIn : %d \r\n",cbSqlStrIn);
	szLogPrintf(lpSrvr, FALSE,"\t       cbSqlStrMax: %d \r\n", cbSqlStrMax);

	rc = SQLNativeSql(*phdbc, szSqlStrIn, cbSqlStrIn, szSqlStr,
				cbSqlStrMax, &pcbSqlStr);
	/*ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLNativeSql"); */

	if (rc != SQL_SUCCESS) {
		SQLError(*phenv, *phdbc, *phstmt, sqlstate, &esq_sql_code,
		error_txt, len_error_txt, &used_error_txt);

		sprintf(buffer,"sqlstate: %s",sqlstate);
		szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
		sprintf(buffer,"esq_sql_code: %ld",esq_sql_code);
		szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
		sprintf(buffer,"error_txt: %s", error_txt);
		szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
	}
  	else {
		szLogPrintf(lpSrvr, FALSE,"\t OUT => SqlStr    : '%s'\r\n",szSqlStr);
		szLogPrintf(lpSrvr, FALSE,"\t        pcbSqlStr : %d '\r\n",pcbSqlStr);
	}
/* ---------------------------------------------------------------------har- */
	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------ */
