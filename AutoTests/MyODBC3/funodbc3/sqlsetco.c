/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSETCO.C
**
** Purpose      : Auto Test DLL, Test von SQLSetConnectAttr
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "SQLSETCONNECTATTR"
#define TABLE "SETCONNECT"

#define MAX_NAME_LEN 50
#define STRING_LEN 10
#define CHAR_LEN 120

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSetConnectAttr:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLSetConnectAttr(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	SQLRETURN               rc;
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
				pTxnIsolation,
				pAsyncEnable,
				pAutoIpd,
				pConTimeout,
				pMetadataId,
				vAutoCommit,
				vConTimeout;

 	UCHAR		pCurrentQualifier[CHAR_LEN],
				pOptTraceFile[CHAR_LEN],
				pTranslateDLL[CHAR_LEN];

	SQLINTEGER		BufferLength;
	SQLINTEGER		StringLengthPtr=0;

/* ---------------------------------------------------------------------har- */

	/* --- Connect ----------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC2, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (HENV)");

	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			                  lpSrvr->szValidLogin0, SQL_NTS,
			                  lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	/* --- GetConnectAttr ---------------------------------------------- */

	/* *** SQL_ATTR_ACCESS_MODE --------------- *** */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_ACCESS_MODE, &pAccessMode,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_ACCESS_MODE)");

	/* *** SQL_ATTR_ASYNC_ENABLE --------- *** */
	/* *** ODBC 3.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_ASYNC_ENABLE, &pAsyncEnable,
				BufferLength, &StringLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_ASYNC_ENABLE)");

	/* *** SQL_ATTR_AUTO_IPD ------------ *** */
	/* *** ODBC 3.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_AUTO_IPD, &pAutoIpd,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_AUTO_IPD)");

	/* *** SQL_ATTR_AUTOCOMMIT --------------- *** */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_AUTOCOMMIT, &pAutoCommit,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_AUTOCOMMIT)");

	/* *** SQL_ATTR_CONNECTION_TIMEOUT ------------ *** */
	/* *** ODBC 3.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc,SQL_ATTR_CONNECTION_TIMEOUT,&pConTimeout,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_CONNECTION_TIMEOUT)");

	/* *** SQL_ATTR_CURRENT_CATALOG ---------- *** */
	/* *** ODBC 2.0 */
	BufferLength = MAX_NAME_LEN;
	rc=SQLGetConnectAttr( *phdbc, SQL_ATTR_CURRENT_CATALOG, pCurrentQualifier,
				BufferLength, &StringLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_CURRENT_CATALOG)");

	/* *** SQL_ATTR_LOGIN_TIMEOUT --------------- *** */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_LOGIN_TIMEOUT, &pLoginTimeout,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_LOGIN_TIMEOUT)");

	/* *** SQL_ATTR_METADATA_ID ------------ *** */
	/* *** ODBC 3.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_METADATA_ID, &pMetadataId,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_METADATA_ID)");

	/* *** SQL_ATTR_ODBC_CURSORS --------------- *** */
   	/* *** ODBC 2.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_ODBC_CURSORS, &pOdbcCursors,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_ODBC_CURSORS)");

	/* *** SQL_ATTR_TRACE --------------- *** */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_TRACE, &pOptTrace,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_TRACE)");

	/* *** SQL_ATTR_TRACEFILE --------------- *** */
	BufferLength = SQL_NTS;lstrcpy(pOptTraceFile,"");
	if (pOptTrace == SQL_OPT_TRACE_ON) {
		rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_TRACEFILE, pOptTraceFile,
					BufferLength, &StringLengthPtr);
		DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_TRACEFILE)");
	}
	/* *** SQL_ATTR_PACKET_SIZE --------------- *** */
	/* *** ODBC 2.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_PACKET_SIZE, &pPacketSize,
				BufferLength, &StringLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_PACKET_SIZE)");

	/* *** SQL_ATTR_QUIET_MODE --------------- *** */
	/* *** ODBC 2.0 */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_QUIET_MODE, &pQuietMode,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_PACKET_SIZE)");

	/* *** SQL_ATTR_TRANSLATE_LIB --------------- *** */
	BufferLength = MAX_NAME_LEN;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_TRANSLATE_LIB, pTranslateDLL,
				BufferLength, &StringLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_TRANSLATE_LIB)");

	/* *** SQL_ATTR_TRANSLATE_OPTION ------------ *** */
	BufferLength = SQL_NTS;
	rc = SQLGetConnectAttr( *phdbc,SQL_ATTR_TRANSLATE_OPTION,&pTranslateOption,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_TRANSLATE_OPTION)");

	/* *** SQL_ATTR_TXN_ISOLATION --------------- *** */
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetConnectAttr( *phdbc, SQL_ATTR_TXN_ISOLATION, &pTxnIsolation,
				BufferLength, &StringLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr (SQL_ATTR_TXN_ISOLATION)");

	/* --- Output ConnectOptons ---------------------------------------- */

	/* *** SQL_ATTR_ACCESS_MODE --------------- *** */
	switch (pAccessMode) {
	 case (SQL_MODE_READ_ONLY): lstrcpy(buffer,"SQL_MODE_READ_ONLY"); break;
	 case (SQL_MODE_READ_WRITE): lstrcpy(buffer,"SQL_MODE_READ_WRITE"); break;
	 default : lstrcpy(buffer, "?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ACCESS_MODE	: '%s' \r\n", buffer);

	/* *** SQL_ATTR_ASYNC_ENABLE ------------- *** */
	switch (pAsyncEnable) {
	 case (SQL_ASYNC_ENABLE_ON): lstrcpy(buffer,"SQL_ASYNC_ENABLE_ON"); break;
	 case (SQL_ASYNC_ENABLE_OFF): lstrcpy(buffer,"SQL_ASYNC_ENABLE_OFF"); break;
         default : lstrcpy(buffer, "?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ASYNC_ENABLE	: '%s' \r\n", buffer);

	/* *** SQL_ATTR_AUTO_IPD ---------------- *** */
	switch (pAutoIpd) {
	 case (SQL_TRUE): lstrcpy(buffer,"SQL_TRUE"); break;
	 case (SQL_FALSE): lstrcpy(buffer,"SQL_FALSE"); break;
	 default : lstrcpy(buffer, "?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_AUTO_IPD	: '%s' \r\n", buffer);

	/* *** SQL_ATTR_AUTOCOMMIT --------------- *** */
	switch (pAutoCommit) {
	 case (SQL_AUTOCOMMIT_ON): lstrcpy(buffer,"SQL_AUTOCOMMIT_ON"); break;
	 case (SQL_AUTOCOMMIT_OFF): lstrcpy(buffer,"SQL_AUTOCOMMIT_OFF"); break;
	 default : lstrcpy(buffer, "?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_AUTOCOMMIT	: '%s' \r\n", buffer);

	/* *** SQL_ATTR_AUTOCOMMIT --------------- *** */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_CONNECTION_TIMEOUT : '%d' \r\n", pConTimeout);

	/* *** SQL_ATTR_CURRENT_CATALOG --------------- *** */
	/* %s - pCurrentQualifier */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_CURRENT_CATALOG	: '-' \r\n");

	/* *** SQL_ATTR_LOGIN_TIMEOUT --------- *** */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_LOGIN_TIMEOUT	: '%d' \r\n", pLoginTimeout);

	/* *** SQL_ATTR_ODBC_CURSORS ---------- *** */
	/* %d - pOdbcCursors */
	switch(pOdbcCursors) {
	 case (SQL_CUR_USE_IF_NEEDED): lstrcpy(buffer,"SQL_CUR_USE_IF_NEEDED");break;
	 case (SQL_CUR_USE_ODBC): lstrcpy(buffer,"SQL_CUR_USE_ODBC"); break;
	 case (SQL_CUR_USE_DRIVER) : lstrcpy(buffer,"SQL_CUR_USE_DRIVER"); break;
	 default : lstrcpy(buffer, "?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_CURSORS	: '%s' \r\n", buffer);

	/* *** SQL_ATTR_TRACE --------------- *** */
	switch (pOptTrace) {
	 case (SQL_OPT_TRACE_ON): lstrcpy(buffer,"SQL_OPT_TRACE_ON"); break;
	 case (SQL_OPT_TRACE_OFF): lstrcpy(buffer,"SQL_OPT_TRACE_OFF"); break;
	 default : lstrcpy(buffer, "?????");
        }
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_TRACE	: '%s' \r\n", buffer);

	/* *** SQL_ATTR_TRACEFILE --------------- *** */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_TRACEFILE	: '%s' \r\n", pOptTraceFile);

	/* *** SQL_ATTR_PACKET_SIZE ------------- *** */
	/* %d - pPacketSize */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_PACKET_SIZE	: '-' \r\n");

	/* *** SQL_ATTR_QUIET_MODE -------------- *** */
	/* %d - pQuietMode */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_QUIET_MODE	: '%d' \r\n", pQuietMode);

	/* *** SQL_ATTR_TRANSLATE_LIB ----------- *** */
	/* %s - pTranslateDLL */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_TRANSLATE_LIB	: '%s' \r\n", pTranslateDLL);

	/* *** SQL_ATTR_TRANSLATE_OPTION --------------- *** */
	/* %d - pTranslateOption */
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_TRANSLATE_OPTION	: '%d' \r\n", pTranslateOption);

	/* *** SQL_ATTR_TXN_ISOLATION --------------- *** */
	/* %d - pTxnIsolation */
	switch (pTxnIsolation) {
	 case (SQL_TXN_READ_UNCOMMITTED) : lstrcpy(buffer,"SQL_TXN_READ_UNCOMMITTED"); break;
	 case (SQL_TXN_READ_COMMITTED) : lstrcpy(buffer,"SQL_TXN_READ_COMMITTED"); break;
	 case (SQL_TXN_REPEATABLE_READ) : lstrcpy(buffer,"SQL_TXN_REPEATABLE_READ"); break;
	 case (SQL_TXN_SERIALIZABLE) : lstrcpy(buffer,"SQL_TXN_SERIALIZABLE"); break;
	 default : lstrcpy(buffer, "?????");
	}
	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_TXN_ISOLATION	: '%s' \r\n", buffer);

	/* --- SQLSetConnectAttr ----------------------------------------- */

	if (pAutoCommit == SQL_AUTOCOMMIT_ON) {
   	szLogPrintf(lpSrvr,FALSE,"\t Set SQL_ATTR_AUTOCOMMIT to OFF \r\n");
		vAutoCommit =  SQL_AUTOCOMMIT_OFF;  /* (o. SQL_AUTOCOMMIT_ON) */
		rc = SQLSetConnectAttr( *phdbc, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
   	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetConnectAttr (SQL_AUTOCOMMIT_OFF)");
	}
	vConTimeout = 60;
	if (vConTimeout > 0) {
   	szLogPrintf(lpSrvr,FALSE,"\t Set SQL_ATTR_CONNECTION_TIMEOUT = '%d' \r\n", vConTimeout);
		rc = SQLSetConnectAttr( *phdbc, SQL_ATTR_CONNECTION_TIMEOUT, &vConTimeout, SQL_IS_UINTEGER);
   	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetConnectAttr (SQL_ATTR_CONNECTION_TIMEOUT)");
	}

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
