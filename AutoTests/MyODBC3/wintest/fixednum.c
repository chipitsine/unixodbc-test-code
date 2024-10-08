/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : FIXEDNUM.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von FIXED-Zahlen
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "FIXEDNUMBERS"
#define TABLE "NUMFIXED"

#define MAX_NAME_LEN 50
#define STR_LEN      256

#define MAX_NUMS 34
#define NUMSWERT "1"
#define NUMSNULL "0"
#define NUMSNEGA "-1"
#define NUMSEXP2 ".12"
#define NUMSEXP3 ".123"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoFixedNumbers:
//|     This test case will test the fixed numbers write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoFixedNumbers(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN+1];

	UCHAR			szDoubleNum[STR_LEN];
	UCHAR			szDoubleNumS[STR_LEN];
	SDWORD                  cbDouble;
	SWORD                   i;
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

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname,TABLE);
	lstrcpy(create,"CREATE TABLE "TABLE" (NUMBERS DECIMAL(18,2) )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO "TABLE" VALUES (?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			    SQL_NUMERIC, 18, 2, szDoubleNum, STR_LEN, &cbDouble);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	/* Insert FIXED mit 2 Kommastellen */
	i=1; lstrcpy(szDoubleNumS,NUMSNULL);
	while (i < MAX_NUMS+1) {
		lstrcpy(szDoubleNum,szDoubleNumS);lstrcat(szDoubleNum,NUMSEXP2);
		szLogPrintf(lpSrvr, FALSE,
			    "\tInsert Value %d.: ->'%s'\r\n",i,szDoubleNum);

		cbDouble = strlen(szDoubleNum);
		rc = SQLExecute(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

		i++;
		lstrcat(szDoubleNumS,NUMSWERT);
		if (i==2) lstrcpy(szDoubleNumS,NUMSWERT);
			else if (i==18) lstrcpy(szDoubleNumS,NUMSNULL);
			else if (i==19) lstrcpy(szDoubleNumS,NUMSNEGA);
	}

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT NUMBERS FROM "TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=0;
	while (1) {
		rc = SQLBindCol(*phstmt, 1, SQL_C_CHAR, szDoubleNum,
						STR_LEN, &cbDouble);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQBindCol");

		rc = SQLFetch(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

		if (rc == SQL_NO_DATA_FOUND) break;

		i++;
		szLogPrintf(lpSrvr,FALSE,
			"\tSelect Values %d.: ->'%s'\r\n",i,szDoubleNum);
	}
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	lstrcpy(drop,"DROP TABLE "TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQFreeStmt");

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQExecDirect");
#endif
	/* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */

	/* rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK); */
	rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQTransact");

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
