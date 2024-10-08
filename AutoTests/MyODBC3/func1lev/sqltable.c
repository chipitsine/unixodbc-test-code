/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLTABLE.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLTABLES
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLTables"
#define TABLE "TABLESTST"
#define TABLE_OWNER "ODBC"
#define TABLE_TYPES "TABLE"

#define MAX_NAME_LEN 50

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"
//*-------------------------------------------------------------------------
RETCODE lstTableInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

/* ------------------------------------------------------------------------- */
//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLTables:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLTables(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */

	UCHAR                   szQualifier[STR_LEN], szOwner[STR_LEN],
				szTableName[STR_LEN], szTableType[STR_LEN];

	/* Declare storage locations for bytes available to return */

	SWORD                   cbQualifier, cbOwner, cbTableName,
				cbTableType;

	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];

	SWORD                   count=0;
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
	/* *** SQLGetInfo check the SQL_ACCESSIBLE_TABLES info value */
	/* Driver not capable !!! */
/*      rc = SQLGetInfo(*phdbc, SQL_ACCESSIBLE_TABLES, (PTR) &rgbInfoValue,
				sizeof(rgbInfoValue), NULL);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetInfo");
	szLogPrintf(lpSrvr,FALSE,"\tSQLGetInfo -> SQL_ACCESSIBLE_TABLES: '%d'\r\n",
			rgbInfoValue);
*/
/* ------------------------------------------------------------------------- */
	/* *** SQLTablePrivileges, check the privileges for each table */
	/* Driver not capable !!! */
/*      rc = SQLTablePrivileges(*phstmt, szTableQualifier, cbTableQualifier,
		szTableOwner, cbTableOwner, szTableName, cbTableName);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTablePrivileges");
*/
/* ------------------------------------------------------------------------- */
	/* ***************************************************************** */
	/* *** I.) Result set contains a list of valid qualifier for the     */
	/* ***     data source                                               */
	/* ***************************************************************** */
	/* All qualifiers, all owners, EMPLOYEE table, all columns */
	lstrcpy(szQualifier,"%");
	lstrcpy(szOwner,"");
	lstrcpy(szTableName,"");
	/* lstrcpy(szTableType,""); */
	cbQualifier = SQL_NTS;
	cbOwner = cbTableName = cbTableType = 0;

	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLTables -> list for all qualifier : Qualifier=%s'\r\n",
			szQualifier);

	rc = SQLTables(*phstmt, szQualifier, cbQualifier, szOwner, cbOwner,
			szTableName, cbTableName, szTableType, cbTableType);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTables");

	if (rc == SQL_SUCCESS) lstTableInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* ***************************************************************** */
	/* *** II.) Result set contains a list of valid owners for the       */
	/* ***      data source                                              */
	/* ***************************************************************** */
	lstrcpy(szQualifier,"");
	lstrcpy(szOwner,"%");
	lstrcpy(szTableName,"");
	/* lstrcpy(szTableType,""); */
	cbQualifier = cbTableName = cbTableType = 0 ;
	cbOwner = SQL_NTS;

	szLogPrintf(lpSrvr, FALSE,"\tII.) SQLTables -> list for all owners : Owner=%s \r\n",
			szOwner);
/* --- mysql ???
	rc = SQLTables(*phstmt, szQualifier, cbQualifier, szOwner, cbOwner,
			szTableName, cbTableName, szTableType, cbTableType);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTables");
*/
	if (rc == SQL_SUCCESS) lstTableInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* ***************************************************************** */
	/* *** III.) Result set contains a list of valid types for the       */
	/* ***       data source                                             */
	/* ***************************************************************** */
	lstrcpy(szQualifier,"");
	lstrcpy(szOwner,"");
	lstrcpy(szTableName,"");
	lstrcpy(szTableType,"%");
	cbQualifier = cbOwner = cbTableName = 0;
	cbTableType = SQL_NTS;

	szLogPrintf(lpSrvr, FALSE,"\tIII.) SQLTables -> list for all types : TableTypes=%s \r\n",
			szTableType);
/* --- mysql ???
	rc = SQLTables(*phstmt, szQualifier, cbQualifier, szOwner, cbOwner,
			szTableName, cbTableName, szTableType, cbTableType);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTables");
*/
	if (rc == SQL_SUCCESS) lstTableInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* ***************************************************************** */
	/* *** IV.) With other search pattern                                */
	/* ***                                                               */
	/* ***************************************************************** */
	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname,TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (TYP_CHAR CHAR(30), TYP_SMALLINT SMALLINT )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --------------------------------------------------------------- */

	lstrcpy(szQualifier,"");
	lstrcpy(szOwner,TABLE_OWNER);
	lstrcpy(szTableName,"%");
	lstrcpy(szTableType,TABLE_TYPES);
	cbQualifier = 0;
	cbOwner = cbTableName = cbTableType = SQL_NTS;

	szLogPrintf(lpSrvr, FALSE,"\tIV.) SQLTables -> list for search pattern : Owner=%s, TableName=%s, TableType=%s'\r\n",
			szOwner, szTableName, szTableType);
/* --- mysql ???
	rc = SQLTables(*phstmt, szQualifier, cbQualifier, szOwner, cbOwner,
			szTableName, cbTableName, szTableType, cbTableType);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTables");
*/
	if (rc == SQL_SUCCESS) lstTableInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* ----------------------------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
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
/* ------------------------------------------------------------------------- */

RETCODE lstTableInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */

	UCHAR                   szQualifier[STR_LEN], szOwner[STR_LEN],
				szTableName[STR_LEN], szTableType[STR_LEN],
				szRemarks[REM_LEN];

	/* Declare storage locations for bytes available to return */

	SDWORD                  cbQualifier, cbOwner, cbTableName,
				cbTableType, cbRemarks;

	SWORD                   count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szQualifier, STR_LEN,&cbQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szOwner, STR_LEN, &cbOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szTableType, STR_LEN,&cbTableType);
	SQLBindCol(*phstmt, 5, SQL_C_CHAR, szRemarks, REM_LEN, &cbRemarks);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 5);

	while(TRUE) {
		count++;
		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */

		if (rc == SQL_ERROR || rc == SQL_SUCCESS_WITH_INFO) {
			/* Show Error */
			/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLError"); */
		}
		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
		   /* Process fetched data */
		   if (cbQualifier == SQL_NULL_DATA) lstrcpy(szQualifier, NULL_VALUE);
		   if (cbOwner == SQL_NULL_DATA) lstrcpy(szOwner, NULL_VALUE);
		   if (cbTableName == SQL_NULL_DATA) lstrcpy(szTableName, NULL_VALUE);
		   if (cbTableType == SQL_NULL_DATA) lstrcpy(szTableType, NULL_VALUE);
		   if (cbRemarks == SQL_NULL_DATA) lstrcpy(szRemarks, NULL_VALUE);

		   szLogPrintf(lpSrvr, FALSE,"\tRow %d.: '%s','%s','%s','%s','%s'\r\n",
		   count, szQualifier, szOwner, szTableName, szTableType, szRemarks);
		} else { break; }
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
