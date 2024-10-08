/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : DESCRCOL.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLDescribeCol
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "sqlexamp.h"

//*-------------------------------------------------------------------------
#define TESTNAME "DescribeColumn"

#define TABLE "DESCRCOL"

#define MAX_NAME_LEN 		80
#define MAX_COL_NAME_LEN 	18

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

/* ---------------------------------------------------------------------- */
RETCODE lst_DescribeColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt,
				SWORD outcol, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*-------------------------------------------------------------------------
//| DoLDescribeColumn:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*-------------------------------------------------------------------------
SWORD FAR PASCAL DoDescribeColumn(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN*3];
	UCHAR                   select[(MAX_NAME_LEN+1)*2];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[(MAX_NAME_LEN+1)*2];
/* ------------------------------------------------------------------har- */
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

/* ---------------------------------------------------------------------- */

	/* --- Create Table  ------------------------------------------ */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE);
	lstrcat(create," (COL1TEST CHAR(20), COL2TEST CHAR(20), ");
	lstrcat(create,"COL3POSTEST CHAR(20), ");
	lstrcat(create,"C CHAR(10), COL5 CHAR(10)) ");
	szLogPrintf(lpSrvr, FALSE,"Create Stmt (Table: "TABLE")= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Select Table  ------------------------------------------ */
	lstrcpy(select,"SELECT * FROM ");
	lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"Select Stmt (Table: "TABLE")= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

/* ----------------------------------------------------------------------- */
	/* *************************************************************** */
	/* *** I. SQLDescribeCol ***************************************** */
	/* *************************************************************** */
	lstrcpy(buffer,"Describe Columns (Table: "TABLE") : \r\n");
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);

	rc = lst_DescribeColumnNames(*phenv, *phdbc, *phstmt, 5, lpSrvr);

/* -------------------------------------------------------------------har- */
	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE);
	sprintf(buffer,"Drop Stmt (Table:"TABLE")= '%s'\r\n",drop);
	szLogPrintf(lpSrvr, FALSE,"%s",buffer);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

#endif
/* ---------------------------------------------------------------------har- */

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
/* ---------------------------------------------------------------------- */
RETCODE lst_DescribeColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt,
				SWORD outcol, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	UWORD			icol;
	UCHAR                   szColName[MAX_NAME_LEN];
	SWORD                   cbColNameMax;
	SWORD                   pcbColName;
	SWORD			pfSqlType;
	UDWORD			pcbColDef;
	SWORD			pibScale;
	SWORD			pfNullable;
/* ----------------------------------------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"Columns -> : \r\n");

	icol = 1;
	cbColNameMax = MAX_COL_NAME_LEN+1;
	while (icol <= outcol) {
		rc = SQLDescribeCol(hstmt, icol, szColName, cbColNameMax,
					&pcbColName, &pfSqlType, &pcbColDef,
					&pibScale, &pfNullable);
		szLogPrintf(lpSrvr, FALSE,"%d. (%d) :	%s\r\n",
				 icol, pcbColName, szColName);
		icol++;
	}

	return(rc);
}
/* ----------------------------------------------------------------------- */
