/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGETINFO.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGETINFO
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

#define TESTNAME "SQLGetInfo"
#define TABLE ""

#define MAX_RGB_VALUE 300
#define MAX_NAME_LEN  50

#define GETINFO_ALL
/* #define GETINFO_OPT1 */
/* #define GETINFO_OPT2 */
/* #define GETINFO_OPT3 */
/* #define GETINFO_OPT4 */
/* #define GETINFO_OPT5 */
/* #define GETINFO_OPT6 */
/* #define GETINFO_OPT7 */
/* #define GETINFO_OPT8 */

/* ---------------------------------------------------------------------har- */

int PrintString(UCHAR *, DWORD, UCHAR *, lpSERVERINFO);

/* *****************************  Test Cases  ****************************** */
/*  The following functions implement the tests                              */
/* ************************************************************************* */

RETCODE PutDriverInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutDBMSProductInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutDataSourceInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutSupportedSQL(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutSQLLimits(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutScalarFunctionInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutConversionInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE PutODBC30Information(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
RETCODE CheckConversion(UDWORD rgbInfoValue, UCHAR buffer[MAX_NAME_LEN]);

/*
 ------------------------------------------------------------------------
| DoSQLGetInfo:
|     This test case will test the SQLGetInfo function.
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 -------------------------------------------------------------------------
*/
SWORD FAR PASCAL DoSQLGetInfo(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ---------------------------------------------------------------------har- */
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (HENV)");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_VERSION : 'SQL_OV_ODBC3' \r\n");

	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

/* ---------------------------------------------------------------------- */
/* ***** 1.) Driver Information */
/* ---------------------------------------------------------------------- */
PutDriverInformation(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 2.) DBMS Product Information */
/* ---------------------------------------------------------------------- */
PutDBMSProductInformation(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 3.) Data Source Information */
/* ---------------------------------------------------------------------- */
PutDataSourceInformation(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 4.) Supported SQL */
/* ---------------------------------------------------------------------- */
PutSupportedSQL(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 5.) SQL Limits */
/* ---------------------------------------------------------------------- */
PutSQLLimits(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 6.) Scalar Function Information */
/* ---------------------------------------------------------------------- */
PutScalarFunctionInformation(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 7.) Conversion Information */
/* ---------------------------------------------------------------------- */
PutConversionInformation(phenv, phdbc, phstmt, lpSrvr);

/* ---------------------------------------------------------------------- */
/* ***** 8.) ODBC 3.0 Information */
/* ---------------------------------------------------------------------- */
PutODBC30Information(phenv, phdbc, phstmt, lpSrvr);

/* ------------------------------------------------------------------har- */
DoAbort:
	rc = SQLDisconnect(*phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDisconnect");

	rc = SQLFreeHandle(SQL_HANDLE_DBC, *phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HDBC)");

	rc = SQLFreeHandle(SQL_HANDLE_ENV, *phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HENV)");

	/* check for errors */
	CHECKERRS(sErr);

	return sErr;
}

/* *********************************************************************** */
/*
 -------------------------------------------------------------------------
| 1.PutDriverInformation:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutDriverInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	UCHAR                   buffer[MAX_NAME_LEN*5];

	UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
	UDWORD                  rgbInfoValue;
#ifdef WIN16
	SDWORD FAR *            rgbInfoValueHandle;
#endif
#ifdef WIN32
	ULONG  FAR *            rgbInfoValueHandle;
#endif
#if !defined( WIN32 ) && !defined( WIN16 )
	SQLLEN            		*rgbInfoValueHandle;
#endif
	SWORD                   cbInfoValueMax;


#ifdef WIN16
	rgbInfoValueHandle = (SDWORD *) malloc(sizeof(SDWORD));
#endif
#ifdef WIN32
	rgbInfoValueHandle = (ULONG FAR *) malloc(sizeof(ULONG));
#endif
#if !defined( WIN32 ) && !defined( WIN16 )
	rgbInfoValueHandle = (ULONG FAR *) malloc(sizeof(SQLHANDLE));
#endif
/* ---------------------------------------------------------------------- */
/* ***** 1.) Driver Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT1)
	szLogPrintf(lpSrvr, FALSE," 1.) Driver Information -->\r\n");

	/* *** 1. SQL_ACTIVE_CONNECTIONS !*/

	szLogPrintf(lpSrvr, FALSE,"\t1.1.SQL_ACTIVE_CONNECTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ACTIVE_CONNECTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.1");
	sprintf(buffer,"%d",(SWORD) rgbInfoValue);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);
	/* *** 2. SQL_ACTIVE_STATEMENTS !*/

	szLogPrintf(lpSrvr, FALSE,"\t1.2.SQL_ACTIVE_STATEMENTS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ACTIVE_STATEMENTS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.2");
	sprintf(buffer,"%d",(SWORD) rgbInfoValue);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 3. SQL_DATA_SOURCE_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.3.SQL_DATA_SOURCE_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DATA_SOURCE_NAME, (PTR) rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.3");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 4. SQL_DRIVER_HDBC !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.4.SQL_DRIVER_HDBC : \r\n");
	cbInfoValueMax = sizeof(SDWORD);
#ifdef WIN32
	memcpy(rgbInfoValueHandle, phdbc, sizeof(HDBC));
	cbInfoValueMax= sizeof(ULONG);
#endif
#if !defined( WIN32 ) && !defined( WIN16 )
	memcpy(rgbInfoValueHandle, phdbc, sizeof(SQLHANDLE));
	cbInfoValueMax= sizeof(SQLHANDLE);
#endif
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_HDBC, (PTR) rgbInfoValueHandle,
				cbInfoValueMax, NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.4");
	sprintf(buffer,"%p", *rgbInfoValueHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 5. SQL_DRIVER_HENV !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.5.SQL_DRIVER_HENV : \r\n");
	cbInfoValueMax= sizeof(SDWORD);
#ifdef WIN32
	memcpy(rgbInfoValueHandle, phenv, sizeof(HENV));
	cbInfoValueMax= sizeof(ULONG);
#endif
#if !defined( WIN32 ) && !defined( WIN16 )
	memcpy(rgbInfoValueHandle, phenv, sizeof(SQLHANDLE));
	cbInfoValueMax= sizeof(SQLHANDLE);
#endif
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_HENV, (PTR) rgbInfoValueHandle,
				cbInfoValueMax, NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.5");
	sprintf(buffer,"%p", *rgbInfoValueHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 6. SQL_DRIVER_HLIB       (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.6.SQL_DRIVER_HLIB (ODBC 2.0) : \r\n");
	cbInfoValueMax= sizeof(SDWORD);
#ifdef WIN32
	memcpy(rgbInfoValueHandle, phdbc, sizeof(HDBC));
	cbInfoValueMax= sizeof(ULONG);
#endif
#if !defined( WIN32 ) && !defined( WIN16 )
	memcpy(rgbInfoValueHandle, phdbc, sizeof(SQLHANDLE));
	cbInfoValueMax= sizeof(SQLHANDLE);
#endif
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_HLIB, (PTR) rgbInfoValueHandle,
				cbInfoValueMax, NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.6");
	sprintf(buffer,"%p", *rgbInfoValueHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 7. SQL_DRIVER_HSTMT ! */
	szLogPrintf(lpSrvr, FALSE,"\t1.7.SQL_DRIVER_HSTMT : \r\n");
	cbInfoValueMax= sizeof(SDWORD);
#ifdef WIN16
	*rgbInfoValueHandle = (SDWORD) *phstmt;
#endif
#ifdef WIN32
	memcpy(rgbInfoValueHandle, phstmt, sizeof(HSTMT));
	cbInfoValueMax= sizeof(ULONG);
#endif
#if !defined( WIN32 ) && !defined( WIN16 )
	memcpy(rgbInfoValueHandle, phstmt, sizeof(SQLHANDLE));
	cbInfoValueMax= sizeof(SQLHANDLE);
#endif
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_HSTMT, (PTR) rgbInfoValueHandle,
				cbInfoValueMax, NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.7");
	sprintf(buffer,"%p", *rgbInfoValueHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 8. SQL_DRIVER_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.8.SQL_DRIVER_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_NAME, (PTR) rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.8");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 9. SQL_DRIVER_ODBC_VER   (ODBC 2.0) */
	szLogPrintf(lpSrvr, FALSE,"\t1.9.SQL_DRIVER_ODBC_VER (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_ODBC_VER, (PTR) rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.9");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 10. SQL_DRIVER_VER */
	szLogPrintf(lpSrvr, FALSE,"\t1.10.SQL_DRIVER_VER : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_VER, (PTR) rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.10");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 11.SQL_FETCH_DIRECTION !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.11.SQL_FETCH_DIRECTION : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_FETCH_DIRECTION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.11");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_FD_FETCH_NEXT)     lstrcat(buffer,"SQL_FD_FETCH_NEXT | ");
	if (rgbInfoValue & SQL_FD_FETCH_FIRST)    lstrcat(buffer,"SQL_FD_FETCH_FIRST |");
	if (rgbInfoValue & SQL_FD_FETCH_LAST)     lstrcat(buffer,"SQL_FD_FETCH_LAST |");
	if (rgbInfoValue & SQL_FD_FETCH_PRIOR)    lstrcat(buffer,"SQL_FD_FETCH_PRIOR |");
	if (rgbInfoValue & SQL_FD_FETCH_ABSOLUTE) lstrcat(buffer,"SQL_FD_FETCH_ABSOLUTE |");
	if (rgbInfoValue & SQL_FD_FETCH_RELATIVE) lstrcat(buffer,"SQL_FD_FETCH_RELATIVE |");
	/* if (rgbInfoValue & SQL_FD_FETCH_RESUME)   lstrcat(buffer,"SQL_FD_FETCH_RESUME |"); */
	if (rgbInfoValue & SQL_FD_FETCH_BOOKMARK) lstrcat(buffer,"SQL_FD_FETCH_BOOKMARK");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 12.SQL_FILE_USAGE        (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.12.SQL_FILE_USAGE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_FILE_USAGE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.12");
	switch (rgbInfoValue) {
	 case SQL_FILE_NOT_SUPPORTED : lstrcpy(buffer,"SQL_FILE_NOT_SUPPORTED"); break;
	 case SQL_FILE_TABLE         : lstrcpy(buffer,"SQL_FILE_TABLE"); break;
	 case SQL_FILE_QUALIFIER     : lstrcpy(buffer,"SQL_FILE_QUALIFIER"); break;
	 default                     : sprintf(buffer,"%d",rgbInfoValue);
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 13.SQL_GETDATA_EXTENSIONS(ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.13.SQL_GETDATA_EXTENSIONS  (ODBC 2.0): \r\n");
	rc = SQLGetInfo(*phdbc, SQL_GETDATA_EXTENSIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.13");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_GD_ANY_COLUMN)lstrcat(buffer,"SQL_GD_ANY_COLUMN |");
	if (rgbInfoValue & SQL_GD_ANY_ORDER) lstrcat(buffer,"SQL_GD_ANY_ORDER |");
	if (rgbInfoValue & SQL_GD_BLOCK)     lstrcat(buffer,"SQL_GD_BLOCK |");
	if (rgbInfoValue & SQL_GD_BOUND)     lstrcat(buffer,"SQL_GD_BOUND");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 14.SQL_LOCK_TYPES        (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.14.SQL_LOCK_TYPES (ODBC 2.0): \r\n");
	rc = SQLGetInfo(*phdbc, SQL_LOCK_TYPES, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.14");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_LCK_NO_CHANGE)   lstrcat(buffer,"SQL_LCK_NO_CHANGE |");
	if (rgbInfoValue & SQL_LCK_EXCLUSIVE)   lstrcat(buffer,"SQL_LCK_EXCLUSIVE |");
	if (rgbInfoValue & SQL_LCK_UNLOCK)      lstrcat(buffer,"SQL_LCK_UNLOCK");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 15.SQL_ODBC_API_CONFORMANCE !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.15.SQL_ODBC_API_CONFORMANCE : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ODBC_API_CONFORMANCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.15");
	switch (rgbInfoValue) {
	 case SQL_OAC_NONE   : lstrcpy(buffer,"SQL_OAC_NONE"); break;
	 case SQL_OAC_LEVEL1 : lstrcpy(buffer,"SQL_OAC_LEVEL1"); break;
	 case SQL_OAC_LEVEL2 : lstrcpy(buffer,"SQL_OAC_LEVEL2"); break;
	 default             : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 16.SQL_ODBC_SAG_CLI_CONFORMANCE !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.16.SQL_ODBC_SAG_CLI_CONFORMANCE : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ODBC_SAG_CLI_CONFORMANCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.16");
	switch (rgbInfoValue) {
	 case SQL_OSCC_NOT_COMPLIANT : lstrcpy(buffer,"SQL_OSSCC_NOT_COMPLIANT"); break;
	 case SQL_OSCC_COMPLIANT     : lstrcpy(buffer,"SQL_OSSCC_COMPLIANT"); break;
	 default                     : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 17.SQL_ODBC_VER */
	szLogPrintf(lpSrvr, FALSE,"\t1.17.SQL_ODBC_VER : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ODBC_VER, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.17");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 18.SQL_POS_OPERATIONS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.18.SQL_POS_OPERATIONS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_POS_OPERATIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.18");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_POS_POSITION)  lstrcat(buffer,"SQL_POS_POSITION |");
	if (rgbInfoValue & SQL_POS_REFRESH)   lstrcat(buffer,"SQL_POS_REFRESH |");
	if (rgbInfoValue & SQL_POS_UPDATE)    lstrcat(buffer,"SQL_POS_UPDATE |");
	if (rgbInfoValue & SQL_POS_DELETE)    lstrcat(buffer,"SQL_POS_DELETE |");
	if (rgbInfoValue & SQL_POS_ADD)       lstrcat(buffer,"SQL_POS_ADD");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 19.SQL_ROW_UPDATES !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.19.SQL_ROW_UPDATES : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ROW_UPDATES, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.19");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 20.SQL_SEARCH_PATTERN_ESCAPE !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.20.SQL_SEARCH_PATTERN_ESCAPE : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SEARCH_PATTERN_ESCAPE, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.20");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 21.SQL_SERVER_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t1.21.SQL_SERVER_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SERVER_NAME, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 1.21");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

DoAbort:
	free(rgbInfoValueHandle);

	return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 2.PutDBMSProductInformation:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutDBMSProductInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;

	UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
/* ---------------------------------------------------------------------- */
/* ***** 2.) DBMS Product Information */
/* ------------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT2)
	szLogPrintf(lpSrvr, FALSE," 2.) DBMS Product Information -->\r\n");

	/* *** 1.SQL_DATABASE_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t2.1.SQL_DATABASE_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DATABASE_NAME, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 2.1");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 2.SQL_DBMS_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t2.2.SQL_DBMS_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DBMS_NAME, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 2.2");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 3.SQL_DBMS_VER !*/
	szLogPrintf(lpSrvr, FALSE,"\t2.3.SQL_DBMS_VER : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DBMS_VER, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 2.3");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);
DoAbort:
	return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 3.PutDataSourceInformation:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutDataSourceInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	UCHAR                   buffer[MAX_NAME_LEN*5];

	UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
	UDWORD                  rgbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 3.) Data Source Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT3)
	szLogPrintf(lpSrvr, FALSE," 3.) Data Source Information -->\r\n");

	/* *** 1.SQL_ACCESSIBLE_PROCEDURES */
	szLogPrintf(lpSrvr, FALSE,"\t3.1.SQL_ACCESSIBLE_PROCEDURES : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ACCESSIBLE_PROCEDURES, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.1");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 2.SQL_ACCESSIBLE_TABLES !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.2.SQL_ACCESSIBLE_TABLES : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ACCESSIBLE_TABLES, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.2");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 3.SQL_BOOKMARK_PERSISTENCE (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.3.SQL_BOOKMARK_PERSISTENCE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_BOOKMARK_PERSISTENCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.3");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_BP_CLOSE)        lstrcat(buffer,"SQL_BP_CLOSE |");
	if (rgbInfoValue & SQL_BP_DELETE)       lstrcat(buffer,"SQL_BP_DELETE |");
	if (rgbInfoValue & SQL_BP_DROP)         lstrcat(buffer,"SQL_BP_DROP |");
	if (rgbInfoValue & SQL_BP_SCROLL)       lstrcat(buffer,"SQL_BP_SCROLL |");
	if (rgbInfoValue & SQL_BP_TRANSACTION)  lstrcat(buffer,"SQL_BP_TRANSACTION |");
	if (rgbInfoValue & SQL_BP_UPDATE)       lstrcat(buffer,"SQL_BP_UPDATE |");
	if (rgbInfoValue & SQL_BP_OTHER_HSTMT)  lstrcat(buffer,"SQL_BP_OTHER_HSTMT");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 4.SQL_CONCAT_NULL_BEHAVIOR! */
	szLogPrintf(lpSrvr, FALSE,"\t3.4.SQL_CONCAT_NULL_BEHAVIOR : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CONCAT_NULL_BEHAVIOR, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.4");
	switch (rgbInfoValue) {
	 case SQL_CB_NULL       : lstrcpy(buffer,"SQL_CB_NULL"); break;
	 case SQL_CB_NON_NULL   : lstrcpy(buffer,"SQL_CB_NON_NULL"); break;
	 default                : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 5.SQL_CURCOR_COMMIT_BEHAVIOR !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.5.SQL_CURSOR_COMMIT_BEHAVIOR : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CURSOR_COMMIT_BEHAVIOR, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.5");
	switch (rgbInfoValue) {
	 case SQL_CB_DELETE   : lstrcpy(buffer,"SQL_CB_DELETE"); break;
	 case SQL_CB_CLOSE    : lstrcpy(buffer,"SQL_CB_CLOSE"); break;
	 case SQL_CB_PRESERVE : lstrcpy(buffer,"SQL_CB_PRESERVE"); break;
	 default              : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 6.SQL_CURSOR_ROLLBACK_BEHAVIOR !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.6.SQL_CURSOR_ROLLBACK_BEHAVIOR : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CURSOR_ROLLBACK_BEHAVIOR, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.6");
	switch (rgbInfoValue) {
	 case SQL_CB_DELETE   : lstrcpy(buffer,"SQL_CB_DELETE"); break;
	 case SQL_CB_CLOSE    : lstrcpy(buffer,"SQL_CB_CLOSE"); break;
	 case SQL_CB_PRESERVE : lstrcpy(buffer,"SQL_CB_PRESERVE"); break;
	 default              : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 7.SQL_DATA_SOURCE_READ_ONLY !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.7.SQL_DATA_SOURCE_READ_ONLY : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DATA_SOURCE_READ_ONLY, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.7");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 8.SQL_DEFAULT_TXN_ISOLATION !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.8.SQL_DEFAULT_TXN_ISOLATION : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DEFAULT_TXN_ISOLATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.8");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_TXN_READ_UNCOMMITTED) lstrcat(buffer,"SQL_TXN_READ_UNCOMMITTED |");
	if (rgbInfoValue & SQL_TXN_READ_COMMITTED)   lstrcat(buffer,"SQL_TXN_READ_COMMITTED |");
	if (rgbInfoValue & SQL_TXN_REPEATABLE_READ)  lstrcat(buffer,"SQL_TXN_REPEATABLE_READ |");
	if (rgbInfoValue & SQL_TXN_SERIALIZABLE)     lstrcat(buffer,"SQL_TXN_SERIALIZABLE |");
	/* if (rgbInfoValue & SQL_TXN_VERSIONING)       lstrcat(buffer,"SQL_TXN_VERSIONING"); */
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 9.SQL_MULT_RESULT_SETS !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.9.SQL_MULT_RESULT_SETS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MULT_RESULT_SETS, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.9");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 10.SQL_MULTIPLE_ACTIVE_TXN !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.10.SQL_MULTIPLE_ACTIVE_TXN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MULTIPLE_ACTIVE_TXN, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.10");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 11.SQL_NEED_LONG_DATA_LEN (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.11.SQL_NEED_LONG_DATA_LEN (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_NEED_LONG_DATA_LEN, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.11");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 12.SQL_NULL_COLLATION (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.12.SQL_NULL_COLLATION (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_NULL_COLLATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.12");
	switch (rgbInfoValue) {
	 case SQL_NC_END      : lstrcpy(buffer,"SQL_NC_END"); break;
	 case SQL_NC_HIGH     : lstrcpy(buffer,"SQL_NC_HIGH"); break;
	 case SQL_NC_LOW      : lstrcpy(buffer,"SQL_NC_LOW"); break;
	 case SQL_NC_START    : lstrcpy(buffer,"SQL_NC_START"); break;
	 default              : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 13.SQL_OWNER_TERM !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.13.SQL_OWNER_TERM : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_OWNER_TERM, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.13");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 14.SQL_PROCEDURE_TERM */
	szLogPrintf(lpSrvr, FALSE,"\t3.14.SQL_PROCEDURE_TERM : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_PROCEDURE_TERM, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.14");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 15.SQL_QUALIFIER_TERM !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.15.SQL_QUALIFIER_TERM : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_QUALIFIER_TERM, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.15");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 16.SQL_SCROLL_CONCURRENCY !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.16.SQL_SCROLL_CONCURRENCY : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SCROLL_CONCURRENCY, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.16");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_SCCO_READ_ONLY)   lstrcat(buffer,"SQL_SCCO_READ_ONLY |");
	if (rgbInfoValue & SQL_SCCO_LOCK)        lstrcat(buffer,"SQL_SCCO_LOCK |");
	if (rgbInfoValue & SQL_SCCO_OPT_ROWVER)  lstrcat(buffer,"SQL_SCCO_OPT_ROWVER |");
	if (rgbInfoValue & SQL_SCCO_OPT_VALUES)  lstrcat(buffer,"SQL_SCCO_OPT_VALUES |");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 17.SQL_SCROLL_OPTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.17.SQL_SCROLL_OPTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SCROLL_OPTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.17");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_SO_FORWARD_ONLY)   lstrcat(buffer,"SQL_SCCO_FORWARD_ONLY |");
	if (rgbInfoValue & SQL_SO_STATIC)         lstrcat(buffer,"SQL_SO_FORWARD_ONLY |");
	if (rgbInfoValue & SQL_SO_KEYSET_DRIVEN)  lstrcat(buffer,"SQL_SO_KEYSET_DRIVEN |");
	if (rgbInfoValue & SQL_SO_DYNAMIC)        lstrcat(buffer,"SQL_SO_DYNAMIC |");
	if (rgbInfoValue & SQL_SO_MIXED)          lstrcat(buffer,"SQL_SO_MIXED");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 18.SQL_STATIC_SENSITIVITY (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.18.SQL_STATIC_SENSITIVITY (ODBC 2.0): \r\n");
	rc = SQLGetInfo(*phdbc, SQL_STATIC_SENSITIVITY, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.18");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_SS_ADDITIONS) lstrcat(buffer,"SQL_ADDITIONS |");
	if (rgbInfoValue & SQL_SS_DELETIONS) lstrcat(buffer,"SQL_DELETIONS |");
	if (rgbInfoValue & SQL_SS_UPDATES)   lstrcat(buffer,"SQL_UPDATES");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 19.SQL_TABLE_TERM !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.19.SQL_TABLE_TERM : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_TABLE_TERM, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.19");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 20.SQL_TXN_CAPABLE !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.20.SQL_TXN_CAPABLE: \r\n");
	rc = SQLGetInfo(*phdbc, SQL_TXN_CAPABLE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.20");
	switch (rgbInfoValue) {
	 case SQL_TC_NONE       : lstrcpy(buffer,"SQL_ADDITIONS"); break;
	 case SQL_TC_DML        : lstrcpy(buffer,"SQL_TC_DML"); break;
	 case SQL_TC_DDL_COMMIT : lstrcpy(buffer,"SQL_TC_DDL_COMMIT"); break;
	 case SQL_TC_DDL_IGNORE : lstrcpy(buffer,"SQL_TC_DLL_IGNORE"); break;
	 case SQL_TC_ALL        : lstrcpy(buffer,"SQL_TC_ALL"); break;
	 default                : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 21.SQL_TXN_ISOLATION_OPTION !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.21.SQL_TXN_ISOLATION_OPTION : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_TXN_ISOLATION_OPTION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.21");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_TXN_READ_UNCOMMITTED) lstrcat(buffer,"SQL_TXN_READ_UNCOMMITTED |");
	if (rgbInfoValue & SQL_TXN_READ_COMMITTED)   lstrcat(buffer,"SQL_TXN_READ_COMMITTED |");
	if (rgbInfoValue & SQL_TXN_REPEATABLE_READ)  lstrcat(buffer,"SQL_TXN_REPEATABLE_READ |");
	if (rgbInfoValue & SQL_TXN_SERIALIZABLE)     lstrcat(buffer,"SQL_TXN_SERIALIZABLE |");
	/* if (rgbInfoValue & SQL_TXN_VERSIONING)       lstrcat(buffer,"SQL_TXN_VERSIONING"); */
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 22.SQL_USER_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t3.22.SQL_USER_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_USER_NAME, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 3.22");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

DoAbort:
	return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 4.PutSuppotedSQL:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutSupportedSQL(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	UCHAR                   buffer[MAX_NAME_LEN*5];

	UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE*3];
	UDWORD                  rgbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 4.) Supported SQL */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT4)
	szLogPrintf(lpSrvr, FALSE," 4.) Supported SQL -->\r\n");

	/* *** 1.SQL_ALTER_TABLE (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.1.SQL_ALTER_TABLE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ALTER_TABLE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.1");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_AT_ADD_COLUMN) lstrcat(buffer,"SQL_AT_ADD_COLUMN |");
	if (rgbInfoValue & SQL_AT_DROP_COLUMN) lstrcat(buffer,"SQL_AT_DROP_COLUMN");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 2.SQL_COLUMN_ALIAS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.2.SQL_COLUMN_ALIAS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_COLUMN_ALIAS, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.2");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);


	/* *** 3.SQL_CORRELATION_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.3.SQL_CORRELATION_NAME : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CORRELATION_NAME, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.3");
	switch (rgbInfoValue) {
	 case SQL_CN_NONE      : lstrcpy(buffer,"SQL_CN_NONE"); break;
	 case SQL_CN_DIFFERENT : lstrcpy(buffer,"SQL_CN_DIFFERENT"); break;
	 case SQL_CN_ANY       : lstrcpy(buffer,"SQL_CN_ANY"); break;
	 default               : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 4.SQL_EXPRESSIONS_IN_ORDERBY !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.4.SQL_EXPRESSIONS_IN_ORDERBY : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_EXPRESSIONS_IN_ORDERBY, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.4");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 5.SQL_GROUP_BY   (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.5.SQL_GROUP_BY (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_GROUP_BY, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.5");
	switch (rgbInfoValue) {
	 case SQL_GB_NOT_SUPPORTED            : lstrcpy(buffer,"SQL_GB_NOT_SUPPORTED"); break;
	 case SQL_GB_GROUP_BY_EQUALS_SELECT   : lstrcpy(buffer,"SQL_GB_GROUP_BY_EQUALS_SELECT"); break;
	 case SQL_GB_GROUP_BY_CONTAINS_SELECT : lstrcpy(buffer,"SQL_GB_GROUP_BY_CONTAINS_SELECT"); break;
	 case SQL_GB_NO_RELATION              : lstrcpy(buffer,"SQL_GB_NO_RELATION"); break;
	 default                              : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 6.SQL_IDENTIFIER_CASE !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.6.SQL_IDENTIFIER_CASE : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_IDENTIFIER_CASE, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.6");
	switch (rgbInfoValue) {
	 case SQL_IC_UPPER     : lstrcpy(buffer,"SQL_IC_UPPER"); break;
	 case SQL_IC_LOWER     : lstrcpy(buffer,"SQL_IC_LOWER"); break;
	 case SQL_IC_SENSITIVE : lstrcpy(buffer,"SQL_IC_SENSITIVE"); break;
	 case SQL_IC_MIXED     : lstrcpy(buffer,"SQL_IC_MIXED"); break;
	 default                              : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 7.SQL_IDENTIFIER_QUOTE_CHAR !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.7.SQL_IDENTIFIER_QUOTE_CHAR : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_IDENTIFIER_QUOTE_CHAR, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.7");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 8.SQL_KEYWORDS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.8.SQL_KEYWORDS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_KEYWORDS, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.8");
	sprintf(buffer, "\tValue = ");
	PrintString(rgbInfoValueChar, lstrlen(rgbInfoValueChar), buffer, lpSrvr);

	/* *** 9.SQL_LIKE_ESCAPE_CLAUSE  (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.9.SQL_LIKE_ESCAPE_CLAUSE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_LIKE_ESCAPE_CLAUSE, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.9");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 10.SQL_NON_NULLABLE_COLUMNS !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.10.SQL_NON_NULLABLE_COLUMNS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_NON_NULLABLE_COLUMNS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.10");
	switch (rgbInfoValue) {
	 case SQL_NNC_NULL     : lstrcpy(buffer,"SQL_NNC_NULL"); break;
	 case SQL_NNC_NON_NULL : lstrcpy(buffer,"SQL_NNC_NON_NULL"); break;
	 default               : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 11.SQL_ODBC_SQL_CONFORMANCE !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.11.SQL_ODBC_SQL_CONFORMANCE : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ODBC_SQL_CONFORMANCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.11");
	switch (rgbInfoValue) {
	 case SQL_OSC_MINIMUM  : lstrcpy(buffer,"SQL_OSC_MINIMUM"); break;
	 case SQL_OSC_CORE     : lstrcpy(buffer,"SQL_OSC_CORE"); break;
	 case SQL_OSC_EXTENDED : lstrcpy(buffer,"SQL_OSC_EXTENDED"); break;
	 default               : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 12.SQL_ODBC_SQL_OPT_IEF !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.12.SQL_ODBC_SQL_OPT_IEF : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ODBC_SQL_OPT_IEF, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.12");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 13.SQL_ORDER_BY_COLUMNS_IN_SELECT (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.13.SQL_ORDER_BY_COLUMNS_IN_SELECT (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ORDER_BY_COLUMNS_IN_SELECT, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.13");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 14.SQL_OUTER_JOINS !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.14.SQL_OUTER_JOINS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_OUTER_JOINS, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.14");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 15.SQL_OWNER_USAGE (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.15.SQL_OWNER_USAGE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_OWNER_USAGE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.15");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_OU_DML_STATEMENTS       ) lstrcat(buffer,"SQL_OU_DML_STATEMENTS |");
	if (rgbInfoValue & SQL_OU_PROCEDURE_INVOCATION ) lstrcat(buffer,"SQL_OU_PRECEDURE_INVOCATION |");
	if (rgbInfoValue & SQL_OU_TABLE_DEFINITION     ) lstrcat(buffer,"SQL_OU_TABLE_DEFINITION |");
	if (rgbInfoValue & SQL_OU_INDEX_DEFINITION     ) lstrcat(buffer,"SQL_OU_INDEX_DEFINITION |");
	if (rgbInfoValue & SQL_OU_PRIVILEGE_DEFINITION ) lstrcat(buffer,"SQL_OU_PRIVILEGE_DEFINITION");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 16.SQL_POSITIONED_STATEMENTS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.16.SQL_POSITIONED_STATEMENTS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_POSITIONED_STATEMENTS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.16");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_PS_POSITIONED_DELETE ) lstrcat(buffer,"SQL_PS_POSITIONED_DELETE |");
	if (rgbInfoValue & SQL_PS_POSITIONED_UPDATE ) lstrcat(buffer,"SQL_PS_POSITIONED_UPDATE |");
	if (rgbInfoValue & SQL_PS_SELECT_FOR_UPDATE ) lstrcat(buffer,"SQL_PS_SELECT_FOR_UPDATE");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 17.SQL_PROCEDURES */
	szLogPrintf(lpSrvr, FALSE,"\t4.17.SQL_PROCEDURES : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_PROCEDURES, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.17");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 18.SQL_QUALIFIER_LOCATION (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.18.SQL_QUALIFIER_LOCATION (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_QUALIFIER_LOCATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.18");
	switch (rgbInfoValue) {
	 case SQL_QL_START : lstrcpy(buffer,"SQL_QL_START"); break;
	 case SQL_QL_END   : lstrcpy(buffer,"SQL_QL_END"); break;
	 default           : sprintf(buffer,"%d", rgbInfoValue);
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 19.SQL_QUALIFIER_NAME_SEPARATOR !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.19.SQL_QUALIFIER_NAME_SEPARATOR : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_QUALIFIER_NAME_SEPARATOR, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.19");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 20.SQL_QUALIFIER_USAGE (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.20.SQL_QUALIFIER_USAGE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_QUALIFIER_USAGE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.20");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_QU_DML_STATEMENTS)        lstrcat(buffer,"SQL_QU_DML_STATEMENTS |");
	if (rgbInfoValue & SQL_QU_PROCEDURE_INVOCATION)  lstrcat(buffer,"SQL_QU_PROCEDURE_INVOCATION |");
	if (rgbInfoValue & SQL_QU_TABLE_DEFINITION)      lstrcat(buffer,"SQL_QU_TABLE_DEFINITION |");
	if (rgbInfoValue & SQL_QU_INDEX_DEFINITION)      lstrcat(buffer,"SQL_QU_INDEX_DEFINITION |");
	if (rgbInfoValue & SQL_QU_PRIVILEGE_DEFINITION)  lstrcat(buffer,"SQL_QU_PRIVILEGE_DEFINITION");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 21.SQL_QUOTED_IDENTIFIER_CASE                 (ODBC 2.0) */
	szLogPrintf(lpSrvr, FALSE,"\t4.21.SQL_QUOTED_IDENTIFIER_CASE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_QUOTED_IDENTIFIER_CASE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.21");
	switch (rgbInfoValue) {
	 case SQL_IC_UPPER     : lstrcpy(buffer,"SQL_IC_UPPER"); break;
	 case SQL_IC_LOWER     : lstrcpy(buffer,"SQL_IC_LOWER"); break;
	 case SQL_IC_SENSITIVE : lstrcpy(buffer,"SQL_IC_SENSITIVE"); break;
	 case SQL_IC_MIXED     : lstrcpy(buffer,"SQL_IC_MIXED"); break;
	 default               : lstrcpy(buffer,"!!!");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 22.SQL_SPECIAL_CHARACTERS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.22.SQL_SPECIAL_CHARACTERS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SPECIAL_CHARACTERS, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.22");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 23.SQL_SUBQUERIES (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.23.SQL_SUBQUERIES (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SUBQUERIES, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.23");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_SQ_CORRELATED_SUBQUERIES)  lstrcat(buffer,"SQL_SQ_CORRELATED_SUBQUERIES |");
	if (rgbInfoValue & SQL_SQ_COMPARISON)             lstrcat(buffer,"SQL_SQ_COMPARISON |");
	if (rgbInfoValue & SQL_SQ_EXISTS)                 lstrcat(buffer,"SQL_SQ_EXISTS |");
	if (rgbInfoValue & SQL_SQ_IN)                     lstrcat(buffer,"SQL_SQ_IN |");
	if (rgbInfoValue & SQL_SQ_QUANTIFIED)             lstrcat(buffer,"SQL_SQ_QUANTIFIED");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 24.SQL_UNION (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t4.24.SQL_UNION (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_UNION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 4.24");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_U_UNION)     lstrcat(buffer,"SQL_U_UNION |");
	if (rgbInfoValue & SQL_U_UNION_ALL) lstrcat(buffer,"SQL_U_UNION_ALL");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);
DoAbort:
	return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 5.PutSQLLimits:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutSQLLimits(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;

	UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
	UDWORD                  rgbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 5.) SQL Limits */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT5)
	szLogPrintf(lpSrvr, FALSE," 5.) SQL Limits -->\r\n");

	/* *** 1.SQL_MAX_BINARY_LITERAL_LEN (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.1.SQL_MAX_BINARY_LITERAL_LEN (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_BINARY_LITERAL_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.1");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 2.SQL_MAX_CHAR_LITERAL_LEN (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.2.SQL_MAX_CHAR_LITERAL_LEN (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_CHAR_LITERAL_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.2");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 3.SQL_MAX_COLUMN_NAME_LEN !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.3.SQL_MAX_COLUMN_NAME_LEN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_COLUMN_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.3");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 4.SQL_MAX_COLUMNS_IN_GROUP_BY (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.4.SQL_MAX_COLUMNS_IN_GROUP_BY (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_BINARY_LITERAL_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.4");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 5.SQL_MAX_COLUMNS_IN_ORDER_BY (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.5.SQL_MAX_COLUMNS_IN_ORDER_BY (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_COLUMNS_IN_ORDER_BY, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.5");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 6.SQL_MAX_COLUMNS_IN_INDEX (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.6.SQL_MAX_COLUMNS_IN_INDEX (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_COLUMNS_IN_INDEX, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.6");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 7.SQL_MAX_COLUMNS_IN_SELECT (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.7.SQL_MAX_COLUMNS_IN_SELECT (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_COLUMNS_IN_SELECT, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.7");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 8.SQL_MAX_COLUMNS_IN_TABLE (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.8.SQL_MAX_COLUMNS_IN_TABLE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_COLUMNS_IN_TABLE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.8");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 9.SQL_MAX_CURSOR_NAME_LEN !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.9.SQL_MAX_CURSOR_NAME_LEN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_CURSOR_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.9");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 10.SQL_MAX_INDEX_SIZE (OBDC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.10.SQL_MAX_INDEX_SIZE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_INDEX_SIZE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.10");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 11.SQL_MAX_OWNER_NAME_LEN !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.11.SQL_MAX_OWNER_NAME_LEN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_OWNER_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.11");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 12.SQL_MAX_PROCEDURE_NAME_LEN */
	szLogPrintf(lpSrvr, FALSE,"\t5.12.SQL_MAX_PROCEDURE_NAME_LEN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_PROCEDURE_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.12");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 13.SQL_MAX_QUALIFIER_NAME_LEN !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.13.SQL_MAX_QUALIFIER_NAME_LEN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_QUALIFIER_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.13");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 14.SQL_MAX_ROW_SIZE (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.14.SQL_MAX_ROW_SIZE (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_ROW_SIZE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.14");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 15.SQL_MAX_ROW_SIZE_INCLUDES_LONG (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.15.SQL_MAX_ROW_SIZE_INCLUDES_LONG (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_ROW_SIZE_INCLUDES_LONG, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.15");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 16.SQL_MAX_STATEMENT_LEN (OBDC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.16.SQL_MAX_STATEMENT_LEN (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_STATEMENT_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.16");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 17.SQL_MAX_TABLE_NAME_LEN !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.17.SQL_MAX_TABLE_NAME_LEN : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_TABLE_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.17");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 18.SQL_MAX_TABLES_IN_SELECT (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t5.18.SQL_MAX_TABLES_IN_SELECT (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_TABLES_IN_SELECT, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.18");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 19.SQL_MAX_USER_NAME_LEN                 (ODBC 2.0) */
	szLogPrintf(lpSrvr, FALSE,"\t5.19.SQL_MAX_USER_NAME_LEN (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_USER_NAME_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 5.19");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);
DoAbort:
	return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 6.PutScalarFunctionInformation
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutScalarFunctionInformation(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	UCHAR                   buffer[MAX_NAME_LEN*10];
	UCHAR                   bufxx[MAX_NAME_LEN*10];

	UDWORD                  rgbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 6.) Scalar Function Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT6)
	szLogPrintf(lpSrvr, FALSE," 6.) Scalar Function Information -->\r\n");

	/* *** 1.SQL_CONVERT_FUNCTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.1.SQL_CONVERT_FUNCTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CONVERT_FUNCTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.1");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_FN_CVT_CONVERT) lstrcat(buffer," SQL_FN_CVT_CONVERT ");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 2.SQL_NUMERIC_FUNCTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.2.SQL_NUMERIC_FUNCTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_NUMERIC_FUNCTIONS, (PTR) &rgbInfoValue,
			 sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.2");
	lstrcpy(buffer," ");
	if (rgbInfoValue &  SQL_FN_NUM_ABS      ) lstrcat(buffer,"SQL_FN_NUM_ABS |");
	if (rgbInfoValue &  SQL_FN_NUM_ACOS     ) lstrcat(buffer," SQL_FN_NUM_ACOS |");
	if (rgbInfoValue &  SQL_FN_NUM_ASIN     ) lstrcat(buffer," SQL_FN_NUM_ASIN |");
	if (rgbInfoValue &  SQL_FN_NUM_ATAN     ) lstrcat(buffer," SQL_FN_NUM_ATAN |");
	if (rgbInfoValue &  SQL_FN_NUM_ATAN2    ) lstrcat(buffer," SQL_FN_NUM_ATAN2 |");
	if (rgbInfoValue &  SQL_FN_NUM_CEILING  ) lstrcat(buffer," SQL_FN_NUM_CEILING |");
	if (rgbInfoValue &  SQL_FN_NUM_COS      ) lstrcat(buffer," SQL_FN_NUM_COS |");
	if (rgbInfoValue &  SQL_FN_NUM_COT      ) lstrcat(buffer," SQL_FN_NUM_COT |");
	if (rgbInfoValue &  SQL_FN_NUM_DEGREES  ) lstrcat(buffer," SQL_FN_NUM_DEGREES |");
	if (rgbInfoValue &  SQL_FN_NUM_EXP      ) lstrcat(buffer," SQL_FN_NUM_EXP |");
	if (rgbInfoValue &  SQL_FN_NUM_FLOOR    ) lstrcat(buffer," SQL_FN_NUM_FLOOR |");
	if (rgbInfoValue &  SQL_FN_NUM_LOG      ) lstrcat(buffer," SQL_FN_NUM_LOG |");
	if (rgbInfoValue &  SQL_FN_NUM_LOG10    ) lstrcat(buffer," SQL_FN_NUM_LOG10 |");
	if (rgbInfoValue &  SQL_FN_NUM_MOD      ) lstrcat(buffer," SQL_FN_NUM_MOD |");
	if (rgbInfoValue &  SQL_FN_NUM_PI       ) lstrcat(buffer," SQL_FN_NUM_PI |");
	if (rgbInfoValue &  SQL_FN_NUM_POWER    ) lstrcat(buffer," SQL_FN_NUM_POWER |");
	if (rgbInfoValue &  SQL_FN_NUM_RADIANS  ) lstrcat(buffer," SQL_FN_NUM_RADIANS |");
	if (rgbInfoValue &  SQL_FN_NUM_RAND     ) lstrcat(buffer," SQL_FN_NUM_RAND |");
	if (rgbInfoValue &  SQL_FN_NUM_ROUND    ) lstrcat(buffer," SQL_FN_NUM_ROUND |");
	if (rgbInfoValue &  SQL_FN_NUM_SIGN     ) lstrcat(buffer," SQL_FN_NUM_SIGN |");
	if (rgbInfoValue &  SQL_FN_NUM_SQRT     ) lstrcat(buffer," SQL_FN_NUM_SQRT |");
	if (rgbInfoValue &  SQL_FN_NUM_TAN      ) lstrcat(buffer," SQL_FN_NUM_TAN |");
	if (rgbInfoValue &  SQL_FN_NUM_TRUNCATE ) lstrcat(buffer," SQL_FN_NUM_TRUNCATE");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 3.SQL_STRING_FUNCTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.3.SQL_STRING_FUNCTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_STRING_FUNCTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.3");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_FN_STR_ASCII) lstrcat(buffer," SQL_FN_STR_ASCII |");
	if (rgbInfoValue & SQL_FN_STR_CHAR) lstrcat(buffer," SQL_FN_STR_CHAR |");
	if (rgbInfoValue & SQL_FN_STR_CONCAT) lstrcat(buffer," SQL_FN_STR_CONCAT |");
	if (rgbInfoValue & SQL_FN_STR_DIFFERENCE) lstrcat(buffer," SQL_FN_STR_DIFFERENCE |");
	if (rgbInfoValue & SQL_FN_STR_INSERT) lstrcat(buffer," SQL_FN_STR_INSERT |");
	if (rgbInfoValue & SQL_FN_STR_LCASE) lstrcat(buffer," SQL_FN_STR_LCASE |");
	if (rgbInfoValue & SQL_FN_STR_LEFT) lstrcat(buffer," SQL_FN_STR_LEFT |");
	if (rgbInfoValue & SQL_FN_STR_LENGTH) lstrcat(buffer," SQL_FN_STR_LENGTH |");
	if (rgbInfoValue & SQL_FN_STR_LOCATE) lstrcat(buffer," SQL_FN_STR_LOCATE |");
	if (rgbInfoValue & SQL_FN_STR_LOCATE_2) lstrcat(buffer," SQL_FN_STR_LOCATE_2 |");
	if (rgbInfoValue & SQL_FN_STR_LTRIM) lstrcat(buffer," SQL_FN_STR_LTRIM |");
	if (rgbInfoValue & SQL_FN_STR_REPEAT) lstrcat(buffer," SQL_FN_STR_REPEAT |");
	if (rgbInfoValue & SQL_FN_STR_RIGHT) lstrcat(buffer," SQL_FN_STR_RIGHT |");
	if (rgbInfoValue & SQL_FN_STR_RTRIM) lstrcat(buffer," SQL_FN_STR_RTRIM |");
	if (rgbInfoValue & SQL_FN_STR_SOUNDEX) lstrcat(buffer," SQL_FN_STR_SOUNDEX |");
	if (rgbInfoValue & SQL_FN_STR_SPACE) lstrcat(buffer," SQL_FN_STR_SPACE |");
	if (rgbInfoValue & SQL_FN_STR_SUBSTRING) lstrcat(buffer," SQL_FN_STR_SUBSTRING |");
	if (rgbInfoValue & SQL_FN_STR_UCASE) lstrcat(buffer," SQL_FN_STR_UCASE");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 4.SQL_SYSTEM_FUNCTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.4.SQL_SYSTEM_FUNCTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_SYSTEM_FUNCTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.4");
	lstrcpy(buffer," ");
	if (rgbInfoValue &  SQL_FN_SYS_DBNAME   ) lstrcat(buffer," SQL_FN_SYS_DBNAME |");
	if (rgbInfoValue &  SQL_FN_SYS_IFNULL   ) lstrcat(buffer," SQL_FN_SYS_IFNULL |");
	if (rgbInfoValue &  SQL_FN_SYS_USERNAME ) lstrcat(buffer," SQL_FN_SYS_USERNAME ");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 5.SQL_TIMEDATE_ADD_INTERVALS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.5.SQL_TIMEDATE_ADD_INTERVALS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_TIMEDATE_ADD_INTERVALS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.5");
	lstrcpy(buffer," ");
	if (rgbInfoValue &  SQL_FN_TSI_FRAC_SECOND ) lstrcat(buffer," SQL_FN_TSI_FRAC_SECOND |");
	if (rgbInfoValue &  SQL_FN_TSI_SECOND      ) lstrcat(buffer," SQL_FN_TSI_SECOND |");
	if (rgbInfoValue &  SQL_FN_TSI_MINUTE      ) lstrcat(buffer," SQL_FN_TSI_MINUTE |");
	if (rgbInfoValue &  SQL_FN_TSI_HOUR        ) lstrcat(buffer," SQL_FN_TSI_HOUR |");
	if (rgbInfoValue &  SQL_FN_TSI_DAY         ) lstrcat(buffer," SQL_FN_TSI_DAY |");
	if (rgbInfoValue &  SQL_FN_TSI_WEEK        ) lstrcat(buffer," SQL_FN_TSI_WEEK |");
	if (rgbInfoValue &  SQL_FN_TSI_MONTH       ) lstrcat(buffer," SQL_FN_TSI_MONTH |");
	if (rgbInfoValue &  SQL_FN_TSI_QUARTER     ) lstrcat(buffer," SQL_FN_TSI_QUALIFIER |");
	if (rgbInfoValue &  SQL_FN_TSI_YEAR        ) lstrcat(buffer," SQL_FN_TSI_YEAR ");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 6.SQL_TIMEDATE_DIFF_INTERVALS (ODBC 2.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.6.SQL_TIMEDATE_DIFF_INTERVALS (ODBC 2.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_TIMEDATE_DIFF_INTERVALS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.6");
	lstrcpy(buffer," ");
	if (rgbInfoValue &  SQL_FN_TSI_FRAC_SECOND ) lstrcat(buffer," SQL_FN_TSI_FRAC_SECOND |");
	if (rgbInfoValue &  SQL_FN_TSI_SECOND      ) lstrcat(buffer," SQL_FN_TSI_SECOND |");
	if (rgbInfoValue &  SQL_FN_TSI_MINUTE      ) lstrcat(buffer," SQL_FN_TSI_MINUTE |");
	if (rgbInfoValue &  SQL_FN_TSI_HOUR        ) lstrcat(buffer," SQL_FN_TSI_HOUR |");
	if (rgbInfoValue &  SQL_FN_TSI_DAY         ) lstrcat(buffer," SQL_FN_TSI_DAY |");
	if (rgbInfoValue &  SQL_FN_TSI_WEEK        ) lstrcat(buffer," SQL_FN_TSI_WEEK |");
	if (rgbInfoValue &  SQL_FN_TSI_MONTH       ) lstrcat(buffer," SQL_FN_TSI_MONTH |");
	if (rgbInfoValue &  SQL_FN_TSI_QUARTER     ) lstrcat(buffer," SQL_FN_TSI_QUALIFIER |");
	if (rgbInfoValue &  SQL_FN_TSI_YEAR        ) lstrcat(buffer," SQL_FN_TSI_YEAR ");

	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 7.SQL_TIMEDATE_FUNCTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t6.7.SQL_TIMEDATE_FUNCTIONS : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_TIMEDATE_FUNCTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 6.7");
	lstrcpy(buffer," ");
	if (rgbInfoValue &  SQL_FN_TD_CURDATE       ) lstrcat(buffer," SQL_FN_TD_CURDATE |");
	if (rgbInfoValue &  SQL_FN_TD_CURTIME       ) lstrcat(buffer," SQL_FN_TD_CURTIME |");
	if (rgbInfoValue &  SQL_FN_TD_DAYNAME       ) lstrcat(buffer," SQL_FN_TD_DAYNAME |");
	if (rgbInfoValue &  SQL_FN_TD_DAYOFMONTH    ) lstrcat(buffer," SQL_FN_TD_DAYOFMONTH |");
	if (rgbInfoValue &  SQL_FN_TD_DAYOFWEEK     ) lstrcat(buffer," SQL_FN_TD_DAYOFWEEK |");
	if (rgbInfoValue &  SQL_FN_TD_DAYOFYEAR     ) lstrcat(buffer," SQL_FN_TD_DAYOFYEAR |");
	if (rgbInfoValue &  SQL_FN_TD_HOUR          ) lstrcat(buffer," SQL_FN_TD_HOUR |");
	if (rgbInfoValue &  SQL_FN_TD_MINUTE        ) lstrcat(buffer," SQL_FN_TD_MINUTE |");
	if (rgbInfoValue &  SQL_FN_TD_MONTH         ) lstrcat(buffer," SQL_FN_TD_MONTH |");
	if (rgbInfoValue &  SQL_FN_TD_MONTHNAME     ) lstrcat(buffer," SQL_FN_TD_MONTHNAME |");
	if (rgbInfoValue &  SQL_FN_TD_NOW           ) lstrcat(buffer," SQL_FN_TD_NOW |");
	if (rgbInfoValue &  SQL_FN_TD_QUARTER       ) lstrcat(buffer," SQL_FN_TD_QUARTER |");
	if (rgbInfoValue &  SQL_FN_TD_SECOND        ) lstrcat(buffer," SQL_FN_TD_SECOND |");
	if (rgbInfoValue &  SQL_FN_TD_TIMESTAMPADD  ) lstrcat(buffer," SQL_FN_TD_TIMESTAMPADD |");
	if (rgbInfoValue &  SQL_FN_TD_TIMESTAMPDIFF ) lstrcat(buffer," SQL_FN_TD_TIMESTAMPDIFF |");
	if (rgbInfoValue &  SQL_FN_TD_WEEK          ) lstrcat(buffer," SQL_FN_TD_WEEK |");
	if (rgbInfoValue &  SQL_FN_TD_YEAR          ) lstrcat(buffer," SQL_FN_TD_YEAR");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

DoAbort:
	return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 7.PutConversionInformation
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutConversionInformation(HENV FAR * phenv, HDBC FAR * phdbc,
                             HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*10];

        UDWORD                  rgbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 7.Conversion Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT7)
        szLogPrintf(lpSrvr, FALSE," 7.) Conversion Information -->\r\n");

        /* *** 1.SQL_CONVERT_BIGINT */
        szLogPrintf(lpSrvr, FALSE,"\t7.1.SQL_COVERT_BIGINT : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_BIGINT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.1");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 2.SQL_CONVERT_BINARY */
        szLogPrintf(lpSrvr, FALSE,"\t7.2.SQL_COVERT_BINARY : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_BINARY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.2");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 3.SQL_CONVERT_BIT  */
        szLogPrintf(lpSrvr, FALSE,"\t7.3.SQL_COVERT_BIT : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_BIT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.3");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 4.SQL_CONVERT_CHAR */
        szLogPrintf(lpSrvr, FALSE,"\t7.4.SQL_COVERT_CHAR : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_CHAR, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.4");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 5.SQL_CONVERT_DATE */
        szLogPrintf(lpSrvr, FALSE,"\t7.5.SQL_COVERT_DATE : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_DATE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.5");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 6.SQL_CONVERT_DECIMAL */
        szLogPrintf(lpSrvr, FALSE,"\t7.6.SQL_COVERT_DECIMAL : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_DECIMAL, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.6");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 7.SQL_CONVERT_DOUBLE */
        szLogPrintf(lpSrvr, FALSE,"\t7.7.SQL_COVERT_DOUBLE : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_DOUBLE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.7");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 8.SQL_CONVERT_FLOAT */
        szLogPrintf(lpSrvr, FALSE,"\t7.8.SQL_COVERT_FLOAT : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_FLOAT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.8");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 9.SQL_CONVERT_INTEGER */
        szLogPrintf(lpSrvr, FALSE,"\t7.9.SQL_COVERT_INTEGER : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_INTEGER, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.9");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 10.SQL_CONVERT_LONGVARBINARY */
        szLogPrintf(lpSrvr, FALSE,"\t7.10.SQL_COVERT_LONGVARBINARY : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_LONGVARBINARY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.10");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 11.SQL_CONVERT_LONGVARCHAR */
        szLogPrintf(lpSrvr, FALSE,"\t7.11.SQL_COVERT_LONGVARCHAR : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_LONGVARCHAR, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.11");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 12.SQL_CONVERT_NUMERIC */
        szLogPrintf(lpSrvr, FALSE,"\t7.12.SQL_COVERT_NUMERIC : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_NUMERIC, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.12");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 13.SQL_CONVERT_REAL */
        szLogPrintf(lpSrvr, FALSE,"\t7.13.SQL_COVERT_REAL : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_REAL, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.13");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 14.SQL_CONVERT_SMALLINT */
        szLogPrintf(lpSrvr, FALSE,"\t7.14.SQL_COVERT_SMALLINT : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_SMALLINT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.14");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 15.SQL_CONVERT_TIME */
        szLogPrintf(lpSrvr, FALSE,"\t7.15.SQL_COVERT_TIME : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_TIME, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.15");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 16.SQL_CONVERT_TIMESTAMP */
        szLogPrintf(lpSrvr, FALSE,"\t7.16.SQL_COVERT_TIMESTAMP : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_TIMESTAMP, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.16");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 17.SQL_CONVERT_TINYINT */
        szLogPrintf(lpSrvr, FALSE,"\t7.17.SQL_COVERT_TINYINT : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_TINYINT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.17");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 18.SQL_CONVERT_VARBINARY */
        szLogPrintf(lpSrvr, FALSE,"\t7.18.SQL_COVERT_VARBINARY : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_VARBINARY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.18");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

        /* *** 19.SQL_CONVERT_VARCHAR */
        szLogPrintf(lpSrvr, FALSE,"\t7.19.SQL_COVERT_VARCHAR : \r\n");
        rc = SQLGetInfo(*phdbc, SQL_CONVERT_VARCHAR, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
        ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo 7.19");

        CheckConversion(rgbInfoValue, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);
DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 8.PutODBC30Information
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutODBC30Information(HENV FAR * phenv, HDBC FAR * phdbc,
			     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	UCHAR                   buffer[MAX_NAME_LEN*20];
	UCHAR                   bufxx[MAX_NAME_LEN*10];

	UDWORD                  rgbInfoValue;
	UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE*10];

#ifdef WIN16
	SDWORD FAR *            rgbInfoValueHandle;
#endif
#ifdef WIN32
	ULONG  FAR *            rgbInfoValueHandle;
#endif
#ifndef WIN32
	SQLLEN					*rgbInfoValueHandle;
#endif
	SWORD                   cbInfoValueMax;

   SQLHDESC                hdesc;

#ifdef WIN16
	rgbInfoValueHandle = (SDWORD *) malloc(sizeof(SDWORD));
#endif
#ifdef WIN32
	rgbInfoValueHandle = (ULONG FAR *) malloc(sizeof(ULONG));
#endif

/* ---------------------------------------------------------------------- */
/* ***** 8.ODBC 3.0 Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT8)
	szLogPrintf(lpSrvr, FALSE," 8.) ODBC 3.0 Information -->\r\n");

	/* *** 1.   SQL_ACTIVE_ENVIRONMENTS (ODBC 3.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.1.SQL_SUBQUERIES (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,    SQL_ACTIVE_ENVIRONMENTS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.1");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 2.SQL_AGGREGATE_FUNCTIONS !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.2.SQL_AGGREGATE_FUNCTIONS (ODBC 3.0): \r\n");
	rc = SQLGetInfo(*phdbc, SQL_AGGREGATE_FUNCTIONS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.2");
	lstrcpy(buffer," ");
	if (rgbInfoValue &  SQL_AF_ALL      ) lstrcat(buffer," SQL_AF_ALL |");
	if (rgbInfoValue &  SQL_AF_AVG      ) lstrcat(buffer," SQL_AF_AVG |");
	if (rgbInfoValue &  SQL_AF_COUNT    ) lstrcat(buffer," SQL_AF_COUNT |");
	if (rgbInfoValue &  SQL_AF_DISTINCT ) lstrcat(buffer," SQL_AF_DISTINCT |");
	if (rgbInfoValue &  SQL_AF_MAX      ) lstrcat(buffer," SQL_AF_MAX |");
	if (rgbInfoValue &  SQL_AF_MIN      ) lstrcat(buffer," SQL_AF_MIN |");
	if (rgbInfoValue &  SQL_AF_SUM      ) lstrcat(buffer," SQL_AF_SUM |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 3.SQL_ALTER_DOMAIN (ODBC 3.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.3.SQL_ALTER_DOMAIN (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ALTER_DOMAIN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.3");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_AD_ADD_DOMAIN_CONSTRAINT)  lstrcat(buffer,"SQL_AD_ADD_DOMAIN_CONSTRAINT |");
	if (rgbInfoValue & SQL_AD_ADD_DOMAIN_DEFAULT)     lstrcat(buffer,"SQL_AD_ADD_DOMAIN_DEFAULT");
	if (rgbInfoValue & SQL_AD_DROP_DOMAIN_CONSTRAINT) lstrcat(buffer,"SQL_AD_DROP_DOMIAN_CONSTRAINT |");
	if (rgbInfoValue & SQL_AD_DROP_DOMAIN_DEFAULT)    lstrcat(buffer,"SQL_AD_DROP_DOMAIN_DEFAULT");
	if (rgbInfoValue & SQL_AD_CONSTRAINT_NAME_DEFINITION) lstrcat(buffer,"SQL_AD_CONSTRAINT_NAME_DEFINITION");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 4.SQL_ASYNC_MODE   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.4.SQL_ASYNC_MODE (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_ASYNC_MODE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.4");
	switch (rgbInfoValue) {
	 case SQL_AM_CONNECTION : lstrcpy(buffer,"SQL_AM_CONNECTION"); break;
	 case SQL_AM_STATEMENT   : lstrcpy(buffer,"SQL_AM_STATEMENT"); break;
	 case SQL_AM_NONE       : lstrcpy(buffer,"SQL_AM_NONE"); break;
	 default                : lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 5.SQL_BATCH_ROW_COUNT   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.5.SQL_BATCH_ROW_COUNT (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_BATCH_ROW_COUNT, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.5");
	switch (rgbInfoValue) {
	 case SQL_BRC_ROLLED_UP	 : lstrcpy(buffer,"SQL_BRC_ROLLED_UP"); break;
	 case SQL_BRC_PROCEDURES : lstrcpy(buffer,"SQL_BRC_PROCEDURES"); break;
	 case SQL_BRC_EXPLICIT   : lstrcpy(buffer,"SQL_BRC_EXPLICT"); break;
	 default                 : lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 6.SQL_BATCH_SUPPORT   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.6.SQL_BATCH_SUPPORT (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_BATCH_SUPPORT, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.6");
	switch (rgbInfoValue) {
	 case SQL_BS_SELECT_EXPLICIT	: lstrcpy(buffer,"SQL_BS_SELECT_EXPLICIT"); break;
	 case SQL_BS_ROW_COUNT_EXPLICIT : lstrcpy(buffer,"SQL_BS_ROW_COUNT_EXPLICIT"); break;
	 case SQL_BS_SELECT_PROC	: lstrcpy(buffer,"SQL_BS_SELECT_PROC"); break;
	 case SQL_BS_ROW_COUNT_PROC 	: lstrcpy(buffer,"SQL_BS_ROW_COUNT_PROC"); break;
	 default                : lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 7.SQL_CATALOG_NAME !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.7.SQL_CATALOG_NAME (ODBC 3.0): \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CATALOG_NAME, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.7");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 8.SQL_COLLATION_SEQ */
	szLogPrintf(lpSrvr, FALSE,"\t8.8.SQL_COLLATION_SEQ (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_COLLATION_SEQ, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.8");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 9.SQL_CREATE_ASSERTION   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.9.SQL_CREATE_ASSERTION (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_ASSERTION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.9");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CA_CONSTRAINT_INITIALLY_DEFERRED )  lstrcat(buffer,"SQL_CA_CONSTRAINT_INITIALLY_DEFERRED |");
	if (rgbInfoValue & SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE ) lstrcat(buffer,"SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE |");
	if (rgbInfoValue & SQL_CA_CONSTRAINT_DEFERRABLE )  	   lstrcat(buffer,"SQL_CA_CONSTRAINT_DEFERRABLE |");
	if (rgbInfoValue & SQL_CA_CONSTRAINT_NON_DEFERRABLE )  	   lstrcat(buffer,"SQL_CA_CONSTRAINT_NON_DEFERRABLE |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 10.SQL_DROP_ASSERTION   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.10.SQL_DROP_ASSERTION (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_ASSERTION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.10");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DA_DROP_ASSERTION )  lstrcat(buffer,"SQL_DA_DROP_ASSERTION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 11.SQL_CREATE_CHARACTER_SET   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.11.SQL_CREATE_CHARACTER_SET (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_CHARACTER_SET, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.11");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CCS_CREATE_CHARACTER_SET )  lstrcat(buffer,"SQL_CCS_CREATE_CHARACTER_SET |");
	if (rgbInfoValue & SQL_CCS_COLLATE_CLAUSE )	   lstrcat(buffer,"SQL_CCS_COLLATE_CLAUSE |");
	if (rgbInfoValue & SQL_CCS_LIMITED_COLLATION )	   lstrcat(buffer,"SQL_CCS_LIMITED_COLLATION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 12.SQL_DROP_CHARACTER_SET   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.12.SQL_DROP_CHARACTER_SET (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_CHARACTER_SET, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.12");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DCS_DROP_CHARACTER_SET )  lstrcat(buffer,"SQL_DCS_DROP_CHARACTER_SET |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 13.SQL_CREATE_COLLATION   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.13.SQL_CREATE_COLLATION (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_COLLATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.13");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CCOL_CREATE_COLLATION )  lstrcat(buffer,"SQL_CCOL_CREATE_COLLATION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 14.SQL_DROP_COLLATION   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.14.SQL_DROP_COLLATION (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_COLLATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.14");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DC_DROP_COLLATION )  lstrcat(buffer,"SQL_DC_DROP_COLLATION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 15.SQL_CREATE_DOMAIN   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.15.SQL_CREATE_DOMAIN (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_DOMAIN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.15");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CDO_CREATE_DOMAIN )  lstrcat(buffer,"SQL_CDO_CREATE_DOMAIN |");
	if (rgbInfoValue & SQL_CDO_CONSTRAINT_NAME_DEFINITION )  lstrcat(buffer,"SQL_CDO_CONSTRAINT_NAME_DEFINITION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 16.SQL_DROP_DOMAIN   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.16.SQL_DROP_DOMAIN (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_DOMAIN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.16");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DD_DROP_DOMAIN )  lstrcat(buffer,"SQL_DD_DROP_DOMAIN |");
	if (rgbInfoValue & SQL_DD_CASCADE )  lstrcat(buffer,"SQL_DD_CASCADE |");
	if (rgbInfoValue & SQL_DD_RESTRICT)  lstrcat(buffer,"SQL_DD_RESTRICT |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 17.SQL_CREATE_SCHEMA   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.17.SQL_CREATE_SCHEMA (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_SCHEMA, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.17");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CS_CREATE_SCHEMA)  	 lstrcat(buffer,"SQL_CS_CREATE_SCHEMA |");
	if (rgbInfoValue & SQL_CS_AUTHORIZATION) 	 lstrcat(buffer,"SQL_CS_AUTHORIZATION |");
	if (rgbInfoValue & SQL_CS_DEFAULT_CHARACTER_SET) lstrcat(buffer,"SQL_CS_DEFAULT_CHARACTER_SET |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 18.SQL_DROP_SCHEMA   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.18.SQL_DROP_SCHEMA (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_SCHEMA, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.18");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DS_DROP_SCHEMA)  lstrcat(buffer,"SQL_DS_DROP_SCHEMA |");
	if (rgbInfoValue & SQL_DS_CASCADE) 	lstrcat(buffer,"SQL_DS_CASCADE |");
	if (rgbInfoValue & SQL_DS_RESTRICT)	lstrcat(buffer,"SQL_DS_RESTRICT |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 19.SQL_CREATE_TABLE   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.19.SQL_CREATE_TABLE (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_TABLE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.19");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CT_CREATE_TABLE)     		lstrcat(buffer,"SQL_CT_CREATE_TABLE |");
	if (rgbInfoValue & SQL_CT_TABLE_CONSTRAINT) 		lstrcat(buffer,"SQL_CT_TABLE_CONSTRAINT |");
	if (rgbInfoValue & SQL_CT_CONSTRAINT_NAME_DEFINITION) 	lstrcat(buffer,"SQL_CT_CONSTRAINT_NAME_DEFINITION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 20.   SQL_DROP_TABLE   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.20.SQL_DROP_TABLE (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_TABLE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.20");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DT_DROP_TABLE)   lstrcat(buffer,"SQL_DT_DROP_TABLE |");
	if (rgbInfoValue & SQL_DT_CASCADE) 	lstrcat(buffer,"SQL_DT_CASCADE |");
	if (rgbInfoValue & SQL_DT_RESTRICT)	lstrcat(buffer,"SQL_DT_RESTRICT |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 21.SQL_CREATE_TRANSLATION   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.21.SQL_CREATE_TRANSLATION (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_TRANSLATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.21");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CTR_CREATE_TRANSLATION) lstrcat(buffer,"SQL_CTR_CREATE_TRANSLATION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 22.SQL_DROP_TRANSLATION   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.22.SQL_DROP_TRANSLATION (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_TRANSLATION, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.22");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DTR_DROP_TRANSLATION) lstrcat(buffer,"SQL_DTR_DROP_TRANSLATION |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 23.SQL_CREATE_VIEW   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.23.SQL_CREATE_VIEW (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_CREATE_VIEW, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.23");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_CV_CREATE_VIEW)  lstrcat(buffer,"SQL_CV_CREATE_VIEW |");
	if (rgbInfoValue & SQL_CV_CHECK_OPTION) lstrcat(buffer,"SQL_CV_CHECK_OPTION |");
	if (rgbInfoValue & SQL_CV_CASCADED)     lstrcat(buffer,"SQL_CV_CASCADED |");
	if (rgbInfoValue & SQL_CV_LOCAL)  	 lstrcat(buffer,"SQL_CV_LOCAL |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 24.SQL_DROP_VIEW   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.24.SQL_DROP_VIEW (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DROP_VIEW, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.24");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DV_DROP_VIEW)  	lstrcat(buffer,"SQL_DV_DROP_VIEW |");
	if (rgbInfoValue & SQL_DV_CASCADE) 	lstrcat(buffer,"SQL_DV_CASCADE |");
	if (rgbInfoValue & SQL_DV_RESTRICT)     lstrcat(buffer,"SQL_DV_RESTRICT |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 25.SQL_CURSOR_SENSITIVITY   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.25.SQL_CURSOR_SENSITIVITY (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_CURSOR_SENSITIVITY, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.25");
	switch (rgbInfoValue) {
	 case SQL_INSENSITIVE	: lstrcpy(buffer,"SQL_UNSENSITIVE"); break;
	 case SQL_UNSPECIFIED   : lstrcpy(buffer,"SQL_UNSPECIFIED"); break;
	 case SQL_SENSITIVE	: lstrcpy(buffer,"SQL_SENSITIVE"); break;
	 default                : lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 26.SQL_DATETIME_LITERALS   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.26.SQL_DATETIME_LITERALS (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DATETIME_LITERALS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.26");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_DL_SQL92_DATE)  			   lstrcat(buffer,"SQL_DL_SQL92_DATE |");
	if (rgbInfoValue & SQL_DL_SQL92_TIME)  			   lstrcat(buffer,"SQL_DL_SQL92_TIME |");
	if (rgbInfoValue & SQL_DL_SQL92_TIMESTAMP)  		   lstrcat(buffer,"SQL_DL_SQL92_TIMESTAMP |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_YEAR)		   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_YEAR |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_MONTH) 	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_MONTH |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_DAY)  		   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_DAY |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_HOUR)  	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_HOUR |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_MINUTE)  	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_MINUTE |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_SECOND)  	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_SECOND |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH)	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR)	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE)	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND)	   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE)   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND)   lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND |");
	if (rgbInfoValue & SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND) lstrcat(buffer,"SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 27.SQL_DDL_INDEX   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.27.SQL_DDL_INDEX (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DDL_INDEX, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.27");
	switch (rgbInfoValue) {
	 case SQL_DI_CREATE_INDEX : lstrcpy(buffer,"SQL_DI_CREATE_INDEX"); break;
	 case SQL_DI_DROP_INDEX   : lstrcpy(buffer,"SQL_DI_DROP_INDEX"); break;
	 default                  : lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 28.SQL_DESCRIBE_PARAMETER */
	szLogPrintf(lpSrvr, FALSE,"\t8.28.SQL_DESCRIBE_PARAMETER (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_DESCRIBE_PARAMETER, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.28");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 29.SQL_DM_VER */
	szLogPrintf(lpSrvr, FALSE,"\t8.29.SQL_DM_VER (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_DM_VER, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.29");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

	/* *** 30. SQL_DRIVER_HDESC !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.30.SQL_DRIVER_HDESC : \r\n");
	cbInfoValueMax = sizeof(SDWORD);
#ifdef WIN32
	rc = SQLGetStmtAttr(*phstmt,SQL_ATTR_APP_PARAM_DESC, &hdesc, SQL_IS_INTEGER, NULL);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetStmtAttr");

	memcpy(rgbInfoValueHandle, &hdesc, sizeof(SQLHDESC));
	cbInfoValueMax= sizeof(ULONG);
#endif
	rc = SQLGetInfo(*phdbc, SQL_DRIVER_HDESC, (PTR) rgbInfoValueHandle,
				cbInfoValueMax, NULL);
	if (rc != SQL_SUCCESS) rc--;
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.30");
	sprintf(buffer,"%p", *rgbInfoValueHandle);
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 31.SQL_INDEX_KEYWORDS   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.31.SQL_INDEX_KEYWORDS (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_INDEX_KEYWORDS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.31");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_IK_ASC)  lstrcat(buffer,"SQL_IK_ASC |");
	if (rgbInfoValue & SQL_IK_DESC) lstrcat(buffer,"SQL_IK_DESC |");
	if (rgbInfoValue & SQL_IK_ALL)  lstrcat(buffer,"SQL_IK_ALL |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 32.SQL_INFO_SCHEMA_VIEWS   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.32.SQL_INFO_SCHEMA_VIEWS (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_INFO_SCHEMA_VIEWS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.32");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_ISV_ASSERTIONS)  	lstrcat(buffer,"SQL_ISV_ASSERTIONS |");
	if (rgbInfoValue & SQL_ISV_CHARACTER_SETS) 	lstrcat(buffer,"SQL_ISV_CHARACTER_SETS |");
	if (rgbInfoValue & SQL_ISV_CHECK_CONSTRAINTS) 	lstrcat(buffer,"SQL_ISV_CHECK_CONSTRAINTS |");
	if (rgbInfoValue & SQL_ISV_COLLATIONS)  	lstrcat(buffer,"SQL_ISV_COLLATIONS |");
	if (rgbInfoValue & SQL_ISV_COLUMN_DOMAIN_USAGE) lstrcat(buffer,"SQL_ISV_COLUMN_DOMAIN_USAGE |");
	if (rgbInfoValue & SQL_ISV_COLUMN_PRIVILEGES)   lstrcat(buffer,"SQL_ISV_COLUMN_PRIVILEGES |");
	if (rgbInfoValue & SQL_ISV_COLUMNS) 		lstrcat(buffer,"SQL_ISV_COLUMNS |");
	if (rgbInfoValue & SQL_ISV_CONSTRAINT_COLUMN_USAGE) lstrcat(buffer,"SQL_ISV_CONSTRAINT_COLUMN_USAGE |");
	if (rgbInfoValue & SQL_ISV_CONSTRAINT_TABLE_USAGE)  lstrcat(buffer,"SQL_ISV_CONSTRAINT_TABLE_USAGE |");
	if (rgbInfoValue & SQL_ISV_DOMAIN_CONSTRAINTS)	lstrcat(buffer,"SQL_ISV_DOMAIN_CONSTRAINTS |");
	if (rgbInfoValue & SQL_ISV_DOMAINS)		lstrcat(buffer,"SQL_ISV_DOMAINS |");
	if (rgbInfoValue & SQL_ISV_KEY_COLUMN_USAGE)	lstrcat(buffer,"SQL_ISV_KEY_COLUMN_USAGE |");
	if (rgbInfoValue & SQL_ISV_REFERENTIAL_CONSTRAINTS) lstrcat(buffer,"SQL_ISV_REFERENTIAL_CONSTRAINTS |");
	if (rgbInfoValue & SQL_ISV_SCHEMATA)		lstrcat(buffer,"SQL_ISV_SCHEMATA |");
	if (rgbInfoValue & SQL_ISV_SQL_LANGUAGES)	lstrcat(buffer,"SQL_ISV_SQL_LANGUAGES |");
	if (rgbInfoValue & SQL_ISV_TABLE_CONSTRAINTS)   lstrcat(buffer,"SQL_ISV_TABLE_CONSTRAINTS |");
	if (rgbInfoValue & SQL_ISV_TABLE_PRIVILEGES)    lstrcat(buffer,"SQL_ISV_TABLE_PRIVILEGES |");
	if (rgbInfoValue & SQL_ISV_TABLES)		lstrcat(buffer,"SQL_ISV_TABLES |");
	if (rgbInfoValue & SQL_ISV_TRANSLATIONS)	lstrcat(buffer,"SQL_ISV_TRANSLATIONS |");
	if (rgbInfoValue & SQL_ISV_USAGE_PRIVILEGES)    lstrcat(buffer,"SQL_ISV_USAGE_PRIVILEGES |");
	if (rgbInfoValue & SQL_ISV_VIEW_COLUMN_USAGE)   lstrcat(buffer,"SQL_ISV_VIEW_COLUMN_USAGE |");
	if (rgbInfoValue & SQL_ISV_VIEW_TABLE_USAGE)    lstrcat(buffer,"SQL_ISV_VIEW_TABLE_USAGE |");
	if (rgbInfoValue & SQL_ISV_VIEWS)		lstrcat(buffer,"SQL_ISV_VIEWS |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 33.SQL_INSERT_STATEMENT   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.33.SQL_INSERT_STATEMENT (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_INSERT_STATEMENT, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.33");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_IS_INSERT_LITERALS) lstrcat(buffer,"SQL_IS_INSERT_LITERALS |");
	if (rgbInfoValue & SQL_IS_INSERT_SEARCHED) lstrcat(buffer,"SQL_IS_INSERT_SEARCHED |");
	if (rgbInfoValue & SQL_IS_SELECT_INTO)     lstrcat(buffer,"SQL_IS_SELECT_INTO |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);


	/* *** 34.SQL_MAX_ASYNC_CONCURRENT_STATEMENTS (ODBC 3.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.34.SQL_MAX_ASYNC_CONCURRENT_STATEMENTS (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,    SQL_MAX_ASYNC_CONCURRENT_STATEMENTS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.34");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 35.SQL_MAX_IDENTIFIER_LEN (ODBC 3.0) !*/
	szLogPrintf(lpSrvr, FALSE,"\t8.35.SQL_MAX_IDENTIFIER_LEN (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_MAX_IDENTIFIER_LEN, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.35");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%d'\r\n",rgbInfoValue);

	/* *** 36.SQL_ODBC_INTERFACE_CONFORMANCE   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.36.SQL_ODBC_INTERFACE_CONFORMANCE (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_ODBC_INTERFACE_CONFORMANCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.36");
	switch (rgbInfoValue) {
	 case SQL_OIC_CORE   : lstrcpy(buffer,"SQL_OIC_CORE"); break;
	 case SQL_OIC_LEVEL1 : lstrcpy(buffer,"SQL_OIC_LEVEL1"); break;
	 case SQL_OIC_LEVEL2 : lstrcpy(buffer,"SQL_OIC_LEVEL2"); break;
	 default                  : lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 37.SQL_OJ_CAPABILITIES   (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.37.SQL_OJ_CAPABILITIES (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_OJ_CAPABILITIES, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.37");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_OJ_LEFT)  	lstrcat(buffer,"SQL_OJ_LEFT |");
	if (rgbInfoValue & SQL_OJ_RIGHT) 	lstrcat(buffer,"SQL_OJ_RIGHT |");
	if (rgbInfoValue & SQL_OJ_FULL)  	lstrcat(buffer,"SQL_OJ_FULL |");
	if (rgbInfoValue & SQL_OJ_NOT_ORDERED)  lstrcat(buffer,"SQL_OJ_NOT_ORDERED |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 38.SQL_PARAM_ARRAY_ROW_COUNTS  (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.38.SQL_PARAM_ARRAY_ROW_COUNTS (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_PARAM_ARRAY_ROW_COUNTS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.38");
	switch (rgbInfoValue) {
	 case SQL_PARC_BATCH 	: lstrcpy(buffer,"SQL_PARC_BATCH"); break;
	 case SQL_PARC_NO_BATCH : lstrcpy(buffer,"SQL_PARC_NO_BATCH"); break;
	 default              	: lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 39.SQL_PARAM_ARRAY_SELECTS  (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.39.SQL_PARAM_ARRAY_SELECTS (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,SQL_PARAM_ARRAY_SELECTS, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.39");
	switch (rgbInfoValue) {
	 case SQL_PAS_BATCH 	: lstrcpy(buffer,"SQL_PAS_BATCH"); break;
	 case SQL_PAS_NO_BATCH  : lstrcpy(buffer,"SQL_PAS_NO_BATCH"); break;
	 case SQL_PAS_NO_SELECT : lstrcpy(buffer,"SQL_PAS_NO_SELECT"); break;
	 default              	: lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);


	/* *** 40.   SQL_SQL_CONFORMANCE  (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.40.   SQL_SQL_CONFORMANCE (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc,   SQL_SQL_CONFORMANCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.40");
	switch (rgbInfoValue) {
	 case SQL_SC_SQL92_ENTRY: 		lstrcpy(buffer,"SQL_SC_SQL92_ENTRY"); break;
	 case SQL_SC_FIPS127_2_TRANSITIONAL: 	lstrcpy(buffer,"SQL_SC_FIPS127_2_TRANSITIONAL"); break;
	 case SQL_SC_SQL92_FULL: 		lstrcpy(buffer,"SQL_SC_SQL92_FULL"); break;
	 case SQL_SC_SQL92_INTERMEDIATE:	lstrcpy(buffer,"SQL_SC_SQL92_INTERMEDIATE:"); break;
	 default              	: lstrcpy(buffer,"???");
	}
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",buffer);

	/* *** 41.SQL_STANDARD_CLI_CONFORMANCE  (ODBC 3.0) */
	szLogPrintf(lpSrvr, FALSE,"\t8.41.SQL_STANDARD_CLI_CONFORMANCE (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_STANDARD_CLI_CONFORMANCE, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.41");
	lstrcpy(buffer," ");
	if (rgbInfoValue & SQL_SCC_XOPEN_CLI_VERSION1) 	lstrcat(buffer,"SQL_SCC_XOPEN_CLI_VERSION1 |");
	if (rgbInfoValue & SQL_SCC_ISO92_CLI)  		lstrcat(buffer,"SQL_SCC_ISO92_CLI |");
	sprintf(bufxx, "\tValue = ");
	PrintString(buffer, lstrlen(buffer), bufxx, lpSrvr);

	/* *** 42.SQL_XOPEN_CLI_YEAR */
	szLogPrintf(lpSrvr, FALSE,"\t8.42.SQL_XOPEN_CLI_YEAR (ODBC 3.0) : \r\n");
	rc = SQLGetInfo(*phdbc, SQL_XOPEN_CLI_YEAR, (PTR) &rgbInfoValueChar,
				sizeof(rgbInfoValueChar), NULL);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetInfo 8.42");
	szLogPrintf(lpSrvr, FALSE,"\tValue = '%s'\r\n",rgbInfoValueChar);

/*
   SQL_SQL92_DATETIME_FUNCTIONS
   SQL_SQL92_FOREIGN_KEY_DELETE_RULE
   SQL_SQL92_FOREIGN_KEY_UPDATE_RULE
   SQL_SQL92_GRANT
   SQL_SQL92_NUMERIC_VALUE_FUNCTIONS
   SQL_SQL92_PREDICATES
   SQL_SQL92_RELATIONAL_JOIN_OPERATORS
   SQL_SQL92_REVOKE
   SQL_SQL92_ROW_VALUE_CONSTRUCTOR
   SQL_SQL92_STRING_FUNCTIONS
   SQL_SQL92_VALUE_EXPRESSIONS

   SQL_DYNAMIC_CURSOR_ATTRIBUTES1	 	SQLUINTEGER
   SQL_DYNAMIC_CURSOR_ATTRIBUTES2	 	SQLUINTEGER
   SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1 		SQLUINTEGER
   SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2		SQLUINTEGER
   SQL_KEYSET_CURSOR_ATTRIBUTES1		SQLUINTEGER
   SQL_KEYSET_CURSOR_ATTRIBUTES2		SQLUINTEGER
   SQL_STATIC_CURSOR_ATTRIBUTES1		SQLUINTEGER
   SQL_STATIC_CURSOR_ATTRIBUTES2		SQLUINTEGER
*/
DoAbort:
	return(0);
#endif
}
/*
 --------------------------------------------------------------------------------
| CheckConversion:
|
|
| Returns:
|
 --------------------------------------------------------------------------------
*/
RETCODE CheckConversion(UDWORD rgbInfoValue, UCHAR buffer[MAX_NAME_LEN])
{
 lstrcpy(buffer," ");
 if (rgbInfoValue &  SQL_CVT_BIGINT) lstrcat(buffer,"SQL_CVT_BIGINT");
 if (rgbInfoValue & SQL_CVT_BINARY) lstrcat(buffer,"SQL_CVT_BINARY");
 if (rgbInfoValue & SQL_CVT_BIT) lstrcat(buffer,"SQL_CVT_BIT");
 if (rgbInfoValue & SQL_CVT_CHAR) lstrcat(buffer,"SQL_CVT_CHAR");
 if (rgbInfoValue & SQL_CVT_DATE) lstrcat(buffer,"SQL_CVT_DATE");
 if (rgbInfoValue & SQL_CVT_DECIMAL) lstrcat(buffer,"SQL_CVT_DECIMAL");
 if (rgbInfoValue & SQL_CVT_DOUBLE) lstrcat(buffer,"SQL_CVT_DOUBLE");
 if (rgbInfoValue & SQL_CVT_FLOAT) lstrcat(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_INTEGER) lstrcat(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_LONGVARBINARY) lstrcat(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_LONGVARCHAR) lstrcat(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_NUMERIC) lstrcat(buffer,"SQL_CVT_NUMERIC");
 if (rgbInfoValue & SQL_CVT_REAL) lstrcat(buffer,"SQL_CVT_REAL");
 if (rgbInfoValue & SQL_CVT_SMALLINT) lstrcat(buffer,"SQL_CVT_SMALLINT");
 if (rgbInfoValue & SQL_CVT_TIME) lstrcat(buffer,"SQL_CVT_TIME");
 if (rgbInfoValue & SQL_CVT_TIMESTAMP) lstrcat(buffer,"SQL_CVT_TIMESTAMP");
 if (rgbInfoValue & SQL_CVT_TINYINT) lstrcat(buffer,"SQL_CVT_TINYINT");
 if (rgbInfoValue & SQL_CVT_VARBINARY) lstrcat(buffer,"SQL_CVT_VARBINARY");
 if (rgbInfoValue & SQL_CVT_VARCHAR) lstrcat(buffer,"SQL_CVT_VARCHAR");

 return(0);
}
/* ************************************************************************* */

/* ------------------------------------------------------------------------- */
/* Information Types - supported by SQLGetInfo */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* ***** Driver Information */
/* ------------------------------------------------------------------------- */
/*
   SQL_ACTIVE_CONNECTIONS
   SQL_ACTIVE_STATEMENTS
   SQL_DATA_SOURCE_NAME
   SQL_DRIVER_HDBC
   SQL_DRIVER_HENV
   SQL_DRIVER_HLIB                              (ODBC 2.0)
   SQL_DRIVER_HSTMT
   SQL_DRIVER_NAME
   SQL_DRIVER_ODBC_VER                          (ODBC 2.0)
   SQL_DRIVER_VER
   SQL_FETCH_DIRECTION
   SQL_FILE_USAGE                               (ODBC 2.0)
   SQL_GETDATA_EXTENSIONS                       (ODBC 2.0)
   SQL_LOCK_TYPES                               (ODBC 2.0)
   SQL_ODBC_API_CONFORMANCE
   SQL_ODBC_SAG_CLI_CONFORMANCE
   SQL_ODBC_VER
   SQL_POS_OPERATIONS                           (ODBC 2.0)
   SQL_ROW_UPDATES
   SQL_SEARCH_PATTERN_ESCAPE
   SQL_SERVER_NAME
*/
/* ------------------------------------------------------------------------- */
/* ***** DBMS Product Information */
/* ------------------------------------------------------------------------- */
/*
   SQL_DATABASE_NAME
   SQL_DBMS_NAME
   SQL_DBMS_VER
*/
/* ------------------------------------------------------------------------- */
/* ***** Data Source Information */
/* ------------------------------------------------------------------------- */
/*
   SQL_ACCESSIBLE_PROCEDURES
   SQL_ACCESSIBLE_TABLES
   SQL_BOOKMARK_PERSISTENCE                     (ODBC 2.0)
   SQL_COLUMN_ALIAS                             (ODBC 2.0)
   SQL_CONCAT_NULL_BEHAVIOR
   SQL_CURCOR_COMMIT_BEHAVIOR
   SQL_CURSOR_ROLLBACK_BEHAVIOR
   SQL_DATA_SOURCE_READ_ONLY
   SQL_DEFAULT_TXN_ISOLATION
   SQL_MULT_RESULT_SET
   SQL_MULTIPLE_ACTIVE_TXN
   SQL_NEED_LONG_DATA_LEN                       (ODBC 2.0)
   SQL_NULL_COLLATION                           (ODBC 2.0)
   SQL_OWNER_TERM
   SQL_PROCEDURE_TERM
   SQL_QUALIFIER_TERM
   SQL_SCROLL_CONCURRENCY
   SQL_SCROLL_OPTIONS
   SQL_STATIC_SENSITIVITY                       (ODBC 2.0)
   SQL_TABLE_TERM
   SQL_TXN_CAPABLE
   SQL_TXN_ISOLATION_OPTION
   SQL_USER_NAME
*/
/* ------------------------------------------------------------------------- */
/* ***** Supported SQL */
/* ------------------------------------------------------------------------- */
/*
   SQL_ALTER_TABLE                              (ODBC 2.0)
   SQL_COLUMN_ALIAS
   SQL_CORRELATION_NAME
   SQL_EXPRESSIONS_IN_ORDERBY
   SQL_GROUP_BY                                 (ODBC 2.0)
   SQL_IDENTIFIER_CASE
   SQL_IDENTIFIER_QUOTE_CHAR
   SQL_KEYWORDS                                 (ODBC 2.0)
   SQL_LIKE_ESCAPE_CLAUSE                       (ODBC 2.0)
   SQL_NON_NULLABLE_COLUMNS
   SQL_ODBC_SQL_CONFORMANCE
   SQL_ODBC_SQL_OPT_IEF
   SQL_ORDER_BY_COLUMNS_IN_SELECT               (ODBC 2.0)
   SQL_OUTER_JOINS
   SQL_OWNER_USAGE                              (ODBC 2.0)
   SQL_POSITIONED_STATEMENTS                    (ODBC 2.0)
   SQL_PROCEDURES
   SQL_QUALIFIER_LOCATION                       (ODBC 2.0)
   SQL_QUALIFIER_NAME_SEPARATOR
   SQL_QUALIFIER_USAGE                          (ODBC 2.0)
   SQL_QUALIFIER_IDENTIFIER_CASE                (ODBC 2.0)
   SQL_SPECIAL_CHARACTERS                       (ODBC 2.0)
   SQL_SUBQUERIES                               (ODBC 2.0)
   SQL_UNION                                    (ODBC 2.0)
*/
/* ------------------------------------------------------------------------- */
/* ***** SQL Limits */
/* ------------------------------------------------------------------------- */
/*
   SQL_MAX_BINARY_LITERAL_LEN                   (ODBC 2.0)
   SQL_MAX_CHAR_LITERAL_LEN                     (ODBC 2.0)
   SQL_MAX_COLUMN_NAME_LEN
   SQL_MAX_COLUMNS_IN_GROUP_BY                  (ODBC 2.0)
   SQL_MAX_COLUMNS_ORDER_BY                     (ODBC 2.0)
   SQL_MAX_COLUMNS_IN_INDEX                     (ODBC 2.0)
   SQL_MAX_COLUMNS_IN_SELECT                    (ODBC 2.0)
   SQL_MAX_COLUMNS_IN_TABLE                     (ODBC 2.0)
   SQL_MAX_CURSOR_NAME_LEN
   SQL_MAX_INDEX_SIZE
   SQL_MAX_OWNER_NAME_LEN
   SQL_MAX_PROCEDURE_NAME_LEN
   SQL_MAX_QUALIFIER_NAME_LEN
   SQL_MAX_ROW_SIZE                             (ODBC 2.0)
   SQL_MAX_ROW_SIZE_INCLUDES_LONG               (ODBC 2.0)
   SQL_MAX_STATEMENT_LEN                        (OBDC 2.0)
   SQL_MAX_TABLE_NAME_LEN
   SQL_MAX_TABLES_IN_SELECT                     (ODBC 2.0)
   SQL_MAX_USER_NAME_LEN                        (ODBC 2.0)
*/
/* ------------------------------------------------------------------------- */
/* ***** Scalar Function Information */
/* ------------------------------------------------------------------------- */
/*
   SQL_CONVERT_FUNCTIONS
   SQL_NUMERIC_FUNCTIONS
   SQL_STRING_FUNCTIONS
   SQL_SYSTEM_FUNCTIONS
   SQL_TIMEDATE_ADD_INTERVALS                   (ODBC 2.0)
   SQL_TIMEDATE_DIFF_INTERVALS                  (ODBC 2.0)
   SQL_TIMEDATE_FUNCTIONS
*/
/* ------------------------------------------------------------------------- */
/* ***** Conversion Information */
/* ------------------------------------------------------------------------- */
/*
   SQL_CONVERT_BIGINT
   SQL_CONVERT_BINARY
   SQL_CONVERT_BIT
   SQL_CONVERT_CHAR
   SQL_CONVERT_DATE
   SQL_CONVERT_DECIMAL
   SQL_CONVERT_DOUBLE
   SQL_CONVERT_FLOAT
   SQL_CONVERT_INTEGER
   SQL_CONVERT_LONGVARBINARY
   SQL_CONVERT_LONGVARCHAR
   SQL_CONVERT_NUMERIC
   SQL_CONVERT_REAL
   SQL_CONVERT_SMALLINT
   SQL_CONVERT_TIME
   SQL_CONVERT_TIMESTAMP
   SQL_CONVERT_TINYINT
   SQL_CONVERT_VARBINARY
   SQL_CONVERT_VARCHAR
*/
/* ------- *** Options ODBC 3.0 *** ----------------------------------------

   SQL_ACTIVE_ENVIRONMENTSR
   SQL_AGGREGATE_FUNCTIONS
   SQL_ALTER_DOMAIN
   SQL_ASYNC_MODE
   SQL_BATCH_ROW_COUNT
   SQL_BATCH_SUPPORT
   SQL_CATALOG_NAME
   SQL_COLLATION_SEQ
   SQL_CREATE_ASSERTION
   SQL_DROP_ASSERTION
   SQL_CREATE_CHARACTER_SET
   SQL_DROP_CHARACTER_SET
   SQL_CREATE_COLLATION
   SQL_DROP_COLLATION
   SQL_CREATE_DOMAIN
   SQL_DROP_DOMAIN
   SQL_CREATE_SCHEMA
   SQL_DROP_SCHEMA
   SQL_CREATE_TABLE
   SQL_DROP_TABLE
   SQL_CREATE_TRANSLATION
   SQL_DROP_TRANSLATION
   SQL_CREATE_VIEW
   SQL_DROP_VIEW
   SQL_CURSOR_SENSITIVITY
   SQL_DATETIME_LITERALS
   SQL_DDL_INDEX
   SQL_DESCRIBE_PARAMETER
   SQL_DM_VER
   SQL_DRIVER_HDESC
   SQL_INDEX_KEYWORDS
   SQL_INFO_SCHEMA_VIEWS
   SQL_INSERT_STATEMENT
   SQL_MAX_ASYNC_CONCURRENT_STATEMENTS
   SQL_MAX_IDENTIFIER_LEN
   SQL_ODBC_INTERFACE_CONFORMANCE
   SQL_OJ_CAPABILITIES  			SQLCHAR
   SQL_PARAM_ARRAY_ROW_COUNTS
   SQL_PARAM_ARRAY_SELECTS
   SQL_SQL_CONFORMANCE
   SQL_STANDARD_CLI_CONFORMANCE
   SQL_XOPEN_CLI_YEAR

   SQL_SQL92_DATETIME_FUNCTIONS
   SQL_SQL92_FOREIGN_KEY_DELETE_RULE
   SQL_SQL92_FOREIGN_KEY_UPDATE_RULE
   SQL_SQL92_GRANT
   SQL_SQL92_NUMERIC_VALUE_FUNCTIONS
   SQL_SQL92_PREDICATES
   SQL_SQL92_RELATIONAL_JOIN_OPERATORS
   SQL_SQL92_REVOKE
   SQL_SQL92_ROW_VALUE_CONSTRUCTOR
   SQL_SQL92_STRING_FUNCTIONS
   SQL_SQL92_VALUE_EXPRESSIONS

   SQL_DYNAMIC_CURSOR_ATTRIBUTES1	 	SQLUINTEGER
   SQL_DYNAMIC_CURSOR_ATTRIBUTES2	 	SQLUINTEGER
   SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1 		SQLUINTEGER
   SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2		SQLUINTEGER
   SQL_KEYSET_CURSOR_ATTRIBUTES1		SQLUINTEGER
   SQL_KEYSET_CURSOR_ATTRIBUTES2		SQLUINTEGER
   SQL_STATIC_CURSOR_ATTRIBUTES1		SQLUINTEGER
   SQL_STATIC_CURSOR_ATTRIBUTES2		SQLUINTEGER
 ************************************************************************* */

