/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLBROWC.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLBROWSECONNECT
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLBrowseConnect"
#define TABLE ""

//--- mysql ??? #define CONN_DSN 	"DSN=SQL-BROWSE-R12"
#define CONN_DSN 	"DSN=myodbc3"
#define CONN_UID 	"UID="
#define CONN_PWD 	"PWD="
#define CONN_SERVERNODE "SERVERNODE=he"
#define CONN_SERVERDB	"SERVERDB=test"
#define CONN_STR	"successful connection !"
#define CONN_ERROR	"Database not running ! "

#define MAX_LEN  300

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLBrowseConnect:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLBrowseConnect(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	HWND			hwnd;
	UCHAR 			szConnStrIn[MAX_LEN];
	SWORD			cbConnStrIn;
	UCHAR			szConnStrOut[MAX_LEN];
	SWORD			cbConnStrOutMax;
	SWORD 			pcbConnStrOut;

	SWORD			count;
/* ---------------------------------------------------------------------har- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	rc = SQLAllocEnv(phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocEnv");

	rc = SQLAllocConnect(*phenv, phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");
/* ------------------------------------------------------------------------- */
	count=1;
	do {
	  switch (count) {
		case (1) :
			/* ***** 1. Set DSN parameter */
			lstrcpy(szConnStrIn,CONN_DSN); cbConnStrIn=SQL_NTS;
			lstrcpy(szConnStrOut,""); cbConnStrOutMax=MAX_LEN;
			break;
		case (2) :
			/* ***** 2. Set UID parameter */
			lstrcpy(szConnStrIn,CONN_UID); cbConnStrIn=SQL_NTS;
			/* Login UserID */
			lstrcat(szConnStrIn,lpSrvr->szValidLogin0);
			lstrcpy(szConnStrOut,""); cbConnStrOutMax=MAX_LEN;
			break;
		case (3) :
			/* ***** 3. Set PWD parameter */
			lstrcpy(szConnStrIn,CONN_PWD); cbConnStrIn=SQL_NTS;
			/* Login Password */
			lstrcat(szConnStrIn,lpSrvr->szValidPassword0);
			lstrcpy(szConnStrOut,""); cbConnStrOutMax=MAX_LEN;
			break;
		case (4) :
			/* ***** 4. Set SERVERNODE parameter */
			lstrcpy(szConnStrIn,CONN_SERVERNODE);
			cbConnStrIn=SQL_NTS;
			lstrcpy(szConnStrOut,""); cbConnStrOutMax=MAX_LEN;
			break;
		case (5) :
			/* ***** 5. Set SERVERNODE parameter */
			lstrcpy(szConnStrIn,CONN_SERVERDB);
			cbConnStrIn=SQL_NTS;
			lstrcpy(szConnStrOut,""); cbConnStrOutMax=MAX_LEN;
			break;
	  }

	szLogPrintf(lpSrvr, FALSE,"\tIN  -> szConnStrIn  = '%s'\r\n",
				  szConnStrIn);

	rc = SQLBrowseConnect(*phdbc, szConnStrIn, cbConnStrIn,	szConnStrOut,
				cbConnStrOutMax, &pcbConnStrOut);
	if (rc != SQL_NEED_DATA) {
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBrowseConnect");
	} else {
		szLogPrintf(lpSrvr, FALSE,
		  "\t  SQLBrowseConnect -> retcode: %d (SQL_NEED_DATA)\r\n",rc);
	}
	szLogPrintf(lpSrvr, FALSE,"\tOUT -> szConnStrOut = '%s'\r\n",
				  szConnStrOut);
	count ++;
	} while (rc == SQL_NEED_DATA);

	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		szLogPrintf(lpSrvr, FALSE,"\tSQLBrowseConnect -> %s\r\n",CONN_STR);
	} else { goto DoAbort; }
/* ------------------------------------------------------------------------- */
	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisConnect");

	rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");
/* ------------------------------------------------------------------------- */
	rc = SQLAllocEnv(phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocEnv");

	rc = SQLAllocConnect(*phenv, phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	hwnd=lpSrvr->hwnd;
	lstrcpy(szConnStrIn,szConnStrOut);
	cbConnStrIn=SQL_NTS;
	lstrcpy(szConnStrOut,"");
	cbConnStrOutMax=MAX_LEN;

	rc = SQLDriverConnect(*phdbc, hwnd, szConnStrIn, cbConnStrIn,
				szConnStrOut, cbConnStrOutMax, &pcbConnStrOut,
				SQL_DRIVER_COMPLETE);
/*	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,
			(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
			? rc : SQL_SUCCESS, rc, "SQLDriverConnect");
*/
	if (rc != SQL_SUCCESS) {
	  szLogPrintf(lpSrvr, FALSE,"\tSQLDriverConnect -> %s\r\n",CONN_ERROR);
	} else {
	  szLogPrintf(lpSrvr, FALSE,"\t\tSQLDriverConnect -> retcode: %d\r\n",rc);
	}

	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisConnect");

	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	  szLogPrintf(lpSrvr, FALSE,"\tSQLDriverConnect -> %s\r\n",CONN_STR);
	  szLogPrintf(lpSrvr, FALSE,"\tOUT -> szConnStrOut = '%s'\r\n",
				  szConnStrOut);
	}

	rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");
	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------- */
