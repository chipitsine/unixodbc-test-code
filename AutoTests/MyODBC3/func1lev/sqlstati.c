/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLSTATI.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSTATISTICS
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLStatistics"
#define TABLE "TABSTATIST"

#define MAX_NAME_LEN 50

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLStatistic:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLStatistics(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   colname[MAX_NAME_LEN+1];
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	/* Declare storage locations for result set data */

	UCHAR  szQualifier[STR_LEN], szOwner[STR_LEN], szTableName[STR_LEN];
	UCHAR  szIndexQualifier[STR_LEN], szIndexName[STR_LEN];
	UCHAR  szColumnName[STR_LEN], szCollation[STR_LEN];
	UCHAR  szFilterCondition[STR_LEN];

	UCHAR  szType[STR_LEN];

	SWORD  NonUnique, Type, SeqInIndex;

	SDWORD Cardinality, Pages;

	UWORD  fUnique, fAccuracy;

	/* Declare storage locations for bytes available to return */

	SDWORD cbQualifier, cbOwner, cbTableName, cbNonUnique, cbIndexQualifier;
	SDWORD cbIndexName, cbType, cbSeqInIndex, cbColumnName, cbCollation;
	SDWORD cbCardinality, cbPages, cbFilterCondition;

	SWORD  count=0;
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
	lstrcat(create," (ID INTEGER, NAME CHAR(80), VNAME CHAR(80), STR CHAR(120), ORT CHAR(60), AGE SMALLINT, GEHALT FIXED(8,2))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Create Index --------------------------------------------- */
	lstrcpy(tabname,TABLE);
	lstrcpy(colname,"ID");
	lstrcpy(create,"CREATE UNIQUE INDEX ");
	lstrcat(create,tabname);
	lstrcat(create,".");
	lstrcat(create,colname);
	szLogPrintf(lpSrvr, FALSE,"\tCreate Index Stmt = '%s'\r\n",create);
/* -- mysql ???
	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
*/
/* ------------------------------------------------------------------------- */
	/* All qualifiers, all owners, EMPLOYEE table, all columns */

	fUnique = SQL_INDEX_ALL;  /* or. SQL_INDEX_UNIQUE */
	fAccuracy = SQL_QUICK;    /* or. SQL_ENSURE       */

	rc = SQLStatistics(*phstmt, NULL, 0, lpSrvr->szValidLogin0, SQL_NTS,
				    TABLE, SQL_NTS, SQL_INDEX_ALL, SQL_QUICK);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLStatistics");

	if (rc == SQL_SUCCESS) {
	/* Bind columns in result set to storage locations */

	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szQualifier, STR_LEN,&cbQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szOwner, STR_LEN, &cbOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(*phstmt, 4, SQL_C_SHORT, &NonUnique, 0, &cbNonUnique);
	SQLBindCol(*phstmt, 5, SQL_C_CHAR, szIndexQualifier, STR_LEN,&cbIndexQualifier);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szIndexName, STR_LEN,&cbIndexName);
	SQLBindCol(*phstmt, 7, SQL_C_SHORT, &Type, 0, &cbType);
	SQLBindCol(*phstmt, 8, SQL_C_SHORT, &SeqInIndex, 0, &cbSeqInIndex);
	SQLBindCol(*phstmt, 9, SQL_C_CHAR, szColumnName, STR_LEN,&cbColumnName);
	SQLBindCol(*phstmt,10, SQL_C_CHAR, szCollation, STR_LEN,&cbCollation);
	SQLBindCol(*phstmt,11, SQL_C_LONG, &Cardinality, 0, &cbCardinality);
	SQLBindCol(*phstmt,12, SQL_C_LONG, &Pages, 0, &cbPages);
	SQLBindCol(*phstmt,13, SQL_C_CHAR, szFilterCondition, STR_LEN, &cbFilterCondition);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 13);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

		if (rc == SQL_ERROR || rc == SQL_SUCCESS_WITH_INFO) {
			/* Show Error */
			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLError");
		}
		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			szLogPrintf(lpSrvr, FALSE,"\tColumn %d : ", count);

			/* *** TABLE_QUALIFIER ------------------- *** */
			if (cbQualifier == SQL_NULL_DATA) lstrcpy(szQualifier, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',", szQualifier);

			/* *** TABLE_OWNER ------------------- *** */
			if (cbOwner == SQL_NULL_DATA) lstrcpy(szOwner, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',", szOwner);

			/* *** TABLE_NAME ------------------- *** */
			szLogPrintf(lpSrvr, FALSE,"'%s',", szTableName);

			/* *** NON_UNIQUE ------------------- *** */
			if (cbNonUnique == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',", NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',", NonUnique);

			/* *** INDEX_QUALIFIER ------------------- *** */
			if (cbIndexQualifier == SQL_NULL_DATA) lstrcpy(szIndexQualifier, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',", szIndexQualifier);

			/* *** INDEX_NAME ------------------- *** */
			if (cbIndexName == SQL_NULL_DATA) lstrcpy(szIndexName, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',", szIndexName);

			/* *** TYPE ------------------- *** */
			/* Type of information */
			switch (Type) {
				case SQL_TABLE_STAT      : strcpy(szType,"SQL_TABLE_STAT"); break;
				case SQL_INDEX_CLUSTERED : strcpy(szType,"SQL_INDEX_CLUSTERED"); break;
				case SQL_INDEX_HASHED    : strcpy(szType,"SQL_INDEX_HASHED"); break;
				case SQL_INDEX_OTHER     : strcpy(szType,"SQL_INDEX_OTHER"); break;
			}
			szLogPrintf(lpSrvr, FALSE,"'%s',", szType);

			/* *** SEQ_IN_INDEX ------------------- *** */
			if (cbSeqInIndex == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',", NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',", SeqInIndex);

			/* *** COLUMN_NAME ------------------- *** */
			if (cbColumnName == SQL_NULL_DATA) lstrcpy(szColumnName, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',", szColumnName);

			/* *** COLLATION ------------------- *** */
			if (cbCollation == SQL_NULL_DATA) lstrcpy(szCollation, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',", szCollation);

			/* *** CARDINALITY ------------------- *** */
			if (cbCardinality == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',", NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',", Cardinality);

			/* *** PAGES ------------------- *** */
			if (cbPages == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',", NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',", Pages);

			/* *** FILTER_CONDITION ------------------- *** */
			if (cbFilterCondition == SQL_NULL_DATA) lstrcpy(szFilterCondition, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s' \r\n", szFilterCondition);

		} else { break; }
	    }
	}
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Index ----------------------------------------------- */
	lstrcpy(colname,"ID");
	lstrcpy(tabname,TABLE);
	lstrcpy(drop,"DROP INDEX ");
	lstrcat(drop,tabname);
	lstrcat(drop,".");
	lstrcat(drop,colname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Index Stmt= '%s'\r\n",drop);
/* -- mysql ???
	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
*/
	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(tabname, TABLE);
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
