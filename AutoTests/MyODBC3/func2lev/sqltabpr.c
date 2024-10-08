/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLTABPR.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLTablePrivileges
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLTABLEPRIVILEGES"
#define TABLE "TABSQLTABPRIV"

#define USER1 "PUBLIC"
#define USER2 "DOMAIN"

#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
RETCODE lstTablePrivilegesInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLTablePrivileges:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLTablePrivileges(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   grant[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];

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
	/* --- Create Table  -------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (CUST_ID INTEGER, CUST_NAME CHAR(30) )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table: "TABLE")= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Grant Table 1. (USER1) ------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(grant,"GRANT SELECT, INSERT, UPDATE, ALTER ON ");
	lstrcat(grant,tabname);
	lstrcat(grant," TO "USER1);
	szLogPrintf(lpSrvr, FALSE,"\tGrant Stmt 1.(Table: "TABLE")= '%s'\r\n",grant);

	rc = SQLExecDirect(*phstmt, grant, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Grant Table 2. (USER2) ------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(grant,"GRANT ALL ON ");
	lstrcat(grant,tabname);
	lstrcat(grant," TO "USER2" WITH GRANT OPTION");
	szLogPrintf(lpSrvr, FALSE,"\tGrant Stmt 2.(Table: "TABLE")= '%s'\r\n",grant);

	rc = SQLExecDirect(*phstmt, grant, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
/* ------------------------------------------------------------------------- */
	/* ***************************************************************** */
	/* *** I. SQLTablePrivileges *************************************** */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLTablePrivileges -> (TableOwner: %s - TableName: "TABLE" )\r\n", lpSrvr->szValidLogin0);

	rc = SQLTablePrivileges(*phstmt,
			NULL, 0, 			/* Table qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Table owner     */
			TABLE, SQL_NTS);		/* Table name      */
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTablePrivileges");

	if (rc == SQL_SUCCESS) lstTablePrivilegesInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	/* --- Drop Table 1. ------------------------------------------ */
	lstrcpy(tabname, TABLE);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table:"TABLE")= '%s'\r\n",drop);

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
/* ------------------------------------------------------------------------- */
RETCODE lstTablePrivilegesInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szTableQualifier[STR_LEN],
		szTableOwner[STR_LEN],
	        szTableName[STR_LEN],
		szGrantor[STR_LEN],
		szGrantee[STR_LEN],
		szPrivilege[STR_LEN],
		szIsGrantable[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SDWORD 	cbTableQualifier, cbTableOwner, cbTableName,
		cbGrantor, cbGrantee, cbPrivilege, cbIsGrantable;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szTableQualifier, STR_LEN,&cbTableQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szTableOwner, STR_LEN, &cbTableOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szGrantor, STR_LEN,&cbGrantor);
	SQLBindCol(*phstmt, 5, SQL_C_CHAR, szGrantee, STR_LEN,&cbGrantee);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szPrivilege, STR_LEN,&cbPrivilege);
	SQLBindCol(*phstmt, 7, SQL_C_CHAR, szIsGrantable, STR_LEN,&cbIsGrantable);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 7);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");*/

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbTableQualifier == SQL_NULL_DATA) lstrcpy(szTableQualifier, NULL_VALUE);
			if (cbTableOwner == SQL_NULL_DATA) lstrcpy(szTableOwner, NULL_VALUE);
			if (cbTableName == SQL_NULL_DATA) lstrcpy(szTableName, NULL_VALUE);
			if (cbGrantor == SQL_NULL_DATA) lstrcpy(szGrantor, NULL_VALUE);
			if (cbGrantee == SQL_NULL_DATA) lstrcpy(szGrantee, NULL_VALUE);
			if (cbPrivilege == SQL_NULL_DATA) lstrcpy(szPrivilege, NULL_VALUE);
			if (cbIsGrantable == SQL_NULL_DATA) lstrcpy(szIsGrantable, NULL_VALUE);

			szLogPrintf(lpSrvr, FALSE,
			  "\tTable %d : '%s','%s','%s','%s','%s','%s','%s'\r\n",
			  count, szTableQualifier, szTableOwner, szTableName,
			  szGrantor, szGrantee, szPrivilege, szIsGrantable);
		} else {
			/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */
			break;
		}
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
