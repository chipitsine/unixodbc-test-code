/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : LONGVAR2.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von LONG-Spalten 2.
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "LONGVARS2"
#define TABLE "TABLONG2"

#define MAX_NAME_LEN 50
#define MAX_LONG     260

//*-------------------------------------------------------------------------

int PrintString(UCHAR *, DWORD, UCHAR *, lpSERVERINFO);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoLongDatatype:
//|     This test case will test the long datatypes write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoLongDatatype2(HENV FAR * phenv, HDBC FAR * phdbc,
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

	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   colname[MAX_NAME_LEN+1];

	HGLOBAL		        hglb;
	UCHAR FAR	             *value;
	UCHAR FAR	             *rc_value;
	UCHAR                   str[60];
	SDWORD                  cbvalue=SQL_NTS;

	SWORD                   i=1,z;
	struct _timeb           timebuf1, timebuf2;
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
	lstrcpy(tabname, TABLE);
	lstrcpy(colname," ( TYPELONG LONG ASCII)");
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname); lstrcat(create,colname);
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Create Table", lpSrvr, 0);/*Output time*/

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");lstrcat(insert,tabname);
	lstrcat(insert," VALUES (?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	hglb = GlobalAlloc(GPTR, MAX_LONG);
	value = GlobalLock(hglb);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_LONGVARCHAR, MAX_LONG, 0, value, 0, &cbvalue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"BindParameter");

	lstrcpy(value,"LONG:-");
	lstrcpy(str,"0123456789");
	i=1;
	while (i < 27)  {
		if (i != 1) lstrcat(value,str);
		z = lstrlen(value) - 1;
		sprintf(buffer, "\tInsert Value (%d Z.) %d.: ->", z,i);
		PrintString(value, lstrlen(value), buffer, lpSrvr);

		rc = SQLExecute(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

		i++;
	}

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Insert Table", lpSrvr, 0);/*Output time*/

	GlobalUnlock(hglb);
	GlobalFree(hglb);
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(colname,"TYPELONG");
	lstrcpy(select,"SELECT ");
	lstrcat(select,colname);
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	hglb = GlobalAlloc(GPTR, MAX_LONG);
	rc_value = GlobalLock(hglb);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc=SQLBindCol(*phstmt,1,SQL_C_CHAR,rc_value,MAX_LONG,&cbvalue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol");

	i=1;
	while (1) {
		rc = SQLFetch(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

		if (rc == SQL_NO_DATA_FOUND) break;

		z = lstrlen(rc_value) - 1;
		if (cbvalue == SQL_NULL_DATA) rc_value[0]='\0';
		sprintf(buffer, "\tSelect Value (%d Z.) %d.: ->", z,i);
		PrintString(rc_value, lstrlen(rc_value), buffer, lpSrvr);
		i++;
	}
	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Select Table", lpSrvr, 0);/*Output time*/

	GlobalUnlock(hglb);
	GlobalFree(hglb);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	lstrcpy(drop,"DROP TABLE ");lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Drop Table", lpSrvr, 0);/*Output time*/
#endif
	/* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */

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
