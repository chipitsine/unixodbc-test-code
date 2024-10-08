/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : MFETCH.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von MFETCH, Zeitmessung mit/ohne MFETCH
**
******************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "MFETCH"
#define TABLE "MFETCH_TEST"

#define MAX_NAME_LEN 120
#define MAX_DATE     10
#define MAX_ROWS     1001

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*-------------------------------------------------------------------------
//| DoMFetchTest:
//|     This test case will test the mfetch access.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*-------------------------------------------------------------------------
SWORD FAR PASCAL DoMFetchTest(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];

	UCHAR                   tabname[MAX_NAME_LEN+1];

	UCHAR                   buffer[MAX_NAME_LEN+1];
	UCHAR                   typ_char[MAX_NAME_LEN];
	SDOUBLE                 typ_float;
	DATE_STRUCT             typ_date;

	SDWORD                  cbChar = SQL_NTS;
	SDWORD                  cbFloat = SQL_NTS;
	SDWORD                  cbDate = SQL_NTS;

	SWORD                   i=1;
	struct _timeb           timebuf1, timebuf2;


	UDWORD			pAutoCommit;
	SWORD                   opt=NO_OUTPUT;
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

	/* --- SQLSetConnectOption ----------------------------------------- */
	i=0;
	/* *** SQL_AUTOCOMMIT ------------------ *** */
	rc = SQLGetConnectOption(*phdbc, SQL_AUTOCOMMIT, &pAutoCommit);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption");

	if (pAutoCommit==SQL_AUTOCOMMIT_ON) lstrcpy(buffer,"SQL_AUTOCOMMIT_ON");
	    else if (pAutoCommit==SQL_AUTOCOMMIT_OFF) lstrcpy(buffer,"SQL_AUTOCOMMIT_OFF");
	szLogPrintf(lpSrvr,FALSE,"\t SQL_AUTOCOMMIT	: '%s' \r\n", buffer);

	if (pAutoCommit == SQL_AUTOCOMMIT_ON) {
		szLogPrintf(lpSrvr,FALSE,"\t Set SQL_AUTOCOMMIT to OFF \r\n");
		pAutoCommit =  SQL_AUTOCOMMIT_OFF;

		rc = SQLSetConnectOption( *phdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetConnectOption");

		rc = SQLGetConnectOption( *phdbc, SQL_AUTOCOMMIT, &pAutoCommit);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption");

		if (pAutoCommit==SQL_AUTOCOMMIT_ON) lstrcpy(buffer,"SQL_AUTOCOMMIT_ON");
		    else if (pAutoCommit==SQL_AUTOCOMMIT_OFF) lstrcpy(buffer,"SQL_AUTOCOMMIT_OFF");
		szLogPrintf(lpSrvr,FALSE,"\t SQL_AUTOCOMMIT	: '%s' \r\n", buffer);
	}
/* ---------------------------------------------------------------------har- */
	rc = SQLAllocStmt(*phdbc, phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt");

/* ---------------------------------------------------------------------har- */

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," ( TYP_CHAR CHAR(120), TYP_FLOAT FLOAT, TYP_DATE DATE)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Create Table", lpSrvr, 0);/*Output time*/

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");lstrcat(insert,tabname);
	lstrcat(insert," ( TYP_CHAR, TYP_FLOAT, TYP_DATE) ");
	lstrcat(insert," VALUES (?, ? ,?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	sprintf(typ_char,"<< Dies ist ein MFETCH-Test >> -> %d.",i);
	typ_float = 999;
	typ_date.year  = 1994;typ_date.month = 11;typ_date.day = 1;
	sprintf(buffer, "1.->'%s' 2.->'%.0f' 3.->'%d.%d.%d'", typ_char, typ_float,
			typ_date.day, typ_date.month, typ_date.year);
	szLogPrintf(lpSrvr,FALSE, "\tInsert Values %d.: %s \r\n", i, buffer);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare?");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_CHAR, MAX_NAME_LEN, 0, &typ_char, 0, &cbChar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter?");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE,
		SQL_DOUBLE, 0, 0, &typ_float, sizeof(typ_float), &cbFloat);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter?");

	rc = SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_DATE,
			SQL_DATE, 0, 0, &typ_date, 0, &cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter?");

	i=1;
	while (i < MAX_ROWS) {
		if (opt != NO_OUTPUT)
			szLogPrintf(lpSrvr,FALSE, "\tInsert Value %d.\r\n", i);

		rc = SQLExecute(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute?");

		i++;
		sprintf(typ_char,"<< Dies ist ein MFETCH-Test >> -> %d.",i);
		typ_float += 100;
		typ_date.day++; if (typ_date.day == 31) typ_date.day=1;
	}
	GetTime(&timebuf2);       /* Get finish time struct */

	sprintf(buffer,"Insert Table  (%d. rows)",i-1);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time*/

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT ");
	lstrcat(select," TYP_CHAR, TYP_FLOAT, TYP_DATE ");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect?");

	rc = SQLBindCol(*phstmt,1,SQL_C_CHAR,&typ_char,MAX_NAME_LEN,&cbChar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol?");

	rc = SQLBindCol(*phstmt,2,SQL_C_DOUBLE,&typ_float,sizeof(typ_float),&cbFloat);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol?");

	rc = SQLBindCol(*phstmt,3,SQL_C_DATE,&typ_date,0,&cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol?");

	i=1;
	while (1)  {
		rc = SQLFetch(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch?");

		if (rc == SQL_NO_DATA_FOUND) break;

		if (opt != NO_OUTPUT) {
			sprintf(buffer, "1.->'%s' 2.->'%.0f' 3.->'%d.%d.%d'",
				typ_char, typ_float,typ_date.day,
				typ_date.month, typ_date.year);
			szLogPrintf(lpSrvr,FALSE,
				"\tSelect Values %d.: %s \r\n", i, buffer);
		}
		i++;
	}
	GetTime(&timebuf2);       /* Get finish time struct */

	sprintf(buffer,"Select Table  (%d. rows)",i-1);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time*/

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
