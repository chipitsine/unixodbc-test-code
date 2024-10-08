/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSETST.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSetStmtOption
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLSetStmtOptions"
#define TABLE ""

#define MAX_NAME_LEN 256
#define MAX_RGB_VALUE 256
#define ERROR_TEXT_LEN 511

#define STR_LEN 128+1
#define REM_LEN 254+1

//*-------------------------------------------------------------------------

/* ------------------------------------------------------------------------ */
/* SQLGetStmtOption, SQLSetStmtOption Parameters : */
/* ------------------------------------------------------------------------ */
/*
		1. SQL_ASYNC_ENABLE
		2. SQL_BIND_TYPE
		3. SQL_CONCURRENCY (ODBC 2.0)
		4. SQL_CURSOR_TYPE (ODBC 2.0)
		5. SQL_KEYSET_SIZE (ODBC 2.0)
		6. SQL_MAX_LENGTH
		7. SQL_MAX_ROWS
		8. SQL_NOSCAN
		9. SQL_QUERY_TIMEOUT
	       10. SQL_RETRIEVE_DATA (ODBC 2.0)
	       11. SQL_ROWSET_SIZE (ODBC 2.0)
	       12. SQL_SIMULATE_CURSOR (ODBC 2.0)
	       13. SQL_USE_BOOKMARKS (ODBC 2.0)

   Only SQLGetStmtOption:
		1. SQL_GET_BOOKMARK (ODBC 2.0)
		2. SQL_ROW_NUMBER (OBDC 2.0)
*/

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLSetStmtOption:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLSetStmtOption(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   buffer[MAX_NAME_LEN*20];

	UWORD                   fOption;
	PTR                     pvParam;
	UDWORD			pPar;
	UDWORD                  vParam;

	UCHAR sqlstate[10];
	DWORD esq_sql_code;
	UCHAR error_txt[ERROR_TEXT_LEN+1];
	SWORD len_error_txt = ERROR_TEXT_LEN;
	SWORD used_error_txt;
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

/* --- SQLGetStmtOption ---------------------------------------------------- */
	/* *** 1. SQL_ASYNC_ENABLE */
	fOption = SQL_ASYNC_ENABLE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t1. SQL_ASYNC_ENABLE : \r\n");
	rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 1."); */

	if (rc != SQL_SUCCESS) {
		SQLError(*phenv, *phdbc, *phstmt, sqlstate, &esq_sql_code,
		error_txt, len_error_txt, &used_error_txt);

		sprintf(buffer,"sqlstate: %s",sqlstate);
		szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
		sprintf(buffer,"esq_sql_code: %ld",esq_sql_code);
		szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
		sprintf(buffer,"error_txt: %s", error_txt);
		szLogPrintf(lpSrvr, FALSE,"\tSQLError -> %s\r\n", buffer);
	}
	   else {
		pPar = (UDWORD) pvParam;
		switch (pPar) {
		  case SQL_ASYNC_ENABLE_OFF : lstrcpy(buffer,"SQL_ASYNC_ENABLE_OFF"); break;
		  case SQL_ASYNC_ENABLE_ON  : lstrcpy(buffer,"SQL_ASYNC_ENABLE_ON"); break;
		  default                   : lstrcpy(buffer,"!!!");
		}
		szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);
	}

	/* *** 2. SQL_BIND_TYPE */
	fOption = SQL_BIND_TYPE;
	szLogPrintf(lpSrvr, FALSE,"\t2. SQL_BIND_TYPE : \r\n");
	rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 2.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 3. SQL_CONCURRENCY (ODBC 2.0) */
	fOption = SQL_CONCURRENCY; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t3. SQL_CONCURRENCY : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 3.");
	pPar = (UDWORD) pvParam;
	switch (pPar) {
	 case SQL_CONCUR_READ_ONLY : lstrcpy(buffer,"SQL_CONSUR_READ_ONLY"); break;
	 case SQL_CONCUR_LOCK      : lstrcpy(buffer,"SQL_CONSUR_LOCK"); break;
	 case SQL_CONCUR_ROWVER    : lstrcpy(buffer,"SQL_CONSUR_ROWCUR"); break;
	 case SQL_CONCUR_VALUES    : lstrcpy(buffer,"SQL_CONSUR_VALUES"); break;
	 default                   : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 4. SQL_CURSOR_TYPE (ODBC 2.0) */
	fOption = SQL_CURSOR_TYPE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t4. SQL_CURSOR_TYPE : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 4.");
	pPar = (UDWORD) pvParam;
	switch (pPar) {
	 case SQL_CURSOR_FORWARD_ONLY   : lstrcpy(buffer,"SQL_CURSOR_FORWARD_ONLY"); break;
	 case SQL_CURSOR_STATIC  	: lstrcpy(buffer,"SQL_CURSOR_STATIC"); break;
	 case SQL_CURSOR_KEYSET_DRIVEN  : lstrcpy(buffer,"SQL_CURSOR_KEYSET_DRIVEN"); break;
	 case SQL_CURSOR_DYNAMIC 	: lstrcpy(buffer,"SQL_CURSOR_DYNAMIC"); break;
	 default                   	: lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 5. SQL_KEYSET_SIZE (ODBC 2.0) */
	fOption = SQL_KEYSET_SIZE;
	szLogPrintf(lpSrvr, FALSE,"\t5. SQL_KEYSET_SIZE : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 5.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 6. SQL_MAX_LENGTH */
	fOption = SQL_MAX_LENGTH;
	szLogPrintf(lpSrvr, FALSE,"\t6. SQL_MAX_LENGTH : \r\n");
	rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 6.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 7. SQL_MAX_ROWS */
	fOption = SQL_MAX_ROWS;
	szLogPrintf(lpSrvr, FALSE,"\t7. SQL_MAX_ROWS : \r\n");
	rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 7.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 8. SQL_NOSCAN */
	fOption = SQL_NOSCAN; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t8. SQL_NOSCAN : \r\n");
	rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 8.");
	pPar = (UDWORD) pvParam;
	switch (pPar) {
	 case SQL_NOSCAN_OFF : lstrcpy(buffer,"SQL_NOSCAN_OFF"); break;
	 case SQL_NOSCAN_ON  : lstrcpy(buffer,"SQL_NOSCAN_ON"); break;
	 default             : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 9. SQL_QUERY_TIMEOUT */
	fOption = SQL_QUERY_TIMEOUT;
	szLogPrintf(lpSrvr, FALSE,"\t9. SQL_QUERY_TIMEOUT : \r\n");
	rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 9.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 10. SQL_RETRIEVE_DATA (ODBC 2.0) */
	fOption = SQL_RETRIEVE_DATA; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t10. SQL_RETRIEVE_DATA : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 10.");
	pPar = (UDWORD) pvParam;
	switch (pPar) {
	 case SQL_RD_OFF : lstrcpy(buffer,"SQL_RD_OFF"); break;
	 case SQL_RD_ON  : lstrcpy(buffer,"SQL_RD_ON"); break;
	 default         : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 11. SQL_ROWSET_SIZE (ODBC 2.0) */
	fOption = SQL_ROWSET_SIZE;
	szLogPrintf(lpSrvr, FALSE,"\t11. SQL_ROWSET_SIZE : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 11.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 12. SQL_SIMULATE_CURSOR (ODBC 2.0) */
	fOption = SQL_SIMULATE_CURSOR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t12. SQL_SIMULATE_CURSOR : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 12.");
	pPar = (UDWORD) pvParam;
	switch (pPar) {
	 case SQL_SC_NON_UNIQUE : lstrcpy(buffer,"SQL_SC_NON_UNIQUE"); break;
	 case SQL_SC_TRY_UNIQUE : lstrcpy(buffer,"SQL_SC_TRY_UNIQUE"); break;
	 case SQL_SC_UNIQUE     : lstrcpy(buffer,"SQL_SC_UNIQUE"); break;
	 default                : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 13. SQL_USE_BOOKMARKS (ODBC 2.0) */
	fOption = SQL_USE_BOOKMARKS; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t13. SQL_USE_BOOKMARKS : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetStmtOption 13.");
	pPar = (UDWORD) pvParam;
	switch (pPar) {
	 case SQL_UB_OFF : lstrcpy(buffer,"SQL_UB_OFF"); break;
	 case SQL_UB_ON  : lstrcpy(buffer,"SQL_UB_ON"); break;
	 default         : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* ***  14. SQL_GET_BOOKMARK (ODBC 2.0) */
	fOption = SQL_GET_BOOKMARK;
	szLogPrintf(lpSrvr, FALSE,"\t14. SQL_GET_BOOKMARK : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,0,"SQLGetStmtOption 14.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 15. SQL_ROW_NUMBER (OBDC 2.0) */
	fOption = SQL_ROW_NUMBER;
	szLogPrintf(lpSrvr, FALSE,"\t15. SQL_ROW_NUMBER : \r\n");
        rc = SQLGetStmtOption(*phstmt, fOption, (PTR) &pvParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,0,"SQLGetStmtOption 15.");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

/* --- SQLSetStmtOption -------------------------------------------------- */
	/* *** 1. SQL_ASYNC_ENABLE */
	/* SQL_ASYNC_ENABLE_OFF, SQL_ASYNC_ENABLE_ON */
	fOption = SQL_ASYNC_ENABLE; vParam = SQL_ASYNC_ENABLE_ON;
	sprintf(buffer,"%d", vParam);
	szLogPrintf(lpSrvr, FALSE,"\t1. SQL_ASYNC_ENABLE : -> SQL_ASYNC_ENABLE_ON \r\n");
	/*
	rc = SQLSetStmtOption(*phstmt, fOption, vParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption 1.");
	*/

	/* *** 2. SQL_BIND_TYPE */
	/* SQL_BIND_BY_COLUMN */
	fOption = SQL_BIND_TYPE; vParam = SQL_BIND_BY_COLUMN;
	sprintf(buffer,"%d", vParam);
	szLogPrintf(lpSrvr, FALSE,"\t2. SQL_BIND_TYPE : -> SQL_BIND_BY_COLUMN \r\n");
	rc = SQLSetStmtOption(*phstmt, fOption, vParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption 2.");

	/* *** 8. SQL_NOSCAN */
	/* SQL_NOSCAN_OFF, SQL_NOSCAN_ON */
	fOption = SQL_NOSCAN; vParam = SQL_NOSCAN_ON;
	sprintf(buffer,"%d", vParam);
	szLogPrintf(lpSrvr, FALSE,"\t8. SQL_NOSCAN : -> SQL_NOSCAN_ON\r\n");
	rc = SQLSetStmtOption(*phstmt, fOption, vParam);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetStmtOption 8.");

/* ------------------------------------------------------------------------- */
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
/* ---------------------------------------------------------------------har- */
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
