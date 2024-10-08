/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLPRKEY.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLPrimaryKeys
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLPRIMARYKEYS"

#define TABLE1 "PR_CUSTOMER"
#define TABLE2 "PR_EMPLOYEE"

#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
RETCODE lstPrimaryKeysInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLPrimaryKeys:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLPrimaryKeys(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   drop[MAX_NAME_LEN+1];

	/* Declare storage locations for result set data */
	UCHAR   szTableName[STR_LEN];

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
	/* --- Create Table 1. ------------------------------------------ */
	lstrcpy(tabname, TABLE1);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (CUSTOMER_ID INTEGER, CUST_NAME CHAR(30),ADRESS CHAR(60), PHONE CHAR(15), PRIMARY KEY (CUSTOMER_ID))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table1: CUSTOMER)= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Create Table 2. ------------------------------------------ */
	lstrcpy(tabname, TABLE2);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (EMPLOYEE_ID INTEGER, NAME CHAR(30),AGE INTEGER, BIRTHDAY DATE, ");
	lstrcat(create,"PRIMARY KEY (EMPLOYEE_ID, NAME)) ");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table2: EMPLOYEE)= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

/* ------------------------------------------------------------------------- */
	/* ***************************************************************** */
	/* *** I. SQLPrimaryKeys ******************************************* */
	/* ***************************************************************** */
	lstrcpy(szTableName,TABLE1);

	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLPrimaryKeys -> all primary key for table: %s'\r\n",
			szTableName);

	/* Get all the primary keys. */
	rc = SQLPrimaryKeys(*phstmt,
			NULL, 0,                        /* Table qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Table owner     */
			szTableName, SQL_NTS);          /* Table name      */
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrimaryKeys");

	if (rc == SQL_SUCCESS) lstPrimaryKeysInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* ***************************************************************** */
	/* *** II. SQLPrimaryKeys ****************************************** */
	/* ***************************************************************** */
	lstrcpy(szTableName,TABLE2);

	szLogPrintf(lpSrvr, FALSE,"\tII.) SQLPrimaryKeys -> all primary key for table: %s'\r\n",
			szTableName);

	/* Get all the primary keys. */
	rc = SQLPrimaryKeys(*phstmt,
			NULL, 0,                        /* Table qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Table owner     */
			szTableName, SQL_NTS);          /* Table name      */
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrimaryKeys");

	if (rc == SQL_SUCCESS) lstPrimaryKeysInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	/* --- Drop Table 1. ------------------------------------------ */
	lstrcpy(tabname, TABLE1);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table1:CUSTOMER)= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
	/* --- Drop Table 2. ------------------------------------------ */
	lstrcpy(tabname, TABLE2);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table2:EMPLOYEE)= '%s'\r\n",drop);

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
/* ------------------------------------------------------------------------ */
RETCODE lstPrimaryKeysInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szTableQualifier[STR_LEN],
		szTableOwner[STR_LEN],
		szTableName[STR_LEN],
		szColumnName[STR_LEN];
	UCHAR   szPkName[STR_LEN];
	SWORD   KeySeq;

	/* Declare storage locations for bytes available to return */
	SDWORD cbTableQualifier, cbTableOwner, cbTableName, cbColumnName;
	SDWORD cbKeySeq, cbPkName;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szTableQualifier, STR_LEN,&cbTableQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szTableOwner, STR_LEN, &cbTableOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szColumnName, STR_LEN,&cbColumnName);
	SQLBindCol(*phstmt, 5, SQL_C_SHORT, &KeySeq, 0, &cbKeySeq);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szPkName, STR_LEN,&cbPkName);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 6);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");*/

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbTableQualifier == SQL_NULL_DATA) lstrcpy(szTableQualifier, NULL_VALUE);
			if (cbTableOwner == SQL_NULL_DATA) lstrcpy(szTableOwner, NULL_VALUE);
			if (cbTableName == SQL_NULL_DATA) lstrcpy(szTableName, NULL_VALUE);
			if (cbColumnName == SQL_NULL_DATA) lstrcpy(szColumnName, NULL_VALUE);
			if (cbPkName == SQL_NULL_DATA) lstrcpy(szPkName, NULL_VALUE);
			/*
			if (cbKeySeq == SQL_NULL_DATA) KeySeq=NULL;
			*/
			szLogPrintf(lpSrvr, FALSE,
			  "\tColumn %d : '%s','%s','%s','%s','%d','%s'\r\n",
			  count, szTableQualifier, szTableOwner, szTableName, szColumnName,
			  KeySeq, szPkName);
		} else { break; }
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
