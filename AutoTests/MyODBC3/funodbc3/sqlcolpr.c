/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLCOLPR.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLColumnPrivileges
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLCOLUMNPRIVILEGES"
#define TABLE "TABSQLCOLPRIV"

#define USER1 "PUBLIC"
#define USER2 "DOMAIN"

#define MAX_NAME_LEN 512

#define STR_LEN 254+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
RETCODE lstColumnPrivilegesInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLColumnPrivileges:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLColumnPrivileges(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   grant[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];

/* ---------------------------------------------------------------------har- */

	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
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

/* ------------------------------------------------------------------------- */
	/* --- Create Table  -------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (CUST_ID INTEGER, CUST_NAME CHAR(30) )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table: "TABLE")= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Grant Table 1. (USER1) ------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(grant,"GRANT SELECT (CUST_NAME) ON ");
	lstrcat(grant,tabname);
	lstrcat(grant," TO "USER1);
	szLogPrintf(lpSrvr, FALSE,"\tGrant Stmt 1.(Table: "TABLE")= '%s'\r\n",grant);

	rc = SQLExecDirect(*phstmt, grant, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Grant Table 2. (USER2) ------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(grant,"GRANT SELECT (CUST_NAME), UPDATE (CUST_NAME) ON ");
	lstrcat(grant,tabname);
	lstrcat(grant," TO "USER2" WITH GRANT OPTION");
	szLogPrintf(lpSrvr, FALSE,"\tGrant Stmt 2.(Table: "TABLE")= '%s'\r\n",grant);

	rc = SQLExecDirect(*phstmt, grant, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
/* ------------------------------------------------------------------------- */
	/* ***************************************************************** */
	/* *** I. SQLColumnPrivileges ************************************** */
	/* ***************************************************************** */
		szLogPrintf(lpSrvr, FALSE,"\tI.) SQLColumnPrivileges -> (TableOwner: %s - TableName: "TABLE" )\r\n", lpSrvr->szValidLogin0);

	rc = SQLColumnPrivileges(*phstmt,
			NULL, 0, 			/* Table qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Table owner     */
			TABLE, SQL_NTS, 		/* Table name      */
			"%", SQL_NTS);			/* Column name      */
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColumnPrivileges");

	if (rc == SQL_SUCCESS) lstColumnPrivilegesInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	/* --- Drop Table 1. ------------------------------------------ */
	lstrcpy(tabname, TABLE);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table:"TABLE")= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
#endif
/* ---------------------------------------------------------------------har- */
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
RETCODE lstColumnPrivilegesInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szTableQualifier[STR_LEN],
		szTableOwner[STR_LEN],
	        szTableName[STR_LEN],
	        szColumnName[STR_LEN],
		szGrantor[STR_LEN],
		szGrantee[STR_LEN],
		szPrivilege[STR_LEN],
		szIsGrantable[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SDWORD 	cbTableQualifier, cbTableOwner, cbTableName, cbColumnName,
		cbGrantor, cbGrantee, cbPrivilege, cbIsGrantable;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szTableQualifier, STR_LEN,&cbTableQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szTableOwner, STR_LEN, &cbTableOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szColumnName, STR_LEN,&cbColumnName);
	SQLBindCol(*phstmt, 5, SQL_C_CHAR, szGrantor, STR_LEN,&cbGrantor);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szGrantee, STR_LEN,&cbGrantee);
	SQLBindCol(*phstmt, 7, SQL_C_CHAR, szPrivilege, STR_LEN,&cbPrivilege);
	SQLBindCol(*phstmt, 8, SQL_C_CHAR, szIsGrantable, STR_LEN,&cbIsGrantable);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 8);

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
			if (cbGrantor == SQL_NULL_DATA) lstrcpy(szGrantor, NULL_VALUE);
			if (cbGrantee == SQL_NULL_DATA) lstrcpy(szGrantee, NULL_VALUE);
			if (cbPrivilege == SQL_NULL_DATA) lstrcpy(szPrivilege, NULL_VALUE);
			if (cbIsGrantable == SQL_NULL_DATA) lstrcpy(szIsGrantable, NULL_VALUE);

			szLogPrintf(lpSrvr, FALSE,
			  "\tColumn %d : '%s','%s','%s','%s','%s','%s','%s','%s'\r\n",
			  count, szTableQualifier, szTableOwner, szTableName, szColumnName,
			  szGrantor, szGrantee, szPrivilege, szIsGrantable);
		} else {
			/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */
			break;
		}
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
