/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDESCO.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLDESCRIBECOL
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funccore.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLDescribeCol"
#define TABLE "DESCRIBECOL"
#define MAX_NAME_LEN 50

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLDescribeCol:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLDescribeCol(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	UWORD			icol;
	UCHAR                   szColName[MAX_NAME_LEN];
	SWORD                   cbColNameMax;
	SWORD                   pcbColName;
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
	lstrcat(create," (ID INTEGER, NAME VARCHAR(80), AGE SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname,TABLE);
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," VALUES (10, 'TestName', 40)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Select Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname, TABLE);
	lstrcpy(select,"SELECT * FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Describe Columns------------------------------------------ */

	/* Column 1. */
	icol = 1; cbColNameMax = MAX_NAME_LEN;
	rc = SQLDescribeCol(*phstmt, icol, szColName, cbColNameMax, &pcbColName,
			    &pfSqlType, &pcbColDef, &pibScale, &pfNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeCol");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 1. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\ticol	: '%d'\r\n",icol);
	szLogPrintf(lpSrvr, FALSE,"\tszColName	: '%s'\r\n",szColName);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColName	: '%d'\r\n",pcbColName);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef	: '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale	: '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);

	/* Column 2. */
	icol = 2; cbColNameMax = MAX_NAME_LEN;
	rc = SQLDescribeCol(*phstmt, icol, szColName, cbColNameMax, &pcbColName,
			    &pfSqlType, &pcbColDef, &pibScale, &pfNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeCol");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 2. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\ticol       : '%d'\r\n",icol);
	szLogPrintf(lpSrvr, FALSE,"\tszColName  : '%s'\r\n",szColName);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColName : '%d'\r\n",pcbColName);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef  : '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale   : '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);

	/* Column 3. */
	icol = 3; cbColNameMax = MAX_NAME_LEN;
	rc = SQLDescribeCol(*phstmt, icol, szColName, cbColNameMax, &pcbColName,
			    &pfSqlType, &pcbColDef, &pibScale, &pfNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeCol");

	szLogPrintf(lpSrvr, FALSE,"\tSQLDescribeCol 3. -> (ouput) \r\n");
	szLogPrintf(lpSrvr, FALSE,"\ticol       : '%d'\r\n",icol);
	szLogPrintf(lpSrvr, FALSE,"\tszColName  : '%s'\r\n",szColName);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColName : '%d'\r\n",pcbColName);
	Get_pfSqlType(pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfSqlType	: '%d'- %s\r\n",pfSqlType, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpcbColDef  : '%d'\r\n",pcbColDef);
	szLogPrintf(lpSrvr, FALSE,"\tpibScale   : '%d'\r\n",pibScale);
	Get_pfNullable(pfNullable, buffer);
	szLogPrintf(lpSrvr, FALSE,"\tpfNullable	: '%d'- %s\r\n",pfNullable,buffer);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
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
/* ------------------------------------------------------------------------- */
