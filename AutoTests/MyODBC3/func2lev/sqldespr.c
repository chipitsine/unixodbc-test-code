/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDESPR.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLDESCRIBEPARAM
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLDescribeParam"
#define TABLE "DESCRIBEPARAM"
#define MAX_NAME_LEN 50

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLDescribeParam:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLDescribeParam(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
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

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (ID INTEGER, NAME CHAR(80), AGE SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname,TABLE);
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," VALUES (?, ?, ?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt= '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
			SQL_INTEGER, 0, 0, &typ_int, sizeof(typ_int), &cbInt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
		SQL_CHAR, MAX_NAME_LEN, 0, &typ_char, 0, &cbChar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_SMALLINT, 0, 0, &typ_smallint, sizeof(typ_smallint), &cbSmallint);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	typ_int = 9999;
	lstrcpy(typ_char,"Test DescribeParam");
	typ_smallint = 10;

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	/* --- DescribeParam ------------------------------------------ */
	/* Column 1. */
	ipar = 1;
	rc = SQLDescribeParam(*phstmt, ipar, &pfSqlType, &pcbColDef, &pibScale,
				&pfNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeParam");

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
	rc = SQLDescribeParam(*phstmt, ipar, &pfSqlType, &pcbColDef, &pibScale,
				&pfNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeParam");

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
	rc = SQLDescribeParam(*phstmt, ipar, &pfSqlType, &pcbColDef, &pibScale,
				&pfNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeParam");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 3. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\tipar       : '%d'\r\n",ipar);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef  : '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale   : '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);
	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	lstrcpy(tabname, TABLE);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif
/* ---------------------------------------------------------------------har- */
	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisConnect");

	rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------ */
