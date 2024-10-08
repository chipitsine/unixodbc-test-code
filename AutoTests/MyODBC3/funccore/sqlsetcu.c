/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSETCU.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSETCURSORNAME
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funccore.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLSetCursorName"
#define TABLE "SETCURSOR"
#define TESTCUR "SQL_TEST_CUR"

#define MAX_NAME_LEN 50
#define MAX_CUR_NAME_LEN 18

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSQLSetCursorName:
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLSetCursorName(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];

	UCHAR                   tabname[MAX_NAME_LEN];

	UCHAR			szCursor[MAX_CUR_NAME_LEN];
	SWORD			cbCursorMax;
	SWORD			cbCursor;
	SWORD			pcbCursor;

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

	rc = SQLAllocStmt(*phdbc, phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");

/* ---------------------------------------------------------------------har- */

	/* --- SQLSetCursorName -------------------------------------------- */
	lstrcpy(szCursor,TESTCUR);  cbCursor = lstrlen(szCursor);
	rc = SQLSetCursorName(*phstmt, szCursor, cbCursor);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetCursorName");

	szLogPrintf(lpSrvr, FALSE,"\tSetCursorName = '%s'\r\n",szCursor);

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname,TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (TYP_CHAR CHAR(60) )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLPrepare(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	/* --- SQLGetCursorName -------------------------------------------- */
	cbCursorMax = MAX_CUR_NAME_LEN;
	rc = SQLGetCursorName(*phstmt, szCursor, cbCursorMax, &pcbCursor);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetCursorName");

	szLogPrintf(lpSrvr, FALSE,"\tGetCursorName = '%s'\r\n",szCursor);

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLPrepare(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");
/* ---------------------------------------------------------------------har- */
	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

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
/* ************************************************************************* */

