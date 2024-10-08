/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSETPO.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSETPOS
**
******************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLSetPos"
#define TABLE "SETPOS"

#define MAX_NAME_LEN	80
#define MAX_STR_LEN	30
#define MAX_ROWS	40
#define ROW_SIZE	8
#define SETPOS1		2
#define SETPOS2		4

#define TXTCOPY "SQLSetPos String "

//*-------------------------------------------------------------------------
RETCODE DoPrintArrayPos(UCHAR fType[MAX_NAME_LEN], UDWORD crow,
		     UWORD rgfRowStatus[ROW_SIZE], SWORD sNumber_rc[ROW_SIZE],
		     UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN],lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*-------------------------------------------------------------------------
//| DoSQLSetPos:
//|     This test case will test the SQLSetPos access.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*-------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLSetPos(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	UCHAR                   szString[MAX_STR_LEN];
	SWORD			sNumber;
	SDWORD			cbNumber=SQL_NTS;
	SDWORD			cbString=SQL_NTS;

	UCHAR			szString_rc[ROW_SIZE][MAX_STR_LEN];
	SWORD			sNumber_rc[ROW_SIZE];
	SDWORD			cbNumber_rc[ROW_SIZE]={SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SDWORD			cbString_rc[ROW_SIZE]={SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};

	UWORD			rgfRowStatus[ROW_SIZE];

	UDWORD			irow, crow;
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
	while (irow < MAX_ROWS+1) {
		sNumber = (SWORD) irow;
		lstrcpy(szString,TXTCOPY);
		sprintf(buffer,"%d.",irow);
		lstrcat(szString,buffer);

		rc = SQLExecute(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute?");
		irow++;
	}
	szLogPrintf(lpSrvr, FALSE,"\tInsert into table (%s) -> %d. Values\r\n",
					TABLE, irow-1);

	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	rc = SQLAllocStmt(*phdbc, phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");

	rc = SQLSetStmtOption(*phstmt, SQL_CONCURRENCY, SQL_CONCUR_READ_ONLY);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption");
	rc = SQLSetStmtOption(*phstmt, SQL_CURSOR_TYPE, SQL_CURSOR_KEYSET_DRIVEN);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption");
	rc = SQLSetStmtOption(*phstmt, SQL_ROWSET_SIZE, ROW_SIZE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption");

	lstrcpy(select,"SELECT ");
	lstrcat(select," NUM, STRING ");
	lstrcat(select," FROM ");
	lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	if (rc == SQL_SUCCESS) {
	   rc = SQLBindCol(*phstmt, 1, SQL_C_SSHORT, &sNumber_rc, sizeof(SWORD), cbNumber_rc);
	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");
	   rc = SQLBindCol(*phstmt, 2, SQL_C_CHAR, szString_rc, MAX_STR_LEN, cbString_rc);
	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	/* ***** SQL_FETCH_NEXT *** ----------------------------------------- */
	   rc = SQLExtendedFetch(*phstmt, SQL_FETCH_NEXT, 1, &crow, rgfRowStatus);
  	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExtendedFetch");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		DoPrintArrayPos("SQL_FETCH_NEXT", crow, rgfRowStatus, sNumber_rc,
		             szString_rc, lpSrvr);
	   }
	/* ***** SQLSetPos (SQL_POSITION) *** ------------------------------- */
	   rc = SQLSetPos(*phstmt, SETPOS1, SQL_POSITION, SQL_LOCK_NO_CHANGE);
  	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos (SQL_POSITION)");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	 	szLogPrintf(lpSrvr, FALSE,"-> Set position to row : %d\r\n",SETPOS1);
		DoPrintArrayPos("SQL_POSITION", crow, rgfRowStatus, sNumber_rc,
		             szString_rc, lpSrvr);
	   }

	/* ***** SQLSetPos (SQL_REFRESH) *** -------------------------------- */
	   rc = SQLSetPos(*phstmt, SETPOS2, SQL_REFRESH, SQL_LOCK_NO_CHANGE);
  	   ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos (SQL_REFRESH)");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	 	szLogPrintf(lpSrvr, FALSE,"-> Set position to row : %d\r\n",SETPOS2);
		DoPrintArrayPos("SQL_REFRESH", crow, rgfRowStatus, sNumber_rc,
		             szString_rc, lpSrvr);
	   }
	/* ------------------------------------------------------------------ */
	}
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

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

RETCODE DoPrintArrayPos(UCHAR fType[MAX_NAME_LEN], UDWORD crow,
		     UWORD rgfRowStatus[ROW_SIZE], SWORD sNumber_rc[ROW_SIZE],
		     UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN],lpSERVERINFO lpSrvr)
{
 UCHAR                   buffer[MAX_NAME_LEN+1];
 UDWORD			 irow;

 szLogPrintf(lpSrvr, FALSE,"\t%s -> Rows fetch : %d.\r\n", fType, crow);
 szLogPrintf(lpSrvr, FALSE,"\tRow | RowStatus | Number | String\r\n");
 for (irow=0; irow < crow; irow++) {
	sprintf(buffer,"\t %d.          %d",irow+1,rgfRowStatus[irow]);
	szLogPrintf(lpSrvr, FALSE,"%s", buffer);
	if (rgfRowStatus[irow] != SQL_ROW_DELETED &&
	    rgfRowStatus[irow] != SQL_ROW_ERROR) {
	    sprintf(buffer,"                      %d              %s",
			sNumber_rc[irow], szString_rc[irow]);
	    szLogPrintf(lpSrvr, FALSE,"%s",buffer);
	}
 	szLogPrintf(lpSrvr, FALSE,"\r\n");
 }
 return(0);
}
/* ------------------------------------------------------------------------- */
