/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSPECO.C
**
** Purpose      : Auto Test DLL, Test von SQLSpecialColumns
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "SQLSpecialColumns"
#define TABLE1 "TABSPECCOL1"
#define TABLE2 "TABSPECCOL2"
#define TABLE3 "TABSPECCOL3"

#define STR_LEN 128+1
#define MAX_NAME_LEN 50

#define OUTPUTTXT "Driver does not support this function !"

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
RETCODE lstSpecialColInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

/* ****************************  Test Cases  ***************************** */
/*  The following functions implement the tests				   */
/* *********************************************************************** */

/*
 ------------------------------------------------------------------------
| DoSpecilaColumns:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 ------------------------------------------------------------------------
*/
SWORD FAR PASCAL DoSQLSpecialColumns(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];

	UWORD			fColType;
	UCHAR			szTableQualifier[MAX_NAME_LEN],
				szTableOwner[MAX_NAME_LEN],
				szTableName[MAX_NAME_LEN];

	UWORD			fScope,
				fNullable;

	SWORD			cbTableQualifier, cbTableOwner, cbTableName;

/* ---------------------------------------------------------------------har- */

	/* --- Connect ----------------------------------------------------- */
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

	/* --- Create Table 1. ------------------------------------------- */
	lstrcpy(tabname,TABLE1);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (MNR INTEGER, NAME CHAR(30))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 1.= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Create Table 2. ------------------------------------------- */
	lstrcpy(tabname,TABLE2);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (MNR INTEGER KEY, NAME CHAR(30))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 2.= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Create Table 3. ------------------------------------------- */
	lstrcpy(tabname,TABLE3);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (MNR INTEGER, NAME CHAR(30), PRIMARY KEY (NAME))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 3.= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- SQLSpecialColumns 1. ---------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\r\n\t1.List SpecialColumns Table -> %s \r\n",TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\t	fColType -> SQL_BEST_ROWID, fScope -> SQL_SCOPE_CURROW \r\n");
	fColType = SQL_BEST_ROWID;
	fScope = SQL_SCOPE_CURROW;
 	fNullable = SQL_NULLABLE;
	lstrcpy(szTableQualifier, "\0");
	lstrcpy(szTableOwner, lpSrvr->szValidLogin0);
	lstrcpy(szTableName, TABLE1);
	cbTableQualifier = cbTableOwner = cbTableName = SQL_NTS;

	rc = SQLSpecialColumns(*phstmt, fColType, szTableQualifier, SQL_NTS,
				szTableOwner, SQL_NTS, szTableName, SQL_NTS,
				fScope, fNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSpecialColumns");

	if (rc==SQL_SUCCESS) lstSpecialColInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- SQLSpecialColumns 2. ---------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\r\n\t2.List SpecialColumns Table -> %s \r\n",TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\t	fColType -> SQL_BEST_ROWID, fScope -> SQL_SCOPE_CURROW \r\n");
	fColType = SQL_BEST_ROWID;
	fScope = SQL_SCOPE_CURROW;
 	fNullable = SQL_NULLABLE;
	lstrcpy(szTableQualifier, "\0");
	lstrcpy(szTableOwner, lpSrvr->szValidLogin0);
	lstrcpy(szTableName, TABLE2);
	cbTableQualifier = cbTableOwner = cbTableName = SQL_NTS;

	rc = SQLSpecialColumns(*phstmt, fColType, szTableQualifier, SQL_NTS,
				szTableOwner, SQL_NTS, szTableName, SQL_NTS,
				fScope, fNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSpecialColumns");

	if (rc==SQL_SUCCESS) lstSpecialColInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- SQLSpecialColumns 3. ---------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\r\n\t3.List SpecialColumns Table -> %s \r\n",TABLE3);
	szLogPrintf(lpSrvr, FALSE,"\t	fColType -> SQL_BEST_ROWID, fScope -> SQL_SCOPE_TRANSACTION \r\n");
	fColType = SQL_BEST_ROWID;
	fScope = SQL_SCOPE_TRANSACTION;
 	fNullable = SQL_NULLABLE;
	lstrcpy(szTableQualifier, "\0");
	lstrcpy(szTableOwner, lpSrvr->szValidLogin0);
	lstrcpy(szTableName, TABLE3);
	cbTableQualifier = cbTableOwner = cbTableName = SQL_NTS;

	rc = SQLSpecialColumns(*phstmt, fColType, szTableQualifier, SQL_NTS,
				szTableOwner, SQL_NTS, szTableName, SQL_NTS,
				fScope, fNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSpecialColumns");

	if (rc==SQL_SUCCESS) lstSpecialColInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- SQLSpecialColumns 4. ---------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\r\n\t4.List SpecialColumns Table -> %s \r\n",TABLE3);
	szLogPrintf(lpSrvr, FALSE,"\t	fColType -> SQL_BEST_ROWID, fScope -> SQL_SCOPE_SESSION \r\n");
	fColType = SQL_BEST_ROWID;
	fScope = SQL_SCOPE_SESSION;
 	fNullable = SQL_NULLABLE;
	lstrcpy(szTableQualifier, "\0");
	lstrcpy(szTableOwner, lpSrvr->szValidLogin0);
	lstrcpy(szTableName, TABLE3);
	cbTableQualifier = cbTableOwner = cbTableName = SQL_NTS;

	rc = SQLSpecialColumns(*phstmt, fColType, szTableQualifier, SQL_NTS,
				szTableOwner, SQL_NTS, szTableName, SQL_NTS,
				fScope, fNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSpecialColumns");

	if (rc==SQL_SUCCESS) lstSpecialColInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- SQLSpecialColumns 5. ---------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\r\n\t5.List SpecialColumns Table -> %s \r\n",TABLE3);
	szLogPrintf(lpSrvr, FALSE,"\t	fColType -> SQL_ROWVER, fScope -> SQL_SCOPE_CURROW \r\n");
	fColType = SQL_ROWVER;
	fScope = SQL_SCOPE_CURROW;
 	fNullable = SQL_NULLABLE;
	lstrcpy(szTableQualifier, "\0");
	lstrcpy(szTableOwner, lpSrvr->szValidLogin0);
	lstrcpy(szTableName, TABLE1);
	cbTableQualifier = cbTableOwner = cbTableName = SQL_NTS;

	rc = SQLSpecialColumns(*phstmt, fColType, szTableQualifier, SQL_NTS,
				szTableOwner, SQL_NTS, szTableName, SQL_NTS,
				fScope, fNullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSpecialColumns");

	if (rc==SQL_SUCCESS) lstSpecialColInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table 1. -------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE1);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Drop Table 2. -------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE2);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Drop Table 3. -------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE3);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
	/* ----------------------------------------------------------------- */

	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	rc = SQLFreeConnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	/*  check for errors */
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* *********************************************************************** */

RETCODE lstSpecialColInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szColumnName[STR_LEN],
		szTypeName[STR_LEN];
	UCHAR   szScope[STR_LEN],
		szPseudoColumn[STR_LEN],
		szScale[STR_LEN];
	SWORD   Scope,
		DataType,
		Scale,
		PseudoColumn;
	SDWORD  Precision,
		Length;

	/* Declare storage locations for bytes available to return */
	SDWORD cbScope, cbColumnName, cbDataType, cbTypeName, cbPrecision;
	SDWORD cbLength, cbScale, cbPseudoColumn;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_SSHORT, &Scope, 0, &cbScope);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szColumnName, STR_LEN, &cbColumnName);
	SQLBindCol(*phstmt, 3, SQL_C_SSHORT, &DataType, 0, &cbDataType);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szTypeName, STR_LEN, &cbTypeName);
	SQLBindCol(*phstmt, 5, SQL_C_SLONG, &Precision, 0, &cbPrecision);
	SQLBindCol(*phstmt, 6, SQL_C_SLONG, &Length, 0, &cbLength);
	SQLBindCol(*phstmt, 7, SQL_C_SSHORT, &Scale, 0, &cbScale);
	SQLBindCol(*phstmt, 8, SQL_C_SSHORT, &PseudoColumn, 0, &cbPseudoColumn);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 8);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbColumnName == SQL_NULL_DATA) lstrcpy(szColumnName, NULL_VALUE);
			if (cbTypeName == SQL_NULL_DATA) lstrcpy(szTypeName, NULL_VALUE);
			if (cbScale == SQL_NULL_DATA) lstrcpy(szScale, NULL_VALUE);
				else sprintf(szScale, "%d", Scale);

			/* Scope information */
			switch (Scope) {
				case SQL_SCOPE_CURROW		: strcpy(szScope,"SQL_SCOPE_CUUROW"); break;
				case SQL_SCOPE_TRANSACTION	: strcpy(szScope,"SQL_SCOPE_TRANSACTION"); break;
				case SQL_SCOPE_SESSION		: strcpy(szScope,"SQL_SCOPE_SESSION"); break;
			}
			/* PseudoColumn information */
			switch (PseudoColumn) {
				case SQL_PC_UNKNOWN	: strcpy(szPseudoColumn,"SQL_PC_UNKNOWN"); break;
				case SQL_PC_PSEUDO	: strcpy(szPseudoColumn,"SQL_PC_PSEUDO"); break;
				case SQL_PC_NOT_PSEUDO	: strcpy(szPseudoColumn,"SQL_PC_NOT_PSEUDO"); break;
			}
			szLogPrintf(lpSrvr, FALSE,
			  "\tColumn %d : '%s','%s','%d','%s','%d','%d','%s','%s'\r\n",
			  count, szScope, szColumnName, DataType, szTypeName,
			  Precision, Length,  szScale, szPseudoColumn);
		} else { break; }
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
