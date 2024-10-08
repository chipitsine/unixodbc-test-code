/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGETCO.C
**
** Purpose      : Auto Test DLL, Test von SQLGetConnectOption
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "SQLGETCONNECTOPT"
#define TABLE "GETCONNECT"

#define MAX_NAME_LEN 50
#define STRING_LEN 10
#define CHAR_LEN 120

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoGetConnectOption:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetConnectOpt(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   buffer[MAX_NAME_LEN+1];

	UDWORD 			pAccessMode,
				pAutoCommit,
				pLoginTimeout,
				pOdbcCursors,
				pOptTrace,
				pPacketSize,
				pQuietMode,
				pTranslateOption,
				pTxnIsolation;

 	UCHAR			pCurrentQualifier[CHAR_LEN],
				pOptTraceFile[CHAR_LEN],
				pTranslateDLL[CHAR_LEN];

/* ---------------------------------------------------------------------har- */

	/* --- Connect ----------------------------------------------------- */
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

	/* --- GetConnectOption -------------------------------------------- */

	/* *** SQL_ACCESS_MODE --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_ACCESS_MODE, &pAccessMode);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption1");

	/* *** SQL_AUTOCOMMIT --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_AUTOCOMMIT, &pAutoCommit);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption2");

	/* *** SQL_CURRENT_QUALIFIER --------------- *** */
	/* (ODBC 2.0) */
	rc = SQLGetConnectOption( *phdbc, SQL_CURRENT_QUALIFIER, pCurrentQualifier);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,0,"SQLGetConnectOption3");

	/* *** SQL_LOGIN_TIMEOUT --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_LOGIN_TIMEOUT, &pLoginTimeout);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption4");

	/* *** SQL_ODBC_CURSORS --------------- *** */
   	/* (ODBC 2.0) */
	rc = SQLGetConnectOption( *phdbc, SQL_ODBC_CURSORS, &pOdbcCursors);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption5");

	/* *** SQL_OPT_TRACE --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_OPT_TRACE, &pOptTrace);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption6");

	/* *** SQL_TRACEFILE --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_OPT_TRACEFILE, pOptTraceFile);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption7");

	/* *** SQL_PACKET_SIZE --------------- *** */
	/* (ODBC 2.0) */
	rc = SQLGetConnectOption( *phdbc, SQL_PACKET_SIZE, &pPacketSize);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,0,"SQLGetConnectOption8");

	/* *** SQL_QUIET_MODE --------------- *** */
	/* (ODBC 2.0) */
	rc = SQLGetConnectOption( *phdbc, SQL_QUIET_MODE, &pQuietMode);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption9");

	/* *** SQL_TRANSLATE_DLL --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_TRANSLATE_DLL, pTranslateDLL);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption10");

	/* *** SQL_TRANSLATE_OPTION --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_TRANSLATE_OPTION, &pTranslateOption);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption11");

	/* *** SQL_TXN_ISOLATION --------------- *** */
	rc = SQLGetConnectOption( *phdbc, SQL_TXN_ISOLATION, &pTxnIsolation);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetConnectOption12");

	/* --- Output ConnectOptons ---------------------------------------- */

	/* *** SQL_ACCESS_MODE --------------- *** */
	switch (pAccessMode) {
	 case (SQL_MODE_READ_ONLY): lstrcpy(buffer,"SQL_MODE_READ_ONLY");break;
	 case (SQL_MODE_READ_WRITE): lstrcpy(buffer,"SQL_MODE_READ_WRITE");break;
	}
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ACCESS_MODE	: '%s' \r\n", buffer);

	/* *** SQL_AUTOCOMMIT --------------- *** */
	switch(pAutoCommit) {
	 case (SQL_AUTOCOMMIT_ON): lstrcpy(buffer,"SQL_AUTOCOMMIT_ON"); break;
	 case (SQL_AUTOCOMMIT_OFF): lstrcpy(buffer,"SQL_AUTOCOMMIT_OFF"); break;
	}
	szLogPrintf(lpSrvr,FALSE,"\t SQL_AUTOCOMMIT	: '%s' \r\n", buffer);

	/* *** SQL_CURRENT_QUALIFIER --------------- *** */
	/* %s - pCurrentQualifier */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_CURRENT_QUALIFIER: '-' \r\n");

	/* *** SQL_LOGIN_TIMEOUT --------------- *** */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_LOGIN_TIMEOUT	: '%d' \r\n", pLoginTimeout);

	/* *** SQL_ODBC_CURSORS --------------- *** */
	/* %d - pOdbcCursors */
	switch (pOdbcCursors) {
	 case (SQL_CUR_USE_IF_NEEDED): lstrcpy(buffer,"SQL_CUR_USE_IF_NEEDED");break;
	 case (SQL_CUR_USE_ODBC):      lstrcpy(buffer,"SQL_CUR_USE_ODBC");break;
	 case (SQL_CUR_USE_DRIVER):    lstrcpy(buffer,"SQL_CUR_USE_DRIVER");break;
	}
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ODBC_CURSORS 	: '%s' \r\n", buffer);

	/* *** SQL_OPT_TRACE --------------- *** */
	switch (pOptTrace) {
	 case (SQL_OPT_TRACE_ON):  lstrcpy(buffer,"SQL_OPT_TRACE_ON"); break;
	 case (SQL_OPT_TRACE_OFF): lstrcpy(buffer,"SQL_OPT_TRACE_OFF"); break;
	}
	szLogPrintf(lpSrvr,FALSE,"\t SQL_OPT_TRACE	: '%s' \r\n", buffer);

	/* *** SQL_TRACEFILE --------------- *** */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_OPT_TRACEFILE	: '%s' \r\n", pOptTraceFile);

	/* *** SQL_PACKET_SIZE --------------- *** */
	/* %d - pPacketSize */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_PACKET_SIZE	: '-' \r\n");

	/* *** SQL_QUIET_MODE --------------- *** */
	/* %d - pQuietMode */
	sprintf(buffer,"%p",pQuietMode);
	szLogPrintf(lpSrvr,FALSE,"\t SQL_QUIET_MODE 	: '%s' \r\n",buffer);

	/* *** SQL_TRANSLATE_DLL --------------- *** */
	/* %s - pTranslateDLL */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_TRANSLATE_DLL  : '%s' \r\n",pTranslateDLL);

	/* *** SQL_TRANSLATE_OPTION --------------- *** */
	/* %d - pTranslateOption */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_TRANSLATE_OPTION: '%d' \r\n",pTranslateOption);

	/* *** SQL_TXN_ISOLATION --------------- *** */
	/* %d - pTxnIsolation */
	switch (pTxnIsolation) {
		case (SQL_TXN_READ_UNCOMMITTED) : lstrcpy(buffer,"SQL_TXN_READ_UNCOMMITTED"); break;
		case (SQL_TXN_READ_COMMITTED) : lstrcpy(buffer,"SQL_TXN_READ_COMMITTED"); break;
		case (SQL_TXN_REPEATABLE_READ) : lstrcpy(buffer,"SQL_TXN_REPEATABLE_READ"); break;
		case (SQL_TXN_SERIALIZABLE) : lstrcpy(buffer,"SQL_TXN_SERIALIZABLE"); break;
		/* case (SQL_TXN_VERSIONING) : lstrcpy(buffer,"SQL_TXN_VERSIONING"); break; */
	}
	szLogPrintf(lpSrvr,FALSE,"\t SQL_TXN_ISOLATION	: '%s' \r\n",buffer);

	/* --- SQLSetConnectOption ----------------------------------------- */
	/*
	if (vAutoCommit == SQL_AUTOCOMMIT_ON) {
		vAutoCommit =  SQL_AUTOCOMMIT_OFF;  (o. SQL_AUTOCOMMIT_ON)
		rc = SQLSetConnectOption( *phdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetConnectOption");
	}
	*/
	/* --- Disconnect -------------------------------------------------- */
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
