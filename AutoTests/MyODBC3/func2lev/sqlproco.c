/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLPROCO.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLProcedureColumn
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLPROCEDURECOLUMNS"

#define TABLE "TABSQLPROCCOL"
#define USER1 "root"
#define PROCN "SEL_TEST"

#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
RETCODE lstProcedureColumnsInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLProcedureColumns:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLProcedureColumns(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
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
	/* ***************************************************************** */
	/* *** I. SQLProcedureColumns ************************************** */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLProcedureColumns -> '\r\n");

	/* Get all the procedures columns. */
	rc = SQLProcedureColumns(*phstmt,
			NULL, 0, 			/* Proc qualifier */
			"%", SQL_NTS,			/* Proc owner     */
			"%", SQL_NTS,			/* Proc name      */
			"%", SQL_NTS);			/* Column name    */
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLProcedureColumns");

	if (rc == SQL_SUCCESS) lstProcedureColumnsInfo(phenv, phdbc, phstmt, lpSrvr);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* ***************************************************************** */
	/* *** II. SQLProcedureColumns ************************************* */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tII.) SQLProcedureColumns -> for USER: "USER1" - PROCEDURE: "PROCN"\r\n");

	/* Get all the procedure columns. */
	rc = SQLProcedureColumns(*phstmt,
			NULL, 0, 			/* Proc qualifier */
			USER1, SQL_NTS,			/* Proc owner     */
			PROCN, SQL_NTS,			/* Proc name      */
			"%", SQL_NTS);			/* Columns name   */
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLProcedureColumns");

	if (rc == SQL_SUCCESS) lstProcedureColumnsInfo(phenv, phdbc, phstmt, lpSrvr);

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
RETCODE lstProcedureColumnsInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szProcQualifier[STR_LEN],
		szProcOwner[STR_LEN],
	        szProcName[STR_LEN],
		szColumnName[STR_LEN],
		szTypeName[STR_LEN],
		szRemarks[STR_LEN];
	SWORD   ColumnType,
		DataType,
		Scale,
		Radix,
		Nullable;
	SDWORD  Precision,
		Length;

	UCHAR   szColumnType[STR_LEN],
		szDataType[STR_LEN],
		szPrecision[STR_LEN],
		szLength[STR_LEN],
		szScale[STR_LEN],
		szRadix[STR_LEN],
		szNullable[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SDWORD 	cbProcQualifier, cbProcOwner, cbProcName, cbColumnName,
		cbColumnType, cbDataType, cbTypeName, cbPrecision,
		cbLength, cbScale, cbRadix, cbNullable, cbRemarks;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szProcQualifier, STR_LEN,&cbProcQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szProcOwner, STR_LEN, &cbProcOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szProcName, STR_LEN,&cbProcName);
	SQLBindCol(*phstmt, 4, SQL_C_CHAR, szColumnName, STR_LEN,&cbColumnName);
	SQLBindCol(*phstmt, 5, SQL_C_SHORT, &ColumnType, 0, &cbColumnType);
	SQLBindCol(*phstmt, 6, SQL_C_SHORT, &DataType, 0, &cbDataType);
	SQLBindCol(*phstmt, 7, SQL_C_CHAR, szTypeName, STR_LEN,&cbTypeName);
	SQLBindCol(*phstmt, 8, SQL_C_LONG, &Precision, 0, &cbPrecision);
	SQLBindCol(*phstmt, 9, SQL_C_LONG, &Length, 0, &cbLength);
	SQLBindCol(*phstmt,10, SQL_C_SHORT, &Scale, STR_LEN,&cbScale);
	SQLBindCol(*phstmt,11, SQL_C_SHORT, &Radix, STR_LEN,&cbRadix);
	SQLBindCol(*phstmt,12, SQL_C_SHORT, &Nullable, STR_LEN,&cbNullable);
	SQLBindCol(*phstmt,13, SQL_C_CHAR, szRemarks, STR_LEN,&cbRemarks);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 13);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");*/

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbProcQualifier == SQL_NULL_DATA) lstrcpy(szProcQualifier, NULL_VALUE);
			if (cbProcOwner == SQL_NULL_DATA) lstrcpy(szProcOwner, NULL_VALUE);
			if (cbProcName == SQL_NULL_DATA) lstrcpy(szProcName, NULL_VALUE);
			if (cbColumnName == SQL_NULL_DATA) lstrcpy(szColumnName, NULL_VALUE);
			if (cbDataType == SQL_NULL_DATA) strcpy(szDataType,NULL_VALUE);
			    else sprintf(szDataType,"%d",DataType);
			if (cbTypeName == SQL_NULL_DATA) strcpy(szTypeName,NULL_VALUE);
			if (cbPrecision == SQL_NULL_DATA) strcpy(szPrecision,NULL_VALUE);
			    else sprintf(szPrecision,"%d",Precision);
			if (cbLength == SQL_NULL_DATA) strcpy(szLength,NULL_VALUE);
			    else sprintf(szLength,"%d",Length);
			if (cbScale == SQL_NULL_DATA) strcpy(szScale,NULL_VALUE);
			    else sprintf(szScale,"%d",Scale);
			if (cbRadix == SQL_NULL_DATA) strcpy(szRadix,NULL_VALUE);
			    else sprintf(szRadix,"%d",Radix);
			if (cbRemarks == SQL_NULL_DATA) lstrcpy(szRemarks, NULL_VALUE);
			/*
			if (cbColumnType == SQL_NULL_DATA) ColumnType=NULL;
			if (cbNullable == SQL_NULL_DATA) Nullable=NULL;
			*/
			switch (ColumnType) {
				case SQL_PARAM_TYPE_UNKNOWN	: strcpy(szColumnType,"SQL_PARAM_TYPE_UNKONWN"); break;
				case SQL_PARAM_INPUT		: strcpy(szColumnType,"SQL_PARAM_INPUT"); break;
				case SQL_PARAM_INPUT_OUTPUT	: strcpy(szColumnType,"SQL_PARAN_INPUT_OUTPUT"); break;
				case SQL_PARAM_OUTPUT		: strcpy(szColumnType,"SQL_PARAM_OUTPUT"); break;
				case SQL_RETURN_VALUE		: strcpy(szColumnType,"SQL_RETURN_VALUE"); break;
				case SQL_RESULT_COL		: strcpy(szColumnType,"SQL_RESULT_COL"); break;
				default				: strcpy(szColumnType,NULL_VALUE); break;
			}
			switch (Nullable) {
				case SQL_NO_NULLS		: strcpy(szNullable,"SQL_NO_NULLS"); break;
				case SQL_NULLABLE		: strcpy(szNullable,"SQL_NULLABLE"); break;
				case SQL_NULLABLE_UNKNOWN	: strcpy(szNullable,"SQL_NULLABLE_UNKNOWN"); break;
				default				: strcpy(szNullable,NULL_VALUE); break;
			}
			szLogPrintf(lpSrvr, FALSE,
			  "\tProcedure %d : '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'\r\n",
			  count, szProcQualifier, szProcOwner, szProcName, szColumnName,
			  szColumnType, szDataType, szTypeName, szPrecision,
			  szLength, szScale, szRadix, szNullable, szRemarks);
		} else {
			/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */
			break;
		}
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
