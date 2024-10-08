/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SETCONOP.C
**
** Purpose      : Auto Test DLL, Test von SQLSetConnectOption(SQLAUTOCOMMIT)
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "unixtest.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "SETCONNECTOPT"
#define TABLE TESTNAME

#define MAX_NAME_LEN 50
#define STRING_LEN 10
#define CHAR_LEN 120

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSetConnectOption:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSetConnectOption(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	UDWORD 			vAutoCommit;
	UCHAR 			szCHAR[CHAR_LEN+1];
	SDWORD 			cbCHAR;
/* ---------------------------------------------------------------------har- */

	/* --- Connect ----------------------------------------------------- */
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

	/* --- GetConnectOption -------------------------------------------- */
	rc = SQLGetConnectOption( *phdbc, SQL_AUTOCOMMIT, &vAutoCommit);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption");

	if (vAutoCommit==SQL_AUTOCOMMIT_ON) lstrcpy(buffer,"ON");
	    else if (vAutoCommit==SQL_AUTOCOMMIT_OFF) lstrcpy(buffer,"OFF");
	szLogPrintf(lpSrvr,FALSE,"\t SQL_AUTOCOMMIT: '%s' \r\n", buffer);

	if (vAutoCommit == SQL_AUTOCOMMIT_ON) {
		vAutoCommit =  SQL_AUTOCOMMIT_OFF;  /* o. SQL_AUTOCOMMIT_ON */
		rc = SQLSetConnectOption( *phdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetConnectOption");
	}

	/* --- Create Table ------------------------------------------------ */
        lstrcpy (create,"create table "TABLE" (col1 char(100))");
	szLogPrintf(lpSrvr,FALSE,"\t Create Stmt.: '%s'\r\n", create);

  	rc = SQLExecDirect (*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table ------------------------------------------------ */
        lstrcpy (insert,"insert "TABLE" values (?)");
	szLogPrintf (lpSrvr,FALSE,"\t Insert Stmt.: '%s'\r\n", insert);

	rc = SQLPrepare (*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	lstrcpy (szCHAR, "COL");

 	cbCHAR = SQL_NTS;
	rc = SQLBindParameter (*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
				SQL_CHAR, CHAR_LEN, 0, szCHAR, 0, &cbCHAR);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLExecute (*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	/* --- Select Table ------------------------------------------------ */
        lstrcpy(select,"select col1 from "TABLE);
	szLogPrintf(lpSrvr,FALSE,"\t Select Stmt.: '%s'\r\n", select);

	rc = SQLExecDirect (*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

 	cbCHAR = SQL_NTS;
	rc = SQLBindCol (*phstmt, 1, SQL_C_CHAR, szCHAR, CHAR_LEN, &cbCHAR);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

  	rc = SQLFetch (*phstmt);
  	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	szLogPrintf (lpSrvr,FALSE,"\t szCHAR(%ld)='%s'\r\n", cbCHAR, szCHAR);

  	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
  	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

#ifdef DROP_TABLE
	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE "TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif

	/* --- SQLTransact (ROLLBACK) -------------------------------------- */
	/*rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK); */
	rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact (SQL_COMMIT)");

	/* --- Disconnect -------------------------------------------------- */
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
