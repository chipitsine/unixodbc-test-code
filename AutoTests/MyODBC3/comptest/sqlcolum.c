/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLCOLUMN.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLCOLUMNS
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "comptest.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLColumns"
#define TABLE "CMPCOLUMNS"

#define MAX_NAME_LEN 50

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLColumns:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLColumns(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1+MAX_NAME_LEN+MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];
	UCHAR                   buffer[MAX_NAME_LEN];

	/* Declare storage locations for result set data */

	UCHAR  szQualifier[STR_LEN], szOwner[STR_LEN];
	UCHAR  szTableName[STR_LEN], szColName[STR_LEN];
	UCHAR  szTypeName[STR_LEN], szRemarks[REM_LEN];
	SDWORD Precision, Length;
	SWORD  DataType, Scale, Radix, Nullable;

	/* Declare storage locations for bytes available to return */

	SDWORD cbQualifier, cbOwner, cbTableName, cbColName;
	SDWORD cbTypeName, cbRemarks, cbDataType, cbPrecision;

	SDWORD cbLength, cbScale, cbRadix, cbNullable;

	SWORD  count=0;

	struct _timeb           timebuf1, timebuf2;
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
	lstrcpy(tabname,TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (TYP_CHAR CHAR(30), TYP_SMALLINT SMALLINT, TYP_INTEGER INTEGER ,");
	lstrcat(create," TYP_FIXED DEC(15,2), TYP_FLOAT FLOAT(15) , ");
	lstrcat(create," TYP_DATE DATE, TYP_TIME DATE)");

	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

/* ------------------------------------------------------------------------- */
	/* *** SQLColumns *** */
	/* All qualifiers, all owners, EMPLOYEE table, all columns */
	lstrcpy(tabname,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\t-> Columns information to Table : '%s' \r\n",
			tabname);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLColumns(*phstmt, NULL, 0, NULL, 0, TABLE, SQL_NTS, NULL, 0);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColumns");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "SQLColumns", lpSrvr, 0);/*Output time*/

	if (rc == SQL_SUCCESS) {
	GetTime(&timebuf1);       /* Get start time struct  */

	/* Bind columns in result set to storage locations */

	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szQualifier, STR_LEN,&cbQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szOwner, STR_LEN, &cbOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szColName, STR_LEN, &cbColName);
	SQLBindCol(*phstmt, 5, SQL_C_SHORT, &DataType, 0, &cbDataType);
	SQLBindCol(*phstmt, 6, SQL_C_CHAR, szTypeName, STR_LEN, &cbTypeName);
	SQLBindCol(*phstmt, 7, SQL_C_LONG, &Precision, 0, &cbPrecision);
	SQLBindCol(*phstmt, 8, SQL_C_LONG, &Length, 0, &cbLength);
	SQLBindCol(*phstmt, 9, SQL_C_SHORT, &Scale, 0, &cbScale);
	SQLBindCol(*phstmt, 10, SQL_C_SHORT, &Radix, 0, &cbRadix);
	SQLBindCol(*phstmt, 11, SQL_C_SHORT, &Nullable, 0, &cbNullable);
	SQLBindCol(*phstmt, 12, SQL_C_CHAR, szRemarks, REM_LEN, &cbRemarks);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 12);

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
			szLogPrintf(lpSrvr, FALSE,"'%s',",szQualifier);
			/* *** TABLE_OWNER ----------------------- *** */
			if (cbOwner == SQL_NULL_DATA) lstrcpy(szOwner, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s',",szOwner);
			/* *** TABLE_NAME ------------------------ *** */
			szLogPrintf(lpSrvr, FALSE,"'%s',",szTableName);
			/* *** COLUMN_NAME ----------------------- *** */
			szLogPrintf(lpSrvr, FALSE,"'%s',",szColName);
			/* *** DATA_TYPE ------------------------- *** */
			if (cbDataType == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',",DataType);
			/* *** TYPE_NAME ------------------------- *** */
			szLogPrintf(lpSrvr, FALSE,"'%s',",szTypeName);
			/* *** PRECISION ------------------------- *** */
			if (cbPrecision == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',",Precision);
			/* *** LENGTH ---------------------------- *** */
			if (cbLength == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',",Length);
			/* *** SCALE ----------------------------- *** */
			if (cbScale == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',",Scale);
			/* *** RADIX ----------------------------- *** */
			if (cbRadix == SQL_NULL_DATA)
				szLogPrintf(lpSrvr, FALSE,"'%s',",NULL_VALUE);
			 else   szLogPrintf(lpSrvr, FALSE,"'%d',",Radix);
			/* *** NULLABLE -------------------------- *** */
			Get_pfNullable(Nullable, buffer);
			szLogPrintf(lpSrvr, FALSE,"'%s',",buffer);
			/* *** REMARKS --------------------------- *** */
			if (cbRemarks == SQL_NULL_DATA) lstrcpy(szRemarks, NULL_VALUE);
			szLogPrintf(lpSrvr, FALSE,"'%s' \r\n",szRemarks);

		} else { break; }
	    }
	}
	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "SQLColumns (fetch informations)", lpSrvr, 0);/*Output time*/

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
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
