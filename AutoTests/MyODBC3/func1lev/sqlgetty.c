/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGETTY.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGetTypeInfo
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLGetTypeInfo"
#define TABLE ""

#define MAX_NAME_LEN 50

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLGetTypeInfo:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetTypeInfo(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR  buffer[STR_LEN];

	/* Declare storage locations for result set data */

	UCHAR  szTypeName[STR_LEN], szLiteralPrefix[STR_LEN];
	UCHAR  szLiteralSuffix[STR_LEN], szCreateParams[STR_LEN];
	UCHAR  szLocalTypeName[STR_LEN];

	SDWORD Precision;

	SWORD DataType, Nullable, CaseSensitive, Searchable, UnsignedAttribute;
	SWORD Money, AutoIncrement;
	SWORD MinimumScale, MaximumScale;

	/* Declare storage locations for bytes available to return */

	SDWORD cbTypeName, cbDataType, cbPrecision, cbLiteralPrefix;
	SDWORD cbLiteralSuffix, cbCreateParams, cbNullable, cbCaseSensitive;
	SDWORD cbSearchable, cbUnsignedAttribute, cbMoney, cbAutoIncrement;
	SDWORD cbLocalTypeName, cbMinimumScale, cbMaximumScale;

	SWORD  count=0;
	SWORD  cAbort=0;
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

/* ------------------------------------------------------------------------- */
	/* All qualifiers, all owners, EMPLOYEE table, all columns */

	rc = SQLGetTypeInfo(*phstmt, SQL_ALL_TYPES);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetTypeInfo");

	if (rc == SQL_SUCCESS) {
	/* Bind columns in result set to storage locations */

	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szTypeName, STR_LEN,&cbTypeName);
	SQLBindCol(*phstmt, 2, SQL_C_SHORT, &DataType, STR_LEN, &cbDataType);
	SQLBindCol(*phstmt, 3, SQL_C_LONG, &Precision, 0,&cbPrecision);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szLiteralPrefix, STR_LEN, &cbLiteralPrefix);
	SQLBindCol(*phstmt, 5, SQL_C_CHAR, szLiteralSuffix, STR_LEN, &cbLiteralSuffix);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szCreateParams, STR_LEN, &cbCreateParams);
	SQLBindCol(*phstmt, 7, SQL_C_SHORT, &Nullable, 0, &cbNullable);
	SQLBindCol(*phstmt, 8, SQL_C_SHORT, &CaseSensitive, 0, &cbCaseSensitive);
	SQLBindCol(*phstmt, 9, SQL_C_SHORT, &Searchable, 0, &cbSearchable);
	SQLBindCol(*phstmt,10, SQL_C_SHORT, &UnsignedAttribute, 0, &cbUnsignedAttribute);
	SQLBindCol(*phstmt,11, SQL_C_SHORT, &Money, 0, &cbMoney);
	SQLBindCol(*phstmt,12, SQL_C_SHORT, &AutoIncrement, 0, &cbAutoIncrement);
	SQLBindCol(*phstmt,13, SQL_C_CHAR, szLocalTypeName, STR_LEN, &cbLocalTypeName);
	SQLBindCol(*phstmt,14, SQL_C_SHORT, &MinimumScale, 0, &cbMinimumScale);
	SQLBindCol(*phstmt,15, SQL_C_SHORT, &MaximumScale, 0, &cbMaximumScale);

	szLogPrintf(lpSrvr, FALSE,
	  "\tGetTypeInfo:'TYPE_NAME','DATA_TYPE','PRECISION','LITERAL_PREFIX',\
	   'LITERAL_SUFFIX','CREATE_PARAMS','NULLABLE','CASE_SENSITIVE',\
	   'SEARCHABLE','UNSIGNED_ATTRIBUTE','MONEY','AUTO_INCREMENT',\
	   'LOCAL_TYPE_NAME','MINIMUM_SCALE','MAXIMUM_SCALE'\r\n");

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

		if (rc == SQL_ERROR || rc == SQL_SUCCESS_WITH_INFO) {
			/* Show Error */
			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLError");
		}
		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			szLogPrintf(lpSrvr, FALSE,"\tColumn %d : ", count);

			/* *** TYPE_NAME ------------------------- *** */
			szLogPrintf(lpSrvr, FALSE,"'%s',",szTypeName);

			/* *** DATA_TYPE ------------------------- *** */
			szLogPrintf(lpSrvr, FALSE,"'%d',",DataType);

			/* *** PRECISION ------------------------- *** */
			if (cbPrecision == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else	szLogPrintf(lpSrvr, FALSE,"'%ld',",Precision);

			/* *** LITERAL_PREFIX -------------------- *** */
			if (cbLiteralPrefix == SQL_NULL_DATA) lstrcpy(szLiteralPrefix, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',",szLiteralPrefix);

			/* *** LITERAL_SUFFIX -------------------- *** */
			if (cbLiteralSuffix == SQL_NULL_DATA) lstrcpy(szLiteralSuffix, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',",szLiteralSuffix);

			/* *** CREATE_PARAMS --------------------- *** */
			if (cbCreateParams == SQL_NULL_DATA) lstrcpy(szCreateParams, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',",szCreateParams);

			/* *** NULLABLE -------------------------- *** */
			Get_pfNullable(Nullable, buffer);
			szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);

			/* *** CASE_SENSITIVE ------------------------- *** */
			Get_BoolValue(CaseSensitive, buffer);
			szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);

			/* *** SEARCHABLE ------------------------ *** */
			Get_Searchable(Searchable, buffer);
			szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);

			/* *** UNSIGNED_ATTRIBUTE ---------------- *** */
			if (cbUnsignedAttribute == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else	{
				Get_BoolValue(UnsignedAttribute, buffer);
				szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);
				}

			/* *** MONEY ----------------------------- *** */
			Get_BoolValue(Money, buffer);
			szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);

			/* *** AUTO_INCREMENT -------------------- *** */
			if (cbAutoIncrement == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else	{
				Get_BoolValue(AutoIncrement, buffer);
				szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);
				}

			/* *** LOCAL_TYPE_NAME ------------------- *** */
			if (cbLocalTypeName == SQL_NULL_DATA) lstrcpy(szLocalTypeName, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',",szLocalTypeName);

			/* *** MINIMUM_SCALE --------------------- *** */
			if (cbMinimumScale == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else	szLogPrintf(lpSrvr, FALSE,"'%d',",MinimumScale);

			/* *** MAXIMUM_SCALE --------------------- *** */
			if (cbMaximumScale == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s'\r\n",NULL_VALUE);
			 else	szLogPrintf(lpSrvr, FALSE,"'%d'\r\n",MaximumScale);
		} else { break; }
	    }
	}
	// --- myodbc3: error crashes ---
	//rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
/* ---------------------------------------------------------------------har- */
	//rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	//rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	//rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	//rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------- */
