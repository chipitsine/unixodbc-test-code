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
** Purpose      : Auto Test DLL, Test von SQLSetStmtAttr
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLSetStmtAttr"
#define TABLE ""

#define MAX_NAME_LEN 256
#define MAX_RGB_VALUE 256
#define ERROR_TEXT_LEN 511

#define STR_LEN 128+1
#define REM_LEN 254+1

#define PARAM_UNTOUCHED 999999
//*-------------------------------------------------------------------------

#define SQLSTMT1 "SHOW TABLES;"

/* ------------------------------------------------------------------------ */
/* SQLGetStmtAttr, SQLSetStmtAttr Parameters : */
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
//| DoSQLSetStmtAttr:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLSetStmtAttr(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   buffer[MAX_NAME_LEN*20];
	UCHAR                   buf[MAX_NAME_LEN*20];

	SQLINTEGER              fOption;
	PTR                     pvParam;
	UDWORD		        pPar;
     /*	UDWORD                  vParam; */
     /* UCHAR                   pvParamChar[MAX_RGB_VALUE];*/
	SQLHANDLE               pvParamHandle;
	SQLPOINTER              pvParamPtr;

	SQLINTEGER		StrLengthPtr=0;
/* ---------------------------------------------------------------------har- */
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

	/* ----------------------------------------------------------------- */

	szLogPrintf(lpSrvr,FALSE,"\t ExecStatement : '%s' \r\n", SQLSTMT1);

   rc = SQLExecDirect(*phstmt, SQLSTMT1, SQL_NTS);
   DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

/* --- SQLGetStmtOption ---------------------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\tGetStmtAttr -> \r\n");

	/* *** 1. SQL_ATTR_APP_PARAM_DESC */
	fOption = SQL_ATTR_APP_PARAM_DESC; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t1. SQL_ATTR_APP_PARAM_DESC : \r\n");
	pvParamHandle= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamHandle,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 2. SQL_ATTR_APP_ROW_DESC */
	fOption = SQL_ATTR_APP_ROW_DESC; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t2. SQL_ATTR_APP_ROW_DESC : \r\n");
	pvParamHandle= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamHandle,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 3. SQL_ATTR_ASYNC_ENABLE */
	fOption = SQL_ATTR_ASYNC_ENABLE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t3. SQL_ATTR_ASYNC_ENABLE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	  case SQL_ASYNC_ENABLE_OFF : lstrcpy(buf,"SQL_ASYNC_ENABLE_OFF"); break;
	  case SQL_ASYNC_ENABLE_ON  : lstrcpy(buf,"SQL_ASYNC_ENABLE_ON"); break;
	  case PARAM_UNTOUCHED      : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	  default                   : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 4. SQL_ATTR_CONCURRENCY (ODBC 2.0) */
	fOption = SQL_ATTR_CONCURRENCY; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t4. SQL_ATTR_CONCURRENCY : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_CONCUR_READ_ONLY : lstrcpy(buf,"SQL_CONSUR_READ_ONLY"); break;
	 case SQL_CONCUR_LOCK      : lstrcpy(buf,"SQL_CONSUR_LOCK"); break;
	 case SQL_CONCUR_ROWVER    : lstrcpy(buf,"SQL_CONSUR_ROWCUR"); break;
	 case SQL_CONCUR_VALUES    : lstrcpy(buf,"SQL_CONSUR_VALUES"); break;
	 case PARAM_UNTOUCHED      : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	 default                   : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 5. SQL_ATTR_CURSOR_SCROLLABLE (ODBC 3.0) */
	fOption = SQL_ATTR_CURSOR_SCROLLABLE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t5. SQL_ATTR_CURSOR_SCROLLABLE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_NONSCROLLABLE : lstrcpy(buf,"SQL_NONSCROLLABLE"); break;
	 case SQL_SCROLLABLE    : lstrcpy(buf,"SQL_SCROLLABLE"); break;
	 default                : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 6. SQL_ATTR_CURSOR_SENSITIVITY (ODBC 3.0) */
	fOption = SQL_ATTR_CURSOR_SENSITIVITY; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t6. SQL_ATTR_CURSOR_SENSITIVITY : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_UNSPECIFIED : lstrcpy(buf,"SQL_UNSPECIFIED"); break;
	 case SQL_INSENSITIVE : lstrcpy(buf,"SQL_INSENSITIVE"); break;
	 case SQL_SENSITIVE   : lstrcpy(buf,"SQL_SENSITIVE"); break;
	 default              : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 7. SQL_ATTR_CURSOR_TYPE (ODBC 2.0) */
	fOption = SQL_ATTR_CURSOR_TYPE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t7. SQL_ATTR_CURSOR_TYPE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_CURSOR_FORWARD_ONLY : lstrcpy(buf,"SQL_CURSOR_FORWARD_ONLY");
					break;
	 case SQL_CURSOR_STATIC	      : lstrcpy(buf,"SQL_CURSOR_STATIC"); break;
	 case SQL_CURSOR_KEYSET_DRIVEN: lstrcpy(buf,"SQL_CURSOR_KEYSET_DRIVEN");
					break;
	 case SQL_CURSOR_DYNAMIC      : lstrcpy(buf,"SQL_CURSOR_DYNAMIC"); break;
	 case PARAM_UNTOUCHED         : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	 default		      : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 8. SQL_ATTR_ENABLE_AUTO_IPD (ODBC 3.0) */
	fOption = SQL_ATTR_ENABLE_AUTO_IPD; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t8. SQL_ATTR_ENABLE_AUTO_IPD : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_TRUE  : lstrcpy(buf,"SQL_TRUE"); break;
	 case SQL_FALSE : lstrcpy(buf,"SQL_FALSE"); break;
	 default        : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 9. SQL_ATTR_FETCH_BOOKMARK_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_FETCH_BOOKMARK_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t9. SQL_ATTR_FETCH_BOOKMARK_PTR : \r\n");
/* --- mysql ???
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
*/
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 10. SQL_ATTR_IMP_PARAM_DESC (ODBC 3.0) */
	fOption = SQL_ATTR_IMP_PARAM_DESC; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t10. SQL_ATTR_IMP_PARAM_DESC : \r\n");
	pvParamHandle= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamHandle,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 11. SQL_ATTR_IMP_ROW_DESC (ODBC 3.0) */
	fOption = SQL_ATTR_IMP_ROW_DESC; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t11. SQL_ATTR_IMP_ROW_DESC : \r\n");
	pvParamHandle= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamHandle,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 12. SQL_ATTR_KEYSET_SIZE (ODBC 2.0) */
	fOption = SQL_ATTR_KEYSET_SIZE;
	szLogPrintf(lpSrvr, FALSE,"\t12. SQL_ATTR_KEYSET_SIZE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 13. SQL_ATTR_MAX_LENGTH */
	fOption = SQL_ATTR_MAX_LENGTH;
	szLogPrintf(lpSrvr, FALSE,"\t13. SQL_ATTR_MAX_LENGTH : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 14. SQL_ATTR_MAX_ROWS */
	fOption = SQL_ATTR_MAX_ROWS;
	szLogPrintf(lpSrvr, FALSE,"\t14. SQL_ATTR_MAX_ROWS : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 15. SQL_ATTR_METADATA_ID (ODBC 3.0) */
	fOption = SQL_ATTR_METADATA_ID; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t15. SQL_ATTR_METADATA_ID : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_TRUE  : lstrcpy(buf,"SQL_TRUE"); break;
	 case SQL_FALSE : lstrcpy(buf,"SQL_FALSE"); break;
	 default        : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 16. SQL_ATTR_NOSCAN */
	fOption = SQL_ATTR_NOSCAN; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t16. SQL_ATTR_NOSCAN : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_NOSCAN_OFF  : lstrcpy(buf,"SQL_NOSCAN_OFF"); break;
	 case SQL_NOSCAN_ON   : lstrcpy(buf,"SQL_NOSCAN_ON"); break;
	 case PARAM_UNTOUCHED : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	 default              : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 17. SQL_ATTR_PARAM_BIND_OFFSET_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_PARAM_BIND_OFFSET_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t17. SQL_ATTR_PARAM_BIND_OFFSET_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 18. SQL_ATTR_PARAM_BIND_TYPE (ODBC 3.0) */
	fOption = SQL_ATTR_PARAM_BIND_TYPE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t18. SQL_ATTR_PARAM_BIND_TYPE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_PARAM_BIND_BY_COLUMN : lstrcpy(buf,"SQL_PARAM_BIND_BY_COLUMN");
					 break;
	 default                       : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 19. SQL_ATTR_PARAM_OPERATION_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_PARAM_OPERATION_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t19. SQL_ATTR_PARAM_OPERATION_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 20. SQL_ATTR_PARAM_STATUS_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_PARAM_STATUS_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t20. SQL_ATTR_PARAM_STATUS_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 21. SQL_ATTR_PARAMS_PROCESSED_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_PARAMS_PROCESSED_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t21. SQL_ATTR_PARAMS_PROCESSED_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 22. SQL_ATTR_PARAMSET_SIZE (ODBC 3.0) */
	fOption = SQL_ATTR_PARAMSET_SIZE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t22. SQL_ATTR_PARAMSET_SIZE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 23. SQL_ATTR_QUERY_TIMEOUT */
	fOption = SQL_ATTR_QUERY_TIMEOUT;
	szLogPrintf(lpSrvr, FALSE,"\t23. SQL_ATTR_QUERY_TIMEOUT : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 24. SQL_ATTR_RETRIEVE_DATA (ODBC 2.0) */
	fOption = SQL_ATTR_RETRIEVE_DATA; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t24. SQL_ATTR_RETRIEVE_DATA : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_RD_OFF	   : lstrcpy(buf,"SQL_RD_OFF"); break;
	 case SQL_RD_ON		   : lstrcpy(buf,"SQL_RD_ON"); break;
	 case PARAM_UNTOUCHED      : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	 default		   : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 25. SQL_ATTR_ROW_ARRAY_SIZE (ODBC 3.0) */
	fOption = SQL_ATTR_ROW_ARRAY_SIZE; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t25. SQL_ATTR_ROW_ARRAY_SIZE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 26. SQL_ATTR_ROW_BIND_OFFSET_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_ROW_BIND_OFFSET_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t26. SQL_ATTR_ROW_BIND_OFFSET_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 27. SQL_ATTR_ROW_BIND_TYPE */
	fOption = SQL_ATTR_ROW_BIND_TYPE;
	szLogPrintf(lpSrvr, FALSE,"\t27. SQL_ATTR_ROW_BIND_TYPE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_BIND_BY_COLUMN : lstrcpy(buf,"SQL_BIND_BY_COLUMN"); break;
	 default                 : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 28. SQL_ATTR_ROW_NUMBER (OBDC 2.0) */
	fOption = SQL_ATTR_ROW_NUMBER;
	szLogPrintf(lpSrvr, FALSE,"\t28. SQL_ATTR_ROW_NUMBER : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 29. SQL_ATTR_ROW_OPERATION_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_ROW_OPERATION_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t29. SQL_ATTR_ROW_OPERATION_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 30. SQL_ATTR_ROW_STATUS_PTR (ODBC 3.0) */
	fOption = SQL_ATTR_ROW_STATUS_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t30. SQL_ATTR_ROW_STATUS_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 31. SQL_ATTR_ROWS_FETCHED_PTR (ODBC 2.0) */
	fOption = SQL_ATTR_ROWS_FETCHED_PTR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t30. SQL_ATTR_ROWS_FETCHED_PTR : \r\n");
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParamPtr,SQL_IS_POINTER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	sprintf(buffer,"%p", pvParamPtr);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 32. SQL_ATTR_SIMULATE_CURSOR (ODBC 2.0) */
	fOption = SQL_ATTR_SIMULATE_CURSOR; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t32. SQL_ATTR_SIMULATE_CURSOR : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_SC_NON_UNIQUE : lstrcpy(buf,"SQL_SC_NON_UNIQUE"); break;
	 case SQL_SC_TRY_UNIQUE : lstrcpy(buf,"SQL_SC_TRY_UNIQUE"); break;
	 case SQL_SC_UNIQUE     : lstrcpy(buf,"SQL_SC_UNIQUE"); break;
	 case PARAM_UNTOUCHED   : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	 default                : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* *** 33. SQL_ATTR_USE_BOOKMARKS (ODBC 2.0) */
	fOption = SQL_ATTR_USE_BOOKMARKS; lstrcpy(buffer,"\0");
	szLogPrintf(lpSrvr, FALSE,"\t33. SQL_ATTR_USE_BOOKMARKS : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_UB_OFF      : lstrcpy(buf,"SQL_UB_OFF"); break;
	 case SQL_UB_ON       : lstrcpy(buf,"SQL_UB_ON"); break;
	 case PARAM_UNTOUCHED : lstrcpy(buf,"PARAM_UNTOUCHED"); break;
	 default              : lstrcpy(buf,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buf);

	/* ***  34. SQL_GET_BOOKMARK (ODBC 2.0) */
	fOption = SQL_GET_BOOKMARK;
	szLogPrintf(lpSrvr, FALSE,"\t34. SQL_GET_BOOKMARK : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

	/* *** 35. SQL_ROWSET_SIZE (ODBC 2.0) */
	fOption = SQL_ROWSET_SIZE;
	szLogPrintf(lpSrvr, FALSE,"\t35. SQL_ROWSET_SIZE : \r\n");
	pvParam= (SQLPOINTER) PARAM_UNTOUCHED;
	rc = SQLGetStmtAttr(*phstmt,fOption, &pvParam,SQL_IS_INTEGER, &StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",pvParam);

/* --- SQLSetStmtOption -------------------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\tSetStmtAttr -> \r\n");

	/* *** 3. SQL_ATTR_ASYNC_ENABLE */
	szLogPrintf(lpSrvr, FALSE,"\t3. SQL_ATTR_ASYNC_ENABLE : \r\n");
	rc = SQLSetStmtAttr(*phstmt, SQL_ATTR_ASYNC_ENABLE,
		(SQLPOINTER) SQL_ASYNC_ENABLE_ON, StrLengthPtr);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr");

	/* *** 16. SQL_ATTR_NOSCAN */
	szLogPrintf(lpSrvr, FALSE,"\t16. SQL_ATTR_NOSCAN : (-> SQL_NOSCAN_ON) \r\n");
	rc = SQLSetStmtAttr(*phstmt, SQL_ATTR_NOSCAN,
		(SQLPOINTER) SQL_NOSCAN_ON, StrLengthPtr);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetStmtAttr");

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
/* ------------------------------------------------------------------------- */
