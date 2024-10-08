/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : CURSNAME.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test Cursorname
**
******************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLSetCursorName"
#define TABLE "TABCURSOR"
#define CURNAME "ADABAS_CUR"

#define MAX_NAME_LEN 30
#define MAX_STR_LEN 256

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*-------------------------------------------------------------------------
//| DoCursname:
//|     This test case will test the mfetch access.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*-------------------------------------------------------------------------
SWORD FAR PASCAL DoCursorName(HENV FAR * phenv, HDBC FAR * phdbc,
                                HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
        RETCODE                 rc;
        SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
        UCHAR                   create[MAX_STR_LEN];
        UCHAR                   drop[MAX_STR_LEN+1];
        UCHAR                   insert[MAX_STR_LEN+1];
        UCHAR                   select[MAX_STR_LEN+1];
        UCHAR                   update[MAX_STR_LEN+1];

	HSTMT			hstmtSelect=0;
	HSTMT			hstmtUpdate=0;

	UCHAR			szName[MAX_NAME_LEN+1];
	SWORD			sAge;
	SDWORD			cbName;
	SDWORD			cbAge;
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
	ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLConnect");
/* ---------------------------------------------------------------------har- */
        rc = SQLAllocStmt(*phdbc, phstmt);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");

	/* ***** Allocate the statements and set cursir name --------------- */
        rc = SQLAllocStmt(*phdbc, &hstmtSelect);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");

        rc = SQLAllocStmt(*phdbc, &hstmtUpdate);
        ERRORCHECK(*phenv,*phdbc,hstmtUpdate,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");
/* ---------------------------------------------------------------------har- */
	rc = SQLSetCursorName(hstmtSelect, CURNAME, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLSetCursor");
/* ---------------------------------------------------------------------har- */
        /* --- Create Table --------------------------------------------- */
        lstrcpy(create,"CREATE TABLE "TABLE);
        /* lstrcat(create,TABLE); */
        lstrcat(create," ( NAME CHAR(60), AGE SMALLINT, ADRESS CHAR(80) )");
        szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

        rc = SQLExecDirect(*phstmt, create, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

        /* --- Insert Table --------------------------------------------- */
        /* --- 1. ---*/
        lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE);
        lstrcat(insert," ( NAME, AGE ) ");
        lstrcat(insert," VALUES ('Christian Meier', 32 )");
        szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 1. = '%s'\r\n",insert);

        rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

        /* --- 2. ---*/
        lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE);
        lstrcat(insert," ( NAME, AGE ) ");
        lstrcat(insert," VALUES ('Hans Mueller', 45 )");
        szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 2. = '%s'\r\n",insert);

        rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

        /* --- 3. ---*/
        lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE);
        lstrcat(insert," ( NAME, AGE ) ");
        lstrcat(insert," VALUES ('Kerstin Frank', 28 )");
        szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 3. = '%s'\r\n",insert);

        rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

        /* --- Select Table --------------------------------------------- */
	/* ** Select the result set and bind its columns to local storage */

        /* --- 1. --- */
        lstrcpy(select,"SELECT ");
        lstrcat(select," NAME, AGE ");
        lstrcat(select," FROM ");
        lstrcat(select,TABLE);
/* --- mysql ????
        lstrcat(select," FOR UPDATE OF");
*/
        szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

        rc = SQLExecDirect(hstmtSelect, select, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = SQLBindCol(hstmtSelect, 1, SQL_C_CHAR, szName, MAX_NAME_LEN, &cbName);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLBindCol(hstmtSelect, 2, SQL_C_SHORT, &sAge, 0, &cbAge);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	/* Read through the result set until the cursor is positioned   */
	/* on the first row  						*/
	do {
		rc = SQLFetch(hstmtSelect);
		ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");
		break;
	 } while (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);

        szLogPrintf(lpSrvr, FALSE,"\tCurrent row : NAME= '%s' \r\n",szName);

	/* Perform a positioned update of John Smith's name*/
        /* --- 2. --- */
        lstrcpy(update,"UPDATE ");
        lstrcat(update,TABLE);
        lstrcat(update," SET NAME= 'Meier, Chris' ");
        lstrcat(update," WHERE CURRENT OF ");
        lstrcat(update,CURNAME);
        szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",update);

	if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		rc = SQLExecDirect(hstmtUpdate, update, SQL_NTS);
	        ERRORCHECK(*phenv,*phdbc,hstmtUpdate,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
	}

        rc = SQLFreeStmt(hstmtUpdate, SQL_CLOSE);
        ERRORCHECK(*phenv,*phdbc,hstmtUpdate,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

 	/* --- GetData new ------------------------------------------------- */
        rc = SQLFreeStmt(hstmtSelect, SQL_CLOSE);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

        rc = SQLExecDirect(hstmtSelect, select, SQL_NTS);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = SQLBindCol(hstmtSelect, 1, SQL_C_CHAR, szName, MAX_NAME_LEN, &cbName);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	rc = SQLBindCol(hstmtSelect, 2, SQL_C_SHORT, &sAge, 0, &cbAge);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	while (1)  {
		rc = SQLFetch(hstmtSelect);
		ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

		if (rc == SQL_NO_DATA_FOUND) break;

	        szLogPrintf(lpSrvr, FALSE,"\tCurrent row : NAME= '%s' \r\n",szName);
	 }

        rc = SQLFreeStmt(hstmtSelect, SQL_CLOSE);
        ERRORCHECK(*phenv,*phdbc,hstmtSelect,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

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
        /* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */
        /* rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK); */
        rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");

/* ---------------------------------------------------------------------har- */
        rc = SQLFreeStmt(*phstmt, SQL_DROP);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

        rc = SQLFreeStmt(hstmtSelect, SQL_DROP);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

        rc = SQLFreeStmt(hstmtUpdate, SQL_DROP);
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


/* ************************************************************************ */
