/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : CHUPDATE.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, CHAR-Spalte Update, mit Zeitmessung
**
******************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "comptest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "CHARUPDATE"
#define TABLE "CHARUPDATE"

#define MAX_NAME_LEN 120
#define MAX_CHAR_LEN 20
#define MAX_DATE     10
#define MAX_ROWS     1001
#define OUT_MSEC     100       	/* interval output in 0.1 sec */

//*-------------------------------------------------------------------------

#ifdef WIN32
char *_ltoa();
#endif

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*-------------------------------------------------------------------------
//| DoCharUpdate:
//|     This test case will test the update a char column.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*-------------------------------------------------------------------------
SWORD FAR PASCAL DoCharUpdate(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   update[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];

	UCHAR                   tabname[MAX_NAME_LEN+1];

	UCHAR                   buffer[MAX_NAME_LEN+1];
	UCHAR                   typ_char[MAX_CHAR_LEN];

	SDWORD                  cbChar = SQL_NTS;

	SWORD                  i=1,smsec=0;

	struct _timeb           timebuf1, timebuf2;
	struct _timeb           checktime;


	UDWORD			pAutoCommit;
	SWORD                   opt=NO_OUTPUT; /* OUTPUT o. NO_OUTPUT */
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
	lstrcat(create," (TYP_CHAR CHAR(20) )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Create Table", lpSrvr, 0);/*Output time*/

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");lstrcat(insert,tabname);
	lstrcat(insert," ( TYP_CHAR ) ");
	lstrcat(insert," VALUES (?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	i=1;
	sprintf(typ_char,"%d.",i);
	sprintf(buffer, "1.->'%s'", typ_char);
	szLogPrintf(lpSrvr,FALSE, "\tInsert Values %d.: %s \r\n", i, buffer);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_CHAR, MAX_NAME_LEN, 0, &typ_char, 0, &cbChar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	szLogPrintf(lpSrvr,FALSE, "\tInsert Value %d.\r\n", i);

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	GetTime(&timebuf2);       /* Get finish time struct */

	sprintf(buffer,"Insert Table  (%d. rows)",i);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time*/

	/* --- Update Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(update,"UPDATE ");lstrcat(update,tabname);
	lstrcat(update," SET TYP_CHAR = ");
	szLogPrintf(lpSrvr, FALSE,"\tUpdate 1. Stmt = '%s'\r\n",update);

	i=1;
	sprintf(typ_char,"%d.",i);
	sprintf(buffer, "1.->'%s'", typ_char);
	szLogPrintf(lpSrvr,FALSE, "\tUpdate Values %d.: %s \r\n", i, buffer);

	GetTime(&timebuf1);       /* Get start time struct  */

	i=1; smsec = 0;
	while (i < MAX_ROWS) {
		if (opt != NO_OUTPUT) {
	          GetTime(&checktime); if (smsec > checktime.millitm) smsec=0;
		  if (checktime.millitm >= (smsec+OUT_MSEC)) {
			szLogPrintf(lpSrvr,FALSE, "\tValue: %d.\r\n", i);
			smsec = checktime.millitm;
		  }
		}

 		_ltoa(i, typ_char, 10);
		sprintf(buffer,"%s'%s.'",update, typ_char);

		rc = SQLExecDirect(*phstmt, buffer, SQL_NTS);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect?");

		i++;
	}
	GetTime(&timebuf2);       /* Get finish time struct */

	sprintf(buffer,"Update Table  (%d. rows)",i-1);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time*/

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT ");
	lstrcat(select," TYP_CHAR ");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect 1. Stmt= '%s'\r\n",select);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=1;

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	if (rc != SQL_NO_DATA_FOUND) {
		rc = SQLGetData(*phstmt,1,SQL_C_CHAR,&typ_char,MAX_NAME_LEN,
				&cbChar);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	}
	else lstrcpy(typ_char,"<NULL>");

	sprintf(buffer, "1.->'%s'",typ_char);
	szLogPrintf(lpSrvr,FALSE,
				"\tSelect Values %d.: %s \r\n", i, buffer);

	GetTime(&timebuf2);       /* Get finish time struct */

	sprintf(buffer,"Select Table  (%d. rows)",i);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time*/

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Update Table --------------------------------------------- */
	/* --- 2. ---*/
	/* --- mysql ???
	lstrcpy(update,"UPDATE ");lstrcat(update,tabname);
	lstrcat(update," ( TYP_CHAR ) VALUES (?)");
	szLogPrintf(lpSrvr, FALSE,"\tUpdate 2. Stmt = '%s'\r\n",update);

	i=1;
	sprintf(typ_char,"%d.",i);
	sprintf(buffer, "1.->'%s'", typ_char);
	szLogPrintf(lpSrvr,FALSE, "\tUpdate Values %d.: %s \r\n", i, buffer);

	GetTime(&timebuf1);       /* Get start time struct  * /

	rc = SQLPrepare(*phstmt, update, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare?");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_CHAR, MAX_NAME_LEN, 0, &typ_char, 0, &cbChar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter?");

	i=1; smsec = 0;
	while (i < MAX_ROWS) {
		if (opt != NO_OUTPUT) {
	          GetTime(&checktime); if (smsec > checktime.millitm) smsec=0;
		  if (checktime.millitm >= (smsec+OUT_MSEC)) {
			szLogPrintf(lpSrvr,FALSE, "\tValue: %d.\r\n", i);
			smsec = checktime.millitm;
		  }
		}
		_ltoa(i, typ_char, 10);

		rc = SQLExecute(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute?");

		i++;
	}
	GetTime(&timebuf2);       /* Get finish time struct * /

	sprintf(buffer,"Update Table  (%d. rows)",i-1);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time* /
	*/
	/* --- Select Table --------------------------------------------- */
	/* --- 2. --- */
	/* --- mysql ???
	lstrcpy(select,"SELECT ");
	lstrcat(select," TYP_CHAR ");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect 2. Stmt= '%s'\r\n",select);

	GetTime(&timebuf1);       /* Get start time struct  * /

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=1;

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	if (rc != SQL_NO_DATA_FOUND) {
		rc = SQLGetData(*phstmt,1,SQL_C_CHAR,&typ_char,MAX_NAME_LEN,
				&cbChar);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	}
	else lstrcpy(typ_char,"<NULL>");

	sprintf(buffer, "1.->'%s'",typ_char);
	szLogPrintf(lpSrvr,FALSE,
				"\tSelect Values %d.: %s \r\n", i, buffer);

	GetTime(&timebuf2);       /* Get finish time struct * /

	sprintf(buffer,"Select Table  (%d. rows)",i);
	CheckTime(timebuf1, timebuf2, buffer, lpSrvr, 0);/*Output time* /

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	*/
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
