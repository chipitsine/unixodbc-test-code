/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLFETSC.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLFetchScroll
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLFetchScroll"

#define TABLE "FETCHSCROLL"

#define MAX_NAME_LEN	80
#define MAX_STR_LEN	30
#define MAX_ROWS	40
#define ROW_SIZE	8
#define ROW_POS		20

#define TXTCOPY "SQLFetchScroll String "

/* ------------------------------------------------------------------------- */
RETCODE DoPrintArray(UCHAR fType[MAX_NAME_LEN],  UDWORD crow,
		     SWORD sNumber_rc[ROW_SIZE], UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN],
		     lpSERVERINFO lpSrvr);

/* ------------------------------------------------------------------------- */

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLFetchScroll:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLFetchScroll(HENV FAR * phenv, HDBC FAR * phdbc,
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

	UDWORD			irow;
	SWORD			   cAbort=0;
	SQLINTEGER		BufferLength;
	SQLUINTEGER    NumRowsFetched;
	SQLSMALLINT		RowStatusArray[ROW_SIZE];

/* ------------------------------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	/* - Connect ------------------------------------------------------- */
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (HENV)");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_VERSION : 'SQL_OV_ODBC3' \r\n");

	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			                  lpSrvr->szValidLogin0, SQL_NTS,
			                  lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	/* ----------------------------------------------------------------- */

	/* --- Create Table --------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE);
	lstrcat(create," ( NUM SMALLINT, STRING CHAR(30)) ");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE);
	lstrcat(insert," VALUES (?, ?) ");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_INTEGER, 0, 0, &sNumber, 0, &cbNumber);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
		SQL_CHAR, MAX_STR_LEN, 0, szString, MAX_STR_LEN, &cbString);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

	irow=1;
	cbString=SQL_NTS; cbNumber=SQL_NTS;
	szLogPrintf(lpSrvr, FALSE,"\tInsert Values ->\r\n");
	while (irow < MAX_ROWS+1) {
		sNumber = (SWORD) irow;
		lstrcpy(szString,TXTCOPY);
		sprintf(buffer,"%d.",irow);
		lstrcat(szString,buffer);

		rc = SQLExecute(*phstmt);
		DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecute");
		irow++;
	}
	szLogPrintf(lpSrvr, FALSE,"\tInsert into table (%s) -> %d. Values\r\n",
					TABLE, irow-1);

	rc = SQLFreeHandle(SQL_HANDLE_STMT, *phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	BufferLength = SQL_IS_UINTEGER;
	rc = SQLSetStmtAttr(*phstmt, SQL_CONCURRENCY, (SQLUINTEGER *) SQL_CONCUR_READ_ONLY, BufferLength);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_CONCURRENCY)");

	BufferLength = SQL_IS_UINTEGER;
	rc = SQLSetStmtAttr(*phstmt, SQL_CURSOR_TYPE, (SQLUINTEGER *) SQL_CURSOR_KEYSET_DRIVEN, BufferLength);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_CURSOR_TYPE)");

	BufferLength = SQL_IS_UINTEGER;
	rc = SQLSetStmtAttr(*phstmt, SQL_ROWSET_SIZE, (SQLUINTEGER *) ROW_SIZE, BufferLength);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_ROWSET_SIZE)");

   /* ***** neues in ODBC 3.0 */
   SQLSetStmtAttr(*phstmt, SQL_ATTR_ROW_BIND_TYPE, SQL_BIND_BY_COLUMN, 0);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_ATTR_ROW_BIND_TYPE)");

   SQLSetStmtAttr(*phstmt, SQL_ATTR_ROW_ARRAY_SIZE, (void *)ROW_SIZE, 0);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_ATTR_ROW_ARRAY_SIZE)");

   SQLSetStmtAttr(*phstmt, SQL_ATTR_ROW_STATUS_PTR, RowStatusArray, 0);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_ATTR_ROW_STATUS_PTR)");

   SQLSetStmtAttr(*phstmt, SQL_ATTR_ROWS_FETCHED_PTR, &NumRowsFetched, 0);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr(SQL_ATTR_ROWS_FETCHED_PTR)");

	lstrcpy(select,"SELECT ");
	lstrcat(select," NUM, STRING ");
	lstrcat(select," FROM ");
	lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	if (rc == SQL_SUCCESS) {
	   rc = SQLBindCol(*phstmt, 1, SQL_C_SSHORT, &sNumber_rc, sizeof(SWORD), cbNumber_rc);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindCol");
	   rc = SQLBindCol(*phstmt, 2, SQL_C_CHAR, szString_rc, MAX_STR_LEN, cbString_rc);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindCol");

	/* ***** SQL_FETCH_NEXT *** ----------------------------------------- */
	   while (1) {
	    rc = SQLFetchScroll(*phstmt, SQL_FETCH_NEXT, 1);
	    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
		DoPrintArray("SQL_FETCH_NEXT", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	    } else { break; }
	   }
	/* ***** SQL_FETCH_FIRST *** ---------------------------------------- */
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_FIRST, 1);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	     DoPrintArray("SQL_FETCH_FIRST", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ***** SQL_FETCH_PRIOR *** ---------------------------------------- */
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_NEXT, 1);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_NEXT, 1);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_PRIOR, 1);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	     DoPrintArray("SQL_FETCH_PRIOR", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ***** SQL_FETCH_LAST  *** ---------------------------------------- */
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_LAST, 1);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	     DoPrintArray("SQL_FETCH_LAST", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ***** SQL_FETCH_ABSOLUTE *** ------------------------------------- */
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_ABSOLUTE, ROW_POS);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	     DoPrintArray("SQL_FETCH_ABSOLUTE", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ***** SQL_FETCH_RELATIVE *** ------------------------------------- */
	   rc = SQLFetchScroll(*phstmt, SQL_FETCH_RELATIVE, 0);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	     DoPrintArray("SQL_FETCH_RELATIVE", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	   }
	/* ***** SQL_FETCH_BOOKMARK *** ------------------------------------- */
	/* rc = SQLFetchScroll(*phstmt, SQL_FETCH_BOOKMARK, ROW_POS);
	   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetchScroll");

	   if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
	     DoPrintArray("SQL_FETCH_BOOKMARK", ROW_SIZE, sNumber_rc, szString_rc, lpSrvr);
	   }
	*/
	/* ------------------------------------------------------------------ */
	}
	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");


	/* - Disconnect ---------------------------------------------------- */
	rc = SQLFreeHandle(SQL_HANDLE_STMT, *phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");

	rc = SQLDisconnect(*phdbc);
   	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDisconnect");

	rc = SQLFreeHandle(SQL_HANDLE_DBC, *phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HDBC)");

	rc = SQLFreeHandle(SQL_HANDLE_ENV, *phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HENV)");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------- */

RETCODE DoPrintArray(UCHAR fType[MAX_NAME_LEN], UDWORD crow,
		     SWORD sNumber_rc[ROW_SIZE], UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN],
		     lpSERVERINFO lpSrvr)
{
 UCHAR                   buffer[MAX_NAME_LEN+1];
 UDWORD			 irow;

 szLogPrintf(lpSrvr, FALSE,"\t%s -> Rows fetch : %d.\r\n", fType, crow);
 szLogPrintf(lpSrvr, FALSE,"\tRow | Number | String\r\n");
 for (irow=0; irow < crow; irow++) {
	sprintf(buffer,"\t %d. : ",irow+1);
	szLogPrintf(lpSrvr, FALSE,"%s", buffer);
	sprintf(buffer,"      %d               %s",
		sNumber_rc[irow], szString_rc[irow]);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);
 	szLogPrintf(lpSrvr, FALSE,"\r\n");
 }

 return(0);
}
/* ------------------------------------------------------------------------- */
