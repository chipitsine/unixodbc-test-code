/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLALCON.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SMALLINT-Datentype
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "sqlexamp.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SMALLINTNUMBERS"

#define TABLE	"SINTNUM"
//*-------------------------------------------------------------------------

#define MAX_NAME_LEN 50
#define ERROR_TEXT_LEN 511

#define STRING_LEN 10

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSmallintDatatype:
//|     This test case will test the smallint datatypes write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSmallintDatatype(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                           sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];

	UCHAR                   tabname[MAX_NAME_LEN+1];

	UCHAR                   szSTRING[STRING_LEN+1];
	SWORD                   SI;
	SDWORD                  cbSTRING, cbSI;
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
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE "); lstrcat(create, tabname);
	lstrcat(create," (SMALLINT1 SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT ");
	lstrcat(insert,tabname);lstrcat(insert," VALUES (1)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 1.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- 2. ---*/
	lstrcpy(insert,"INSERT ");
	lstrcat(insert,tabname);lstrcat(insert," VALUES (32000)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 2.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- 3. ---*/
	lstrcpy(insert,"INSERT ");
	lstrcat(insert,tabname);lstrcat(insert," VALUES (-20)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 3.= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = GetRowCount(*phstmt, "Insert Table", lpSrvr);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"GetRowCount");

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT * FROM "); lstrcat(select, tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= %s\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = GetRowCount(*phstmt, "Select Table", lpSrvr);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"GetRowCount");

	rc = SQLBindCol(*phstmt, 1, SQL_C_CHAR, szSTRING,STRING_LEN,&cbSTRING);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	szLogPrintf(lpSrvr,FALSE,
			"\tSelect Values 1. -> SMALLINT='%s'\r\n",szSTRING);

	rc = SQLFreeStmt(*phstmt, SQL_UNBIND);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- 2. --- */
	rc = SQLBindCol(*phstmt, 1, SQL_C_DEFAULT, &SI, sizeof(SI), &cbSI);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	rc = GetRowCount(*phstmt, "SQLFetch", lpSrvr);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"GetRowCount");

	szLogPrintf(lpSrvr,FALSE,
			"\tSelect Values 2. -> SMALLINT='%d'\r\n",SI);

	rc = SQLFreeStmt(*phstmt, SQL_UNBIND);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- 3. --- */
	rc = SQLBindCol(*phstmt, 1, SQL_C_DEFAULT, &SI, sizeof(SI), &cbSI);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	rc = GetRowCount(*phstmt, "SQLFetch", lpSrvr);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"GetRowCount");

	szLogPrintf(lpSrvr,FALSE,
			"\tSelect Values 3. -> SMALLINT='%d'\r\n",SI);
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");lstrcat(drop, tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK);*/
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
/* ------------------------------------------------------------------------- */
