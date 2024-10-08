/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : RESLTMAX.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Test: Ergebnismenge auf maximum von Zeilen beschraenken.
**
******************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

/* ----------------------------------------------------------------------- */
#define TESTNAME "ResultsetMax"
#define TABLE "RESULTMAX"

#define MAX_NAME_LEN	512
#define MAX_STR_LEN	30
#define MAX_ROWS_INPUT  20
#define MAX_ROWS	10

#define TXTCOPY "TEST: ResultsetMax"

/* ------------------------------------------------------------------------ */
/*
 -------------------------------------------------------------------------
| DoResultMax:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 -------------------------------------------------------------------------
*/
SWORD FAR PASCAL DoResultMax(HENV FAR * phenv, HDBC FAR * phdbc,
                             HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ---------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   buf[MAX_NAME_LEN+1];

	UCHAR                   szString[MAX_STR_LEN];
	SWORD			sNumber;
	SDWORD			cbNumber=SQL_NTS;
	SDWORD			cbString=SQL_NTS;

	UCHAR			szString_rc[MAX_NAME_LEN];
	SWORD			sNumber_rc;
	SDWORD			cbNumber_rc = SQL_NTS;
	SDWORD			cbString_rc = SQL_NTS;

	UDWORD			irow;
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
/* --- ResultsetMaximun ------------------------------------------------- */
	/* --- Create Table --------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE);
	lstrcat(create," ( NUM SMALLINT, STRING CHAR(30)) ");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE);
	lstrcat(insert," VALUES (?, ?) ");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_INTEGER, 0, 0, &sNumber, 0, &cbNumber);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
		SQL_CHAR, MAX_STR_LEN, 0, szString, MAX_STR_LEN, &cbString);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");


	irow=1;
	cbString=SQL_NTS; cbNumber=SQL_NTS;
	szLogPrintf(lpSrvr, FALSE,"\tInsert Values ->\r\n");
	while (irow < MAX_ROWS_INPUT+1) {
		sNumber = (SWORD) irow;
		lstrcpy(szString,TXTCOPY);
		sprintf(buf,"%d.",irow);
		lstrcat(szString,buf);

		rc = SQLExecute(*phstmt);
	        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute?");
		irow++;
	}
	szLogPrintf(lpSrvr, FALSE,"\tInsert into table (%s) -> %d Valeus\r\n",TABLE, irow-1);

	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetchStmt");
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	rc = SQLAllocStmt(*phdbc, phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");

	rc = SQLSetStmtOption(*phstmt, SQL_MAX_ROWS, MAX_ROWS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption");

	szLogPrintf(lpSrvr, FALSE,"\t -> Set SQL_MAX_ROWS to : %d\r\n",MAX_ROWS);

	lstrcpy(select,"SELECT ");
	lstrcat(select," \"NUM\", \"STRING\" ");
	lstrcat(select," FROM ");
	lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt = '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	if (rc == SQL_SUCCESS) {
	   rc = SQLBindCol(*phstmt, 1, SQL_C_SSHORT, &sNumber_rc, 0, &cbNumber_rc);
 	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	   rc = SQLBindCol(*phstmt, 2, SQL_C_CHAR, szString_rc, MAX_STR_LEN, &cbString_rc);
 	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	   irow=1;
	   while(1) {
	      rc = SQLFetch(*phstmt);
	      ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

 	      if (rc == SQL_NO_DATA_FOUND) break;

	      szLogPrintf(lpSrvr, FALSE,"\tRow -> %d - %s \r\n", sNumber_rc, szString_rc);
	      irow += 1;
	   }
	}
	/* ------------------------------------------------------------------ */
        rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

/* ------------------------------------------------------------------------- */
        /* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
        lstrcpy(drop,"DROP TABLE ");
        lstrcat(drop,TABLE);
        szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

        rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

        rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif
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
