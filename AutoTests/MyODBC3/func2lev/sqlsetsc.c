/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSETSC.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSetScrollOptions
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLSetScrollOptions"
#define PREPARESEL "select * from sysodbctypes"

#define MAX_NAME_LEN 80
#define MAX_RGB_VALUE 30

#define STR_LEN 128+1
#define REM_LEN 254+1

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*-------------------------------------------------------------------------
//| DoSQLSetScrollOptions:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*-------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLSetScrollOptions(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ---------------------------------------------------------------------- */
	UCHAR                   buffer[MAX_NAME_LEN*2];
	UCHAR                   buf[MAX_NAME_LEN];

	UWORD                   fConcurrency;
	SDWORD			crowKeyset;
	UWORD			crowRowset;
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


/* --- SQLSetScrollOptions --------------------------------------------------- */
	/* *** 1. SQLSetScrollOptions -> Set defaults */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 1;
	lstrcpy(buffer,"1. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions");

	/* *** 2. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_STATIC; crowRowset = 5;
	lstrcpy(buffer,"2. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_STATIC \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions");

	/* *** 3. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_KEYSET_DRIVEN; crowRowset = 10;
	lstrcpy(buffer,"3. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_KEYSET_DRIVEN \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions");

	/* *** 4. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_DYNAMIC; crowRowset = 5;
	lstrcpy(buffer,"4. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_DYNAMIC \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions");

	/* *** 5. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_LOCK;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	lstrcpy(buffer,"5. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_LOCK \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions");

	/* *** 6. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_ROWVER;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	lstrcpy(buffer,"6. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_ROWVER \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */

	/* *** 7. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_VALUES;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	lstrcpy(buffer,"7. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_VALUES \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */

	/* *** 8. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 0;
	lstrcpy(buffer,"8. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d\r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */

	/* *** 9. SQLSetScrollOptions */
	fConcurrency = 99;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	lstrcpy(buffer,"9. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d \r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */

	/* *** 10. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = 999; crowRowset = 5;
	lstrcpy(buffer,"10. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = \r\n");
	sprintf(buf,   "\t crowRowset   = %d \r\n",crowRowset);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */

	/* *** 11. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	lstrcpy(buffer,"11. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d  \r\n",crowRowset);
	lstrcat(buf,   "\t state        = API_STMT_PREPARED\r\n");
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLPrepare(*phstmt, PREPARESEL, sizeof(PREPARESEL));
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLPrepare -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare"); */

	if (rc == SQL_SUCCESS) {
	   rc = SQLSetScrollOptions(*phstmt, fConcurrency, crowKeyset, crowRowset);
	   szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	  /* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */
	}

	/* *** 12. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	lstrcpy(buffer,"12. SQLSetScrollOptions -> \r\n");
	lstrcat(buffer,"\t fConcurrency = SQL_CONCUR_READ_ONLY \r\n");
	lstrcat(buffer,"\t crowKeyset   = SQL_SCROLL_FORWARD_ONLY \r\n");
	sprintf(buf,   "\t crowRowset   = %d \r\n",crowRowset);
	lstrcat(buf,   "\t HSTMT !!\r\n");
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);szLogPrintf(lpSrvr, FALSE,"%s",buf);

	rc = SQLSetScrollOptions(NULL, fConcurrency, crowKeyset, crowRowset);
	szLogPrintf(lpSrvr, FALSE,"\t\t SQLSetScrollOptions -> retcode: %d\r\n", rc);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetScrollOptions"); */

/* ------------------------------------------------------------------------- */
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
