/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLCLCRS.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLCLOSECURSOR
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLCloseCursor"

#define TABLE1	"TABCLOSECUR1"
#define TABLE2	"TABCLOSECUR2"

#define SQLSTMT1 "SELECT * FROM DUAL"

#define MAX_ROWS 5			/* Max. Zeilen			*/
#define MAX_NAME_LEN 256
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLCloseCursor:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLCloseCursor(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;

	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];

        UCHAR           	szCharData[MAX_NAME_LEN];
	SWORD			i,j,count=0;
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
	/* --- Create Table 1. --------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE1);
	lstrcat(create," ( NAME CHAR(50), AGE INTEGER, ADRESSE CHAR(80))\0");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 1. = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	/* --- Create Table 2. ------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE2);
	lstrcat(create," ( KUNDE CHAR(50), ADRESSE CHAR(80), PLZ INTEGER) \0");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 2. = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");


	/* --- Insert Table 1. ------------------------------------------- */
	 lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE1);
	 lstrcat(insert," VALUES ('Heinrich', 44, 'Alt-Moabit 96, Berlin')");

	 for (j=0; j < MAX_ROWS; j++) {
	  szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 1. = <%d.> '%s'\r\n",j+1,insert);

	  rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	  rc = SQLCloseCursor(*phstmt);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");
	 }
	/* --- Insert Table 2. ------------------------------------------- */
	 lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE2);
	 lstrcat(insert," VALUES ('Matzke', 'Turmstr. 98, Berlin', 10559)");

	 for (j=0; j < MAX_ROWS; j++) {
	  szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 2. = <%d.> '%s'\r\n",j+1,insert);

	  rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	  rc = SQLCloseCursor(*phstmt);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");
	 }

	/* --- Select Table 1. ------------------------------------------- */
	lstrcpy(select,"SELECT * FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 1. = '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
	count = 1; i=1;
	while (rc != SQL_NO_DATA_FOUND) {
	 rc = SQLFetch(*phstmt);
	 DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetch");

	 if (rc != SQL_NO_DATA_FOUND) {
	  rc = SQLGetData(*phstmt, i, SQL_C_CHAR, szCharData, sizeof(szCharData), NULL);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetData");

	  szLogPrintf(lpSrvr, FALSE,"\t Column %d (Pos.%d) = '%s'\r\n",i,count,szCharData);
	  count ++;
	 }
	}
	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	/* --- Select Table 2. ------------------------------------------- */
	lstrcpy(select,"SELECT * FROM ");lstrcat(select,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 2. = '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	rc = SQLFetch(*phstmt);
	if (rc == SQL_ERROR) rc--;
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetch");

	/* --- Drop Table 1. -------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");lstrcat(drop,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt 1.= '%s'\r\n",drop);

	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Drop Table 2. -------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");lstrcat(drop,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt 2.= '%s'\r\n",drop);

	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

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
