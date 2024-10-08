/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDESCP.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLDescribeParam
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLDescribeParam"


#define TABLE "DESCRIBEPARAM"
#define SQLSTMT1 "SELECT * FROM DUAL"
#define MAX_NAME_LEN 50

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLDescribeParam:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLDescribeParam(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;

	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	UCHAR                   typ_char[MAX_NAME_LEN];
	SDWORD                  typ_int;
	SWORD                   typ_smallint;

	SDWORD                  cbChar = SQL_NTS;
	SDWORD                  cbInt  = SQL_NTS;
	SDWORD                  cbSmallint  = SQL_NTS;

	UWORD			ipar;
	SWORD			pfSqlType;
	UDWORD			pcbColDef;
	SWORD			pibScale;
	SWORD			pfNullable;

/* ------------------------------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	/* - Connect ------------------------------------------------------- */
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

	/* ----------------------------------------------------------------- */

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (ID INTEGER, NAME CHAR(80), AGE SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname,TABLE);
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," VALUES (?, ?, ?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt= '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
			SQL_INTEGER, 0, 0, &typ_int, sizeof(typ_int), &cbInt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
		SQL_CHAR, MAX_NAME_LEN, 0, &typ_char, 0, &cbChar);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_SMALLINT, 0, 0, &typ_smallint, sizeof(typ_smallint), &cbSmallint);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

	typ_int = 9999;
	lstrcpy(typ_char,"Test DescribeParam");
	typ_smallint = 10;

	rc = SQLExecute(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecute");

	/* --- DescribeParam ------------------------------------------ */
	/* Column 1. */
	ipar = 1;
	rc = SQLDescribeParam(*phstmt, ipar, &pfSqlType, &pcbColDef, &pibScale, &pfNullable);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDescribeParam");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 1. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\tipar	: '%d'\r\n",ipar);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef	: '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale	: '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);

	/* Column 2. */
	ipar = 2;
	rc = SQLDescribeParam(*phstmt, ipar, &pfSqlType, &pcbColDef, &pibScale, &pfNullable);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDescribeParam");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 2. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\tipar       : '%d'\r\n",ipar);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef  : '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale   : '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);

	/* Column 3. */
	ipar = 3;
	rc = SQLDescribeParam(*phstmt, ipar, &pfSqlType, &pcbColDef, &pibScale, &pfNullable);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDescribeParam");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 3. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\tipar       : '%d'\r\n",ipar);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef  : '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale   : '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* - Disconnect ---------------------------------------------------- */
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
