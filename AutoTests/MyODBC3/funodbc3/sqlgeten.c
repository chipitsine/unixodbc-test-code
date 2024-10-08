/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGETEN.C
**
** Purpose      : Auto Test DLL, Test von SQLGetEnvAttr
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "SQLGETENVATTR"
#define TABLE "GETENV"

#define MAX_NAME_LEN 50
#define STRING_LEN 10
#define CHAR_LEN 120

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoGetConnectAttr:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetEnvAttr(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   buf[MAX_NAME_LEN+1];

	UDWORD 			pConnectPool;
	UDWORD 			pCpMatch;
	DWORD			pOdbcVer;
	DWORD			pOdbcVerSet;
	DWORD			pOutputNts;

	SQLINTEGER		BufferLength=0;
	SQLINTEGER		StringLengthPtr=0;
	SQLINTEGER		StringLength=0;
/* ---------------------------------------------------------------------har- */

	/* --- Connect ----------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC2, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (SQL_ATTR_ODBC_VERSION)");

	/* --- GetEnvAttr -------------------------------------------------- */
	/* *** SQL_ATTR_CONNECTION_POOLING --------------- *** */
	rc = SQLGetEnvAttr( *phenv, SQL_ATTR_CONNECTION_POOLING, &pConnectPool, BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLGetEnvAttr (SQL_ATTR_CONNECTION_POOLING)");

	/* *** SQL_ATTR_CP_MATCH ------------------------- *** */
	rc = SQLGetEnvAttr( *phenv, SQL_ATTR_CP_MATCH, &pCpMatch, BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLGetEnvAttr (SQL_ATTR_CP_MATCH)");

	/* *** SQL_ATTR_ODBC_VERSION --------------------- *** */
	rc = SQLGetEnvAttr( *phenv, SQL_ATTR_ODBC_VERSION, &pOdbcVer, BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLGetEnvAttr (SQL_ATTR_ODBC_VERSION)");

	/* *** SQL_ATTR_OUTPUT_NTS ----------------------- *** */
	rc = SQLGetEnvAttr( *phenv, SQL_ATTR_OUTPUT_NTS, &pOutputNts, BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLGetEnvAttr (SQL_ATTR_OUTPUT_NTS)");

	/* --- Ouput EntAttributes -------------------------------------- */
	/* *** SQL_ATTR_CONNECTION_POOLING --------------- *** */
	switch (pConnectPool) {
	 case (SQL_CP_OFF): 		         lstrcpy(buf,"SQL_CP_OFF"); break;
	 case (SQL_CP_ONE_PER_DRIVER): 	lstrcpy(buf,"SQL_ONE_PER_DRIVER"); break;
	 case (SQL_CP_ONE_PER_HENV): 	   lstrcpy(buf,"SQL_ONE_PER_HENV"); break;
	 default : lstrcpy(buf,"?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_CONNECTION_POOLING	: '%s' \r\n", buf);

	/* *** SQL_ATTR_CP_MATCH ------------------------ *** */
	switch (pCpMatch) {
	 case (SQL_CP_STRICT_MATCH):  lstrcpy(buf,"SQL_CP_STRICT_MATCH"); break;
	 case (SQL_CP_RELAXED_MATCH): lstrcpy(buf,"SQL_CP_RELAXED_MATCH"); break;
	 default : lstrcpy(buf,"?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_CP_MATCH			: '%s' \r\n", buf);

	/* *** SQL_ATTR_ODBC_VERSION -------------------- *** */
	switch (pOdbcVer) {
	 case (SQL_OV_ODBC3): lstrcpy(buf,"SQL_OV_ODBC3"); break;
	 case (SQL_OV_ODBC2): lstrcpy(buf,"SQL_OV_ODBC2"); break;
	 default :            lstrcpy(buf,"?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_VERSION		: '%s' \r\n", buf);

	/* *** SQL_ATTR_OUTPUT_NTS ---------------------- *** */
	switch (pOutputNts) {
	 case (SQL_TRUE):  lstrcpy(buf,"SQL_TRUE"); break;
	 case (SQL_FALSE): lstrcpy(buf,"SQL_FALSE"); break;
	 default :         lstrcpy(buf,"?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_OUTPUT_NTS		: '%s' \r\n", buf);

	/* --- SetEnvAttr ----------------------------------------------- */
	/* *** SQL_ATTR_CONNECTION_POOLING --------------- *** */
	pConnectPool=SQL_CP_OFF;
   	rc = SQLSetEnvAttr( *phenv, SQL_ATTR_CONNECTION_POOLING, (void*)pConnectPool, StringLength);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLSetEnvAttr (SQL_ATTR_CONNECTION_POOLING)");

	/* *** SQL_ATTR_CP_MATCH ------------------------- *** */
	pCpMatch=SQL_CP_STRICT_MATCH;
	rc = SQLSetEnvAttr( *phenv, SQL_ATTR_CP_MATCH, (void*)pCpMatch, StringLength);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLSetEnvAttr (SQL_ATTR_CP_MATCH)");

	/* *** SQL_ATTR_ODBC_VERSION --------------------- *** */
	pOdbcVerSet=SQL_OV_ODBC2;
	rc=SQLSetEnvAttr( *phenv, SQL_ATTR_ODBC_VERSION, (void*)pOdbcVerSet, StringLength);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLSetEnvAttr (SQL_ATTR_ODBC_VERSION)");

	/* *** SQL_ATTR_OUTPUT_NTS ----------------------- *** */
	pOutputNts=SQL_TRUE;
	rc = SQLSetEnvAttr( *phenv, SQL_ATTR_OUTPUT_NTS, (void*)pOutputNts,StringLength);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr, SQL_SUCCESS,rc,"SQLSetEnvAttr (SQL_ATTR_OUTPUT_NTS)");


  /* ----------------------------------------------------------------- */
	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			                  lpSrvr->szValidLogin0, SQL_NTS,
			                  lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

  /* --- Disconnect -------------------------------------------------- */
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
/* ************************************************************************* */
