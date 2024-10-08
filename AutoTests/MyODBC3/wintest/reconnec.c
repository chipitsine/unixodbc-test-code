/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : RECONNEC.C
**
** Purpose      : Auto Test DLL, Test von RECONNECT
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "RECONNECT"

#define TABLE1 "RECONNECT1"
#define TABLE2 "RECONNECT2"

#define MAX_CONNECTIONS 16		/* Anzahl Array	Connections	*/
#define MAX_STATEMENTS  16		/* Anzahl Array Statements	*/

#define CONNECT_TEST   8		/* Max. Connect			*/
//#define CONNECT_TEST   5		/* Max. Connect			*/
#define WAIT_TIME      65		/* Warten Timeout in sec.	*/

#define MAX_ROWS 15			/* Max. Zeilen			*/
#define READ_ROWS1 10
#define READ_ROWS2 5

#define MAX_NAME_LEN 256
#define STRING_LEN 10

/* ------------------------------------------------------------------------- */

#define CLOSE_CURSOR3			/* Offener- o. geschlossener Cursor  */
#define TIMEOUT				/* Timeout bercksichtigen	     */

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoReConnect:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoReConnect(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                           sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR			buffer[MAX_NAME_LEN+1];

	HDBC 			hdbc[MAX_CONNECTIONS];
	HSTMT			hstmt[MAX_STATEMENTS];

	UDWORD 			pLoginTimeout[MAX_CONNECTIONS];

        UCHAR           	szCharData[MAX_NAME_LEN];
        SDWORD          	cbChar=SQL_NTS;

	SWORD			i,j,count=0;

	UDWORD			pAutoCommit;

	struct _timeb           timebuf1, timebuf2;
	long			diff;
/* ---------------------------------------------------------------------har- */

/* -----Init arrays -------------------------------------------------------- */
	hdbc[0] = NULL;	hdbc[1] = NULL;	hdbc[2] = NULL;	hdbc[3] = NULL;
	hdbc[4] = NULL;	hdbc[5] = NULL;	hdbc[6] = NULL;	hdbc[7] = NULL;

	hstmt[0] = NULL; hstmt[1] = NULL; hstmt[2] = NULL; hstmt[3] = NULL;
	hstmt[4] = NULL; hstmt[5] = NULL; hstmt[6] = NULL; hstmt[7] = NULL;

/* -----SQLAllocEnv -------------------------------------------------------- */
	/* ***** SQLAllocEnv */
	rc = SQLAllocEnv(phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocEnv");

/* -----SQLAllocConnect ---------------------------------------------------- */
	/* ***** SQLAllocConnect 1. */
	i = 0;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 1.");
}
	/* ***** SQLAllocConnect 2. */
	i = 1;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 2.");
}
	/* ***** SQLAllocConnect 3. */
	i = 2;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 3.");
}
	/* ***** SQLAllocConnect 4. */
	i = 3;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 4.");
}
	/* ***** SQLAllocConnect 5. */
	i = 4;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 5.");
}
	/* ***** SQLAllocConnect 6. */
	i = 5;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 6.");
}
	/* ***** SQLAllocConnect 7. */
	i = 6;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 7.");
}
	/* ***** SQLAllocConnect 8. */
	i = 7;
if (CONNECT_TEST >= i+1) {
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect 8.");
}

/* -----SQLConnect --------------------------------------------------------- */
	/* ***** SQLConnect 1. */
	i = 0;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 1.");
}
	/* ***** SQLConnect 2. */
	i = 1;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 2.");
}
	/* ***** SQLConnect 3. */
	i = 2;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 3.");
}
	/* ***** SQLConnect 4. */
	i = 3;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 4.");
}
	/* ***** SQLConnect 5. */
	i = 4;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 5.");
}
	/* ***** SQLConnect 6. */
	i = 5;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 6.");
}
	/* ***** SQLConnect 7. */
	i = 6;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 7.");
}
	/* ***** SQLConnect 8. */
	i = 7;
if (CONNECT_TEST >= i+1) {
	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLConnect 8.");
}

/* -----SQLAllocStmt ------------------------------------------------------- */
	/* ***** SQLAllocStmt 1. */
	i = 0;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 1.");
}
	/* ***** SQLAllocStmt 2. */
	i = 1;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 2.");
}
	/* ***** SQLAllocStmt 3. */
	i = 2;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 3.");
}
	/* ***** SQLAllocStmt 4. */
	i = 3;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 4.");
}
	/* ***** SQLAllocStmt 5. */
	i = 4;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 5.");
}
	/* ***** SQLAllocStmt 6. */
	i = 5;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 6.");
}
	/* ***** SQLAllocStmt 7. */
	i = 6;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 7.");
}
	/* ***** SQLAllocStmt 8. */
	i = 7;
if (CONNECT_TEST >= i+1) {
	rc = SQLAllocStmt(hdbc[i], &hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLAllocStmt 8.");
}

/* -----SQLGetConnectOption ------------------------------------------------ */
	/* ***** SQLGetConnectOption 1. */
	i = 0;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLGetConnectOption( hdbc[i], SQL_LOGIN_TIMEOUT, &pLoginTimeout[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption 1.");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_LOGIN_TIMEOUT	(Connect 1.) : '%d' \r\n", pLoginTimeout[i]);
}
	/* ***** SQLGetConnectOption 2. */
	i = 1;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLGetConnectOption( hdbc[i], SQL_LOGIN_TIMEOUT, &pLoginTimeout[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption 2.");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_LOGIN_TIMEOUT	(Connect 2.) : '%d' \r\n", pLoginTimeout[i]);
}
	/* ***** SQLGetConnectOption 3. */
	i = 2;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLGetConnectOption( hdbc[i], SQL_LOGIN_TIMEOUT, &pLoginTimeout[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption 3.");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_LOGIN_TIMEOUT	(Connect 3.) : '%d' \r\n", pLoginTimeout[i]);
}
	/* ***** SQLGetConnectOption 4. */
	i = 3;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLGetConnectOption( hdbc[i], SQL_LOGIN_TIMEOUT, &pLoginTimeout[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption 4.");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_LOGIN_TIMEOUT	(Connect 4.) : '%d' \r\n", pLoginTimeout[i]);
}

	/* --- SQLSetConnectOption ----------------------------------------- */
	i=0;
	/* *** SQL_AUTOCOMMIT ------------------ *** */
	rc = SQLGetConnectOption(hdbc[i], SQL_AUTOCOMMIT, &pAutoCommit);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption");

	if (pAutoCommit==SQL_AUTOCOMMIT_ON) lstrcpy(buffer,"SQL_AUTOCOMMIT_ON");
	    else if (pAutoCommit==SQL_AUTOCOMMIT_OFF) lstrcpy(buffer,"SQL_AUTOCOMMIT_OFF");
	szLogPrintf(lpSrvr,FALSE,"\t SQL_AUTOCOMMIT	: '%s' \r\n", buffer);

	if (pAutoCommit == SQL_AUTOCOMMIT_OFF) {
		szLogPrintf(lpSrvr,FALSE,"\t Set SQL_AUTOCOMMIT to ON \r\n");
		pAutoCommit =  SQL_AUTOCOMMIT_ON;

		rc = SQLSetConnectOption( hdbc[i], SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_ON);
		ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLSetConnectOption");

		rc = SQLGetConnectOption( hdbc[i], SQL_AUTOCOMMIT, &pAutoCommit);
		ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption");

		if (pAutoCommit==SQL_AUTOCOMMIT_ON) lstrcpy(buffer,"SQL_AUTOCOMMIT_ON");
		    else if (pAutoCommit==SQL_AUTOCOMMIT_OFF) lstrcpy(buffer,"SQL_AUTOCOMMIT_OFF");
		szLogPrintf(lpSrvr,FALSE,"\t SQL_AUTOCOMMIT	: '%s' \r\n", buffer);
	}
/* ----- Create- and Insert into Table ------------------------------------ */
	i=0;
if (CONNECT_TEST >= i+1) {
	/* --- Create Table 1. ------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE1);
	lstrcat(create," ( NAME CHAR(50), AGE INTEGER, ADRESSE CHAR(80))\0");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 1. = '%s'\r\n",create);

	rc = SQLExecDirect(hstmt[i], create, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Create Table 2. ------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE2);
	lstrcat(create," ( KUNDE CHAR(50), ADRESSE CHAR(80), PLZ INTEGER) ");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 2. = '%s'\r\n",create);

	rc = SQLExecDirect(hstmt[i], create, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */
	if (pAutoCommit == SQL_AUTOCOMMIT_OFF) {
	  rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	  ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");
	}
	/* --- Insert Table 1. ------------------------------------------- */
	 lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE1);
	 lstrcat(insert," VALUES ('MEIER', 35, 'Hermannstr. 10, Berlin')");

	for (j=0; j < MAX_ROWS; j++) {
	 szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 1. = <%d.> '%s'\r\n",j+1,insert);

	 rc = SQLExecDirect(hstmt[i], insert, SQL_NTS);
	 ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
	}
	/* --- Insert Table 2. ------------------------------------------- */
	 lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE2);
	 lstrcat(insert," VALUES ('XYZ', 'Muellerstr. 65, Berlin', 10772)");

	for (j=0; j < MAX_ROWS; j++) {
	 szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 2. = <%d.> '%s'\r\n",j+1,insert);

	 rc = SQLExecDirect(hstmt[i], insert, SQL_NTS);
	 ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
	}
}

	/* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */
	if (pAutoCommit == SQL_AUTOCOMMIT_OFF) {
	  rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	  ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");
	}
/* -----SQLExecDirect ------------------------------------------------------ */
	/* ***** SQLExecDirect 1. */
	i = 0;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT * FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 1. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 1.");
	count = 0;
	while (count < READ_ROWS1) {
		rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,
				&cbChar);
		ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 1.");

		rc = SQLFetch(hstmt[i]);
		ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 1.");

		if (rc == SQL_NO_DATA_FOUND) break;

		count ++;
	}
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 2. */
	i = 1;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT * FROM ");lstrcat(select,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 2. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 2.");

	count = 0;
	while (count < READ_ROWS2) {
		rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,
				&cbChar);
		ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 2.");

		rc = SQLFetch(hstmt[i]);
		ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 2.");

		if (rc == SQL_NO_DATA_FOUND) break;
		count ++;
	}
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 3. */
	i = 2;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT KUNDE FROM ");lstrcat(select,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 3. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 3.");

	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 3.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 3.");
#ifdef CLOSE_CURSOR3
	rc = SQLFreeStmt(hstmt[i], SQL_CLOSE);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 3.");
#endif
}

/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 4. */
	i = 3;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT NAME FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 4. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 4.");

	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,
			&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 4.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 4.");
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 5. */
	i = 4;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT AGE FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 5. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 5.");

	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,
			&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 5.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 5.");
}
/* ------------------------------------------------------------------------- */
/* Wait TIMEOUT (ODBC-User = 60 sec ) */
#ifdef TIMEOUT
   szLogPrintf(lpSrvr,FALSE,"\t Wait TIMEOUT for %s-User (60 sec.) ... \r\n",lpSrvr->szValidLogin0);
   GetTime(&timebuf1);       /* Get start time struct  */
   while(1) {
        GetTime(&timebuf2);       /* Get finish time struct */
	diff = timebuf2.time - timebuf1.time;
        if (diff >= WAIT_TIME) break;
   }
#endif
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 1. */
	/* Zurcksetzen mit offenen CURSOR */
	i = 0; /* Fetch Next */
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 1.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 1.");
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 3. */
	/* Zurcksetzen mit offenen o. geschlossenen CURSOR */
	i = 2; /* Fetch Next */
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
#ifdef CLOSE_CURSOR3
	lstrcpy(select,"SELECT KUNDE FROM ");lstrcat(select,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 3. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 3.");
#endif
	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 3.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 3.");

#ifdef CLOSE_CURSOR3
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 3.");
#endif
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 6. */
	i = 5;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT ADRESSE FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 6. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 6.");

	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 6.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 6.");
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 7. */
	i = 6;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT NAME FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 7. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 7.");

	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,
			&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 7.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 7.");
}
/* ------------------------------------------------------------------------- */
	/* ***** SQLExecDirect 8. */
	i = 7;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	lstrcpy(select,"SELECT ADRESSE FROM ");lstrcat(select,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 8. = '%s'\r\n",select);

	rc = SQLExecDirect(hstmt[i], select, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect 8.");

	rc = SQLBindCol(hstmt[i], 1, SQL_C_CHAR, szCharData, MAX_NAME_LEN,
			&cbChar);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLBindCol 8.");

	rc = SQLFetch(hstmt[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFetch 8.");
}

/* ------------------------------------------------------------------------- */
	/* --- Drop Table 1. -------------------------------------------- */
	i=0;
if (CONNECT_TEST >= i+1) {
#ifdef DROP_TABLE
	lstrcpy(drop,"DROP TABLE ");lstrcat(drop,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt 1.= '%s'\r\n",drop);

	rc = SQLFreeStmt(hstmt[i], SQL_CLOSE);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLExecDirect(hstmt[i], drop, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif

	/* --- Drop Table 2. -------------------------------------------- */
#ifdef DROP_TABLE

	lstrcpy(drop,"DROP TABLE ");lstrcat(drop,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt 2.= '%s'\r\n",drop);

	rc = SQLFreeStmt(hstmt[i], SQL_CLOSE);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLExecDirect(hstmt[i], drop, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif
}
/* ------------------------------------------------------------------------- */

/* -----SQLFreeStmt -------------------------------------------------------- */
	/* ***** SQLFreeStmt 1. */
	i = 0;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 1.");
}
	/* ***** SQLFreeStmt 2. */
	i = 1;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 2.");
}
	/* ***** SQLFreeStmt 3. */
#ifndef CLOSE_CURSOR3
	i = 2;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 3.");
}
#endif
	/* ***** SQLFreeStmt 4. */
	i = 3;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 4.");
}
	/* ***** SQLFreeStmt 5. */
	i = 4;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 5.");
}
	/* ***** SQLFreeStmt 6. */
	i = 5;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 6.");
}
	/* ***** SQLFreeStmt 7. */
	i = 6;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 7.");
}
	/* ***** SQLFreeStmt 8. */
	i = 7;
if (CONNECT_TEST >= i+1) {
	rc = SQLFreeStmt(hstmt[i], SQL_DROP);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt 8.");
}

/* -----SQLDisconnect ------------------------------------------------------ */
	/* ***** SQLDisconnect 1. */
	i = 0;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 1.");
}
	/* ***** SQLDisconnect 2. */
	i = 1;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 2.");
}
	/* ***** SQLDisconnect 3. */
	i = 2;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 3.");
}
	/* ***** SQLDisconnect 4. */
	i = 3;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 4.");
}
	/* ***** SQLDisconnect 5. */
	i = 4;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 5.");
}
	/* ***** SQLDisconnect 6. */
	i = 5;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 6.");
}
	/* ***** SQLDisconnect 7. */
	i = 6;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 7.");
}
	/* ***** SQLDisconnect 8. */
	i = 7;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect 8.");
}

/* -----SQLFreeConnect ----------------------------------------------------- */
	/* ***** SQLFreeConnect 1.*/
	i = 0;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 1.");
}
	/* ***** SQLFreeConnect 2.*/
	i = 1;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 2.");
}
	/* ***** SQLFreeConnect 3.*/
	i = 2;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 3.");
}
	/* ***** SQLFreeConnect 4.*/
	i = 3;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 4.");
}
	/* ***** SQLFreeConnect 5.*/
	i = 4;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 5.");
}
	/* ***** SQLFreeConnect 6.*/
	i = 5;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 6.");
}
	/* ***** SQLFreeConnect 7.*/
	i = 6;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 7.");
}
	/* ***** SQLFreeConnect 8.*/
	i = 7;
if ((CONNECT_TEST >= i+1) && (hdbc[i] != NULL)) {
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],hstmt[i],lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect 8.");
}
/* -----SQLFreeEnv --------------------------------------------------------- */
	/* ***** SQLFreeEnv */
	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* -------------------------------------------------------------------------- */
