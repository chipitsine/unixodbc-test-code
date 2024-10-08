/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLFRKEY.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLForeignKeys
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLFOREIGNKEYS"

#define TABLE1 "FR_CUSTOMER"
#define TABLE2 "FR_EMPLOYEE"
#define TABLE3 "FR_SALES_ORDER"
#define TABLE4 "FR_SALES_LINE"


#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
int PrintString(UCHAR *, DWORD, UCHAR *, lpSERVERINFO);

RETCODE lstForeignKeysInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLForeignKeys:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLForeignKeys(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	/* Declare storage locations for result set data */
	UCHAR   szPkTableName[STR_LEN];
	UCHAR   szFkTableName[STR_LEN];
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
	/* --- Create Table 1. ------------------------------------------ */
	lstrcpy(tabname, TABLE1);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (CUSTOMER_ID INTEGER, CUST_NAME CHAR(30),ADRESS CHAR(60), PHONE CHAR(15), PRIMARY KEY (CUSTOMER_ID))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table1: CUSTOMER)= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Create Table 2. ------------------------------------------ */
	lstrcpy(tabname, TABLE2);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (EMPLOYEE_ID INTEGER, NAME CHAR(30),AGE INTEGER, BIRTHDAY DATE, PRIMARY KEY (EMPLOYEE_ID)) ");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table2: EMPLOYEE)= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Create Table 3. ------------------------------------------ */
	lstrcpy(tabname, TABLE3);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (SALES_ID INTEGER, CUSTOMER_ID INTEGER,EMPLOYEE_ID INTEGER, TOTAL_PRICE FIXED(6,2), ");
	lstrcat(create,"PRIMARY KEY (SALES_ID), ");
	lstrcat(create,"FOREIGN KEY (CUSTOMER_ID) REFERENCES "TABLE1" (CUSTOMER_ID), ");
	lstrcat(create,"FOREIGN KEY (EMPLOYEE_ID) REFERENCES "TABLE2" (EMPLOYEE_ID)) ");
	sprintf(buffer, "\tCreate Stmt (Table3: SALES_ORDER)= ");
	PrintString(create, lstrlen(create), buffer, lpSrvr);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Create Table 4. ------------------------------------------ */
	lstrcpy(tabname, TABLE4);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (SALES_ID INTEGER, LINE_NUMBER INTEGER, PART_ID INTEGER, QUANTITY INTEGER, PRICE FIXED(6,2), ");
	lstrcat(create,"FOREIGN KEY (SALES_ID) REFERENCES "TABLE3" (SALES_ID)) ");
	sprintf(buffer, "\tCreate Stmt (Table4: SALES_LINE)= ");
	PrintString(create, lstrlen(create), buffer, lpSrvr);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

/* ------------------------------------------------------------------------- */
	/* ***************************************************************** */
	/* *** I. SQLForeignKeys (PkTableName) ***************************** */
	/* ***************************************************************** */
	lstrcpy(szPkTableName,TABLE3);

	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLForeignKeys -> with primary key tablename: %s'\r\n",
			szPkTableName);

	/* Get all the foreign keys that refer to TABLE3 primary key.        */
	/* Wenn Ownername -> "lpSrvr->szValidLogin0, SQL_NTS,"*/
	rc = SQLForeignKeys(*phstmt,
			NULL, 0,                        /* Primary qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Primary owner     */
			szPkTableName, SQL_NTS,         /* Primary table     */
			NULL, 0,                        /* Foreign qualifier */
			NULL, 0,                        /* Foreign owner     */
			NULL, 0);                       /* Foreign table     */
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLForeignKeys");

	if (rc == SQL_SUCCESS) lstForeignKeysInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

	/* ***************************************************************** */
	/* *** II. SQLForeignKeys (FkTableName) ***************************** */
	/* ***************************************************************** */
	lstrcpy(szFkTableName,TABLE3);

	szLogPrintf(lpSrvr, FALSE,"\tII.) SQLForeignKeys -> with foreign key tablename: %s'\r\n",
			szFkTableName);

	/* Get all the foreign keys in the TABLE3 table.                     */
	/* Wenn Ownername -> "lpSrvr->szValidLogin0, SQL_NTS,"*/
	rc = SQLForeignKeys(*phstmt,
			NULL, 0,                        /* Primary qualifier */
			NULL, 0,                        /* Primary owner     */
			NULL, 0,                        /* Primary table     */
			NULL, 0,                        /* Foreign qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Foreign owner     */
			szFkTableName, SQL_NTS);        /* Foreign table     */
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLForeignKeys");

	if (rc == SQL_SUCCESS) lstForeignKeysInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

	/* ***************************************************************** */
	/* *** III. SQLForeignKeys (PkTableName and FkTableName) *********** */
	/* ***************************************************************** */
	lstrcpy(szPkTableName,TABLE3);
	lstrcpy(szFkTableName,TABLE4);

	szLogPrintf(lpSrvr, FALSE,"\tIII.) SQLForeignKeys -> with primary key tablename: '%s' and foreign key tablename: '%s'\r\n",
			szPkTableName, szFkTableName);

	/* Get all the foreign keys in the TABLE3 table,refer to primary key.*/
	/* Wenn Ownername -> "lpSrvr->szValidLogin0, SQL_NTS,"*/
	rc = SQLForeignKeys(*phstmt,
			NULL, 0,                        /* Primary qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Primary owner     */
			szPkTableName, SQL_NTS,         /* Primary table     */
			NULL, 0,                        /* Foreign qualifier */
			lpSrvr->szValidLogin0, SQL_NTS, /* Foreign owner     */
			szFkTableName, SQL_NTS);        /* Foreign table     */
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLForeignKeys");

	if (rc == SQL_SUCCESS) lstForeignKeysInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");
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
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
	/* --- Drop Table 3. ------------------------------------------ */
	lstrcpy(tabname, TABLE3);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table3:SALES_ORDER)= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
	/* --- Drop Table 4. ------------------------------------------ */
	lstrcpy(tabname, TABLE4);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table4:SALES_LINE)= '%s'\r\n",drop);

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
/* ------------------------------------------------------------------------ */

RETCODE lstForeignKeysInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szPkQualifier[STR_LEN],
		szPkOwner[STR_LEN],
		szPkTableName[STR_LEN],
		szPkColumnName[STR_LEN];
	UCHAR   szFkQualifier[STR_LEN],
		szFkOwner[STR_LEN],
		szFkTableName[STR_LEN],
		szFkColumnName[STR_LEN];
	UCHAR   szFkName[STR_LEN],
		szPkName[STR_LEN];
	SWORD   KeySeq,
		UpdateRule,
		DeleteRule;
	UCHAR   szUpdateRule[STR_LEN],
		szDeleteRule[STR_LEN],
		szKeySeq[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SDWORD cbPkQualifier, cbPkOwner, cbPkTableName, cbPkColumnName;
	SDWORD cbFkQualifier, cbFkOwner, cbFkTableName, cbFkColumnName;
	SDWORD cbKeySeq, cbUpdateRule, cbDeleteRule, cbFkName, cbPkName;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */

	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szPkQualifier, STR_LEN,&cbPkQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szPkOwner, STR_LEN, &cbPkOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szPkTableName, STR_LEN,&cbPkTableName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szPkColumnName, STR_LEN,&cbPkColumnName);
	SQLBindCol(*phstmt, 5, SQL_C_CHAR, szFkQualifier, STR_LEN,&cbFkQualifier);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szFkOwner, STR_LEN, &cbFkOwner);
	SQLBindCol(*phstmt, 7, SQL_C_CHAR, szFkTableName, STR_LEN,&cbFkTableName);
	SQLBindCol(*phstmt, 8, SQL_C_CHAR, szFkColumnName, STR_LEN,&cbFkColumnName);
	SQLBindCol(*phstmt, 9, SQL_C_SHORT, &KeySeq, 0, &cbKeySeq);
	SQLBindCol(*phstmt,10, SQL_C_SHORT, &UpdateRule, 0, &cbUpdateRule);
	SQLBindCol(*phstmt,11, SQL_C_SHORT, &DeleteRule, 0, &cbDeleteRule);
	SQLBindCol(*phstmt,12, SQL_C_CHAR, szFkName, STR_LEN,&cbFkName);
	SQLBindCol(*phstmt,13, SQL_C_CHAR, szPkName, STR_LEN,&cbPkName);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 13);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbPkQualifier == SQL_NULL_DATA) lstrcpy(szPkQualifier, NULL_VALUE);
			if (cbPkOwner == SQL_NULL_DATA) lstrcpy(szPkOwner, NULL_VALUE);
			if (cbPkTableName == SQL_NULL_DATA) lstrcpy(szPkTableName, NULL_VALUE);
			if (cbPkColumnName == SQL_NULL_DATA) lstrcpy(szPkColumnName, NULL_VALUE);
			if (cbFkQualifier == SQL_NULL_DATA) lstrcpy(szFkQualifier, NULL_VALUE);
			if (cbFkOwner == SQL_NULL_DATA) lstrcpy(szFkOwner, NULL_VALUE);
			if (cbFkTableName == SQL_NULL_DATA) lstrcpy(szFkTableName, NULL_VALUE);
			if (cbFkColumnName == SQL_NULL_DATA) lstrcpy(szFkColumnName, NULL_VALUE);
			if (cbUpdateRule == SQL_NULL_DATA) lstrcpy(szUpdateRule, NULL_VALUE);
			if (cbDeleteRule == SQL_NULL_DATA) lstrcpy(szDeleteRule, NULL_VALUE);
			if (cbFkName == SQL_NULL_DATA) lstrcpy(szFkName, NULL_VALUE);
			if (cbPkName == SQL_NULL_DATA) lstrcpy(szPkName, NULL_VALUE);

			if (cbKeySeq == SQL_NULL_DATA) lstrcpy(szKeySeq, NULL_VALUE);
			  else	sprintf(szKeySeq, "%d", KeySeq);

			/* UpdateRule information */
			switch (UpdateRule) {
				case SQL_CASCADE        : strcpy(szUpdateRule,"SQL_CASCADE"); break;
				case SQL_RESTRICT       : strcpy(szUpdateRule,"SQL_RESTRICT"); break;
				case SQL_SET_NULL       : strcpy(szUpdateRule,"SQL_SET_NULL"); break;
			}
			/* DeleteRule information */
			switch (DeleteRule) {
				case SQL_CASCADE        : strcpy(szDeleteRule,"SQL_CASCADE"); break;
				case SQL_RESTRICT       : strcpy(szDeleteRule,"SQL_RESTRICT"); break;
				case SQL_SET_NULL       : strcpy(szDeleteRule,"SQL_SET_NULL"); break;
			}
			szLogPrintf(lpSrvr, FALSE,
			  "\tColumn %d : '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s' \r\n",
			  count, szPkQualifier, szPkOwner, szPkTableName, szPkColumnName,
			  szFkQualifier, szFkOwner, szFkTableName, szFkColumnName,
			  szKeySeq, szUpdateRule, szDeleteRule, szFkName, szPkName);
		} else { break; }
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
