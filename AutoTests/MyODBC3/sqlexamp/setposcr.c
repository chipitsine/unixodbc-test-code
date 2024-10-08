/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : setposcr.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSetPos, Update current of cursor
**
******************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "sqlexamp.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SetPosCur"
#define TABLE "SETPOSCR"

#define MAX_NAME_LEN	512
#define MAX_STR_LEN	30
#define MAX_ROWS	20
#define ROW_SIZE	10

#define SETPOS1		2
#define SETPOS2		4
#define SETPOS3		6
#define SETPOS4		8
#define SETPOS5		10

#define TXTCOPY "String "
#define UPDCOPY "Update "

#define COPYUPD(update, szCursor, pos) \
{ \
	   lstrcpy(update,"UPDATE "); \
	   lstrcat(update,TABLE); \
	   lstrcat(update," SET \"STRING\"='"); \
	   lstrcat(update,UPDCOPY); \
	   lstrcat(update,pos); \
	   lstrcat(update,".' WHERE CURRENT OF "); \
  	   lstrcat(update,szCursor); \
}

/* ------------------------------------------------------------------------ */
RETCODE DoPrintArrayPos(UCHAR fType[MAX_NAME_LEN], UDWORD crow,
		     UWORD rgfRowStatus[ROW_SIZE], SWORD sNumber_rc[ROW_SIZE],
		     UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN],
		     lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSetposCurrent:
//|     This test case will test integer numbers write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSetPosCurrent(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   update[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];
	UCHAR                   buf[MAX_NAME_LEN+1];

	UCHAR                   szString[MAX_STR_LEN];
	UCHAR                   szCursor[MAX_STR_LEN];
	SWORD			sNumber;
	SDWORD			cbNumber=SQL_NTS;
	SDWORD			cbString=SQL_NTS;
	SWORD			cbCursor=SQL_NTS;

	UCHAR			szString_rc[ROW_SIZE][MAX_STR_LEN];
	SWORD			sNumber_rc[ROW_SIZE];
	SDWORD			cbNumber_rc[ROW_SIZE]={SQL_NTS, SQL_NTS,SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SDWORD			cbString_rc[ROW_SIZE]={SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};

	UWORD			rgfRowStatus[ROW_SIZE];

	UDWORD			irow, crow;

	HSTMT 	   		hstmt1;
	HSTMT      		hstmt2;
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

/* --- SQLSetPos -------------------------------------------------------- */
	/* --- Create Table --------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE);
	lstrcat(create," (NUM SMALLINT, STRING CHAR(30)) ");
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
	sprintf(buffer,"Insert Values ->");
	szLogPrintf(lpSrvr, FALSE,"\t%s\r\n",buffer);

	while (irow < MAX_ROWS+1) {
		sNumber = (SWORD) irow;
		lstrcpy(szString,TXTCOPY);
		sprintf(buf,"%d.",irow);
		lstrcat(szString,buf);

		rc = SQLExecute(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute?");
		irow++;
	}
	szLogPrintf(lpSrvr, FALSE,"\tInsert into table (%s) ->%d. Values\r\n",TABLE,irow-1);

	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	rc = SQLAllocStmt(*phdbc, &hstmt1);
	ERRORCHECK(*phenv,*phdbc, hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 1.");

	rc = SQLAllocStmt(*phdbc, &hstmt2);
	ERRORCHECK(*phenv,*phdbc, hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 2.");

	/* ***** SetStmtOption *** ----------------------------------------- */

	rc = SQLSetStmtOption(hstmt1, SQL_CONCURRENCY, SQL_CONCUR_VALUES);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption (SQL_CONCUR_VALUES)");

	rc = SQLSetStmtOption(hstmt1, SQL_CURSOR_TYPE, SQL_CURSOR_DYNAMIC);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption (SQL_CURSOR_DYNAMIC)");

	rc = SQLSetStmtOption(hstmt1, SQL_ROWSET_SIZE, ROW_SIZE);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption (SQL_ROWSET_SIZE)");

	/* ----------------------------------------------------------------- */
	lstrcpy(select,"SELECT ");
	lstrcat(select," \"NUM\", \"STRING\" ");
	lstrcat(select," FROM ");
	lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(hstmt1, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	if (rc == SQL_SUCCESS) {
	   rc = SQLBindCol(hstmt1,1,SQL_C_SSHORT,&sNumber_rc,sizeof(SWORD),
			   cbNumber_rc);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	   rc = SQLBindCol(hstmt1,2,SQL_C_CHAR,szString_rc,MAX_STR_LEN,
			   cbString_rc);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	/* ***** GetCursorName *** ----------------------------------------- */
	   rc = SQLGetCursorName(hstmt1, szCursor, MAX_STR_LEN, &cbCursor);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLGetCursorName");

	   szLogPrintf(lpSrvr, FALSE,"\t -> CursorName = '%s'\r\n",szCursor);

	/* ***** SQL_FETCH_NEXT *** ---------------------------------------- */
	   rc = SQLExtendedFetch(hstmt1,SQL_FETCH_NEXT,1,&crow,rgfRowStatus);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLExentedFetch");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		DoPrintArrayPos("SQL_FETCH_NEXT", crow, rgfRowStatus,
			        sNumber_rc, szString_rc, lpSrvr);
	   }

	/* ***** Update Table Stmt *** ------------------------------------ */
 	   COPYUPD (update, szCursor, "2");
	   szLogPrintf(lpSrvr, FALSE,"\tUpdate Stmt= '%s'\r\n",update);

	/* ***** SetPos 1. and Update 1. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS1, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 1.(Pos.:2)");

 	   COPYUPD(update, szCursor, "2");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 1.(Update)");

	/* ***** SetPos 2. and Update 2. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS2, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 2.(Pos.:4)");

 	   COPYUPD(update, szCursor, "4");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 2.(Update)");

	/* ***** SetPos 3. and Update 3. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS3, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 3.(Pos.:6)");

 	   COPYUPD(update, szCursor, "6");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 3.(Update)");

	/* ***** SetPos 4. and Update 4. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS4, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 4.(Pos.:8)");

 	   COPYUPD(update, szCursor, "8");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 4.(Update)");

	/* ***** SetPos 5. and Update 5. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS5, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 5.(Pos.:10)");

 	   COPYUPD(update, szCursor, "10");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 5.(Update)");

	/* ***** SQL_FETCH_NEXT *** ---------------------------------------- */
	   rc = SQLExtendedFetch(hstmt1,SQL_FETCH_NEXT,1,&crow,rgfRowStatus);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLExentedFetch");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		DoPrintArrayPos("SQL_FETCH_NEXT", crow, rgfRowStatus,
			        sNumber_rc, szString_rc, lpSrvr);
	   }

	/* ***** SetPos 6. and Update 6. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS1, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 6.(Pos.:12)");

 	   COPYUPD(update, szCursor, "12");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 6.(Update)");

	/* ***** SetPos 7. and Update 7. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS2, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 7.(Pos.:14)");

 	   COPYUPD(update, szCursor, "14");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 7.(Update)");

	/* ***** SetPos 8. and Update 8. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS3, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 1.(Pos.:16)");

 	   COPYUPD(update, szCursor, "16");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 8.(Update)");

	/* ***** SetPos 9. and Update 9. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS4, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 9.(Pos.:18)");

 	   COPYUPD(update, szCursor, "18");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 9.(Update)");

	/* ***** SetPos 10. and Update 10. *** --------------------------- */
	   rc = SQLSetPos(hstmt1, SETPOS5, SQL_POSITION, SQL_LOCK_NO_CHANGE);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLSetPos 10.(Pos.:20)");

 	   COPYUPD(update, szCursor, "20");
	   rc = SQLExecDirect(hstmt2, update, SQL_NTS);
	   ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 10.(Update)");
	/* ----------------------------------------------------------------- */
	/* ***** SQL_FETCH_FIRST *** --------------------------------------- */
	   rc = SQLExtendedFetch(hstmt1,SQL_FETCH_FIRST,1,&crow,rgfRowStatus);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLExentedFetch");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		DoPrintArrayPos("SQL_FETCH_NEXT", crow, rgfRowStatus,
			        sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ***** SQL_FETCH_NEXT *** ---------------------------------------- */
	   rc = SQLExtendedFetch(hstmt1,SQL_FETCH_NEXT,1,&crow,rgfRowStatus);
	   ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLExentedFetch");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		DoPrintArrayPos("SQL_FETCH_NEXT", crow, rgfRowStatus,
			        sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ----------------------------------------------------------------- */
 	}
	rc = SQLFreeStmt(hstmt1, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 1.");

	rc = SQLFreeStmt(hstmt2, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 2.");

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE);
   	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(hstmt1, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

/* ---------------------------------------------------------------------har- */
	rc = SQLFreeStmt(hstmt1, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLFreeStmt(hstmt2, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,hstmt2,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,hstmt1,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}

/* ------------------------------------------------------------------------- */
RETCODE DoPrintArrayPos(UCHAR fType[MAX_NAME_LEN], UDWORD crow,
		     UWORD rgfRowStatus[ROW_SIZE], SWORD sNumber_rc[ROW_SIZE],
		     UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN],
		     lpSERVERINFO lpSrvr)
{
 UDWORD			 irow;
 UCHAR                   buffer[MAX_NAME_LEN+1];

 szLogPrintf(lpSrvr, FALSE,"\t %s -> Rows fetch : %d.\r\n", fType, crow);
 szLogPrintf(lpSrvr, FALSE,"\t Row | RowStatus | Number | String\r\n");

 for (irow=0; irow < crow; irow++) {
	sprintf(buffer,"  %d.          %d",irow+1,rgfRowStatus[irow]);
        szLogPrintf(lpSrvr, FALSE," '%s'",buffer);
	if (rgfRowStatus[irow] != SQL_ROW_DELETED &&
	    rgfRowStatus[irow] != SQL_ROW_ERROR) {
	    sprintf(buffer,"                      %d              %s",
			sNumber_rc[irow], szString_rc[irow]);
	    szLogPrintf(lpSrvr, FALSE," '%s'",buffer);
	}
	szLogPrintf(lpSrvr, FALSE,"\r\n");
 }
 return(0);
}
/* ------------------------------------------------------------------------- */
