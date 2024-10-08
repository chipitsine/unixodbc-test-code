/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : DATETIME.C
**
** Purpose      : Auto Test DLL, Test von DATE/TIME Datatypen
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "unixtest.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "DATETIME"
#define TABLE "DATETIME"

#define MAX_NAME_LEN 50
#define DATE_LEN 10
#define TIME_LEN 8
#define TIMESTAMP_LEN DATE_LEN + TIME_LEN
#define CRLF "\x0D\x0A"

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoDateTime:
//|     This test case will test the date and time datatypes write and read
//|     from the database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoDateTime(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                           sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];

	UCHAR                   szDATE[DATE_LEN+1],
				szTIME[TIME_LEN+1];
	SDWORD                  cbDATE,
				cbTIME,
				cbTIMESTAMP,
				cbTIMESTAMP2;
	DATE_STRUCT             sDATE;
	TIME_STRUCT             sTIME;
	TIMESTAMP_STRUCT        sTIMESTAMP, sTIMESTAMP2;
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

/* ------------------------------------------------------------------------- */

	/* --- Create Table --------------------------------------------- */
	lstrcpy(create,"CREATE TABLE "TABLE" (DATE1 date, TIME1 time)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT "TABLE" VALUES ('1994-09-27', '08:39:44')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 1.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExeDirect");

	/* --- 2. ---*/
	lstrcpy(insert,"INSERT "TABLE" VALUES ('1994-09-28', '19:43:44')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 2.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- 3. ---*/
	lstrcpy(insert,"INSERT "TABLE" VALUES ('1994-09-29', '14:32:10')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 3.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- 4. ---*/
	lstrcpy(insert,"INSERT "TABLE" VALUES ('1994-09-30', '11:35:33')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 4.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = GetRowCount(*phstmt, "Insert Table", lpSrvr);

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT * FROM "TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= %s\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecdirect");

	rc = GetRowCount(*phstmt, "Select Table", lpSrvr);

	rc = SQLBindCol(*phstmt, 1, SQL_C_CHAR, szDATE, DATE_LEN+1, &cbDATE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLBindCol(*phstmt, 2, SQL_C_CHAR, szTIME, 300, &cbTIME);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	szLogPrintf(lpSrvr, FALSE, "\tSelect Values 1. -> DATE='%s' (%d) - ",
		     szDATE, cbDATE);
	szLogPrintf(lpSrvr, FALSE, " TIME='%s' (%d)\r\n", szTIME, cbTIME);

	rc = SQLFreeStmt(*phstmt, SQL_UNBIND);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- 2. --- */
	rc = SQLBindCol(*phstmt, 1, SQL_C_DATE, &sDATE, sizeof(DATE_STRUCT),
			&cbDATE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLBindCol(*phstmt, 2, SQL_C_TIME, &sTIME, sizeof(TIME_STRUCT),
			&cbTIME);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	rc = GetRowCount(*phstmt, "Select Table", lpSrvr);

	szLogPrintf(lpSrvr, FALSE,
		"\tSelect Values 2. -> DATE='%d,%d,%d' - TIME='%d,%d,%d'\r\n",
		sDATE.year, sDATE.month, sDATE.day, sTIME.hour, sTIME.minute,
		sTIME.second );

	rc = SQLFreeStmt(*phstmt, SQL_UNBIND);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- 3. --- */
	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	rc = SQLGetData (*phstmt, 1, SQL_C_TIMESTAMP, &sTIMESTAMP,
			      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr, FALSE,
		"\tSelect Value 3a. -> TIMESTAMP = %d,%d,%d,%d,%d,%d,%d\r\n",
		sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
		sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second,
		sTIMESTAMP.fraction );

	rc = SQLGetData (*phstmt, 2, SQL_C_TIMESTAMP, &sTIMESTAMP,
			      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr, FALSE,
		"\tSelect Value 3b. -> TIMESTAMP = %d,%d,%d,%d,%d,%d,%d\r\n",
		sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
		sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second,
		sTIMESTAMP.fraction );

	/* --- 4. --- */
	rc = SQLBindCol (*phstmt, 1, SQL_C_TIMESTAMP, &sTIMESTAMP,
			 sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLBindCol (*phstmt, 2, SQL_C_TIMESTAMP, &sTIMESTAMP2,
			 sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP2);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	rc = GetRowCount(*phstmt, "Select Table", lpSrvr);

	szLogPrintf(lpSrvr, FALSE,
		"\tSelect Value 4a. -> TIMESTAMP = %d,%d,%d,%d,%d,%d,%d\r\n",
		sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
		sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second,
		sTIMESTAMP.fraction );

	szLogPrintf(lpSrvr, FALSE,
		"\tSelect Value 4b. -> TIMESTAMP = %d,%d,%d,%d,%d,%d,%d\r\n",
		sTIMESTAMP2.year, sTIMESTAMP2.month, sTIMESTAMP2.day,
		sTIMESTAMP2.hour, sTIMESTAMP2.minute, sTIMESTAMP2.second,
		sTIMESTAMP2.fraction );

#ifdef DROP_TABLE
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */

	lstrcpy(drop,"DROP TABLE "TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif
	/* rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK); */
	rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");

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

