/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLPROC.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLProcedures
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLPROCEDURES"

#define TABLE "TABSQLPROC"
#define USER1 "DOMAIN"

#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------
RETCODE lstProceduresInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLProcedures:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLProcedures(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
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
	/* ***************************************************************** */
	/* *** I. SQLProcedures ******************************************** */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLProcedures -> all procedures'\r\n");

	/* Get all the procedures. */
	/*
	rc = SQLProcedures(*phstmt,
			NULL, 0,                        /* Proc qualifier * /
			"%", SQL_NTS,                   /* Proc owner     * /
			"%", SQL_NTS);                  /* Proc name      * /
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLProcedures");

	if (rc == SQL_SUCCESS) lstProceduresInfo(phenv, phdbc, phstmt, lpSrvr);
	*/
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

	/* ***************************************************************** */
	/* *** II. SQLProcedures ******************************************* */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tII.) SQLProcedures -> all procedures for USER: "USER1"\r\n");

	/* Get all the procedures. */
	/*
	rc = SQLProcedures(*phstmt,
			NULL, 0,                        /* Proc qualifier * /
			USER1, SQL_NTS,                 /* Proc owner     * /
			"%", SQL_NTS);                  /* Proc name      * /
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLProcedures");

	if (rc == SQL_SUCCESS) lstProceduresInfo(phenv, phdbc, phstmt, lpSrvr);
	*/
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

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
RETCODE lstProceduresInfo(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szProcQualifier[STR_LEN],
		szProcOwner[STR_LEN],
		szProcName[STR_LEN],
		szRemarks[STR_LEN];
	SWORD   NumInputParams,
		NumOutputParams,
		NumResultParams,
		ProcedureType;


	UCHAR   szProcedureType[STR_LEN],
		szNumInputParams[STR_LEN],
		szNumOutputParams[STR_LEN],
		szNumResultParams[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SDWORD  cbProcQualifier, cbProcOwner, cbProcName, cbNumInputParams,
		cbNumOutputParams, cbNumResultParams, cbRemarks,
		cbProcedureType;

	SWORD  count=0;
/* ---------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(*phstmt, 1, SQL_C_CHAR, szProcQualifier, STR_LEN,&cbProcQualifier);
	SQLBindCol(*phstmt, 2, SQL_C_CHAR, szProcOwner, STR_LEN, &cbProcOwner);
	SQLBindCol(*phstmt, 3, SQL_C_CHAR, szProcName, STR_LEN,&cbProcName);
	SQLBindCol(*phstmt, 4, SQL_C_SHORT, &NumInputParams, 0, &cbNumInputParams);
	SQLBindCol(*phstmt, 5, SQL_C_SHORT, &NumOutputParams, 0, &cbNumOutputParams);
	SQLBindCol(*phstmt, 6, SQL_C_SHORT, &NumResultParams, 0, &cbNumResultParams);
	SQLBindCol(*phstmt, 7, SQL_C_CHAR, szRemarks, STR_LEN,&cbRemarks);
	SQLBindCol(*phstmt, 8, SQL_C_SHORT, &ProcedureType, 0, &cbProcedureType);

	rc = lst_ColumnNames(phenv, phdbc, phstmt, lpSrvr, 8);

	while(TRUE) {
		count++;

		rc = SQLFetch(*phstmt);
		/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");*/

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbProcQualifier == SQL_NULL_DATA) lstrcpy(szProcQualifier, NULL_VALUE);
			if (cbProcOwner == SQL_NULL_DATA) lstrcpy(szProcOwner, NULL_VALUE);
			if (cbProcName == SQL_NULL_DATA) lstrcpy(szProcName, NULL_VALUE);
			if (cbNumInputParams == SQL_NULL_DATA) strcpy(szNumInputParams,NULL_VALUE);
			    else sprintf(szNumInputParams,"%d",NumInputParams);
			if (cbNumOutputParams == SQL_NULL_DATA) strcpy(szNumOutputParams,NULL_VALUE);
			    else sprintf(szNumOutputParams,"%d",NumOutputParams);
			if (cbNumResultParams == SQL_NULL_DATA) strcpy(szNumResultParams,NULL_VALUE);
			    else sprintf(szNumResultParams,"%d",NumResultParams);
			if (cbRemarks == SQL_NULL_DATA) lstrcpy(szRemarks, NULL_VALUE);
			/*
			if (cbProcedureType == SQL_NULL_DATA) ProcedureType=NULL;
			*/
			switch (ProcedureType) {
				case SQL_PT_UNKNOWN     : strcpy(szProcedureType,"SQL_PT_UNKONWN"); break;
				case SQL_PT_PROCEDURE   : strcpy(szProcedureType,"SQL_PT_PROCEDURE"); break;
				case SQL_PT_FUNCTION    : strcpy(szProcedureType,"SQL_PT_FUNCTION"); break;
				default                 : strcpy(szProcedureType,NULL_VALUE); break;
			}

			szLogPrintf(lpSrvr, FALSE,
			  "\tProcedure %d : '%s','%s','%s','%s','%s','%s','%s','%s'\r\n",
			  count, szProcQualifier, szProcOwner, szProcName, szNumInputParams,
			  szNumOutputParams, szNumResultParams, szRemarks, szProcedureType);

		} else {
			/* ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch"); */
			break;
		}
	    }
	return TRUE;
}
/* ------------------------------------------------------------------------- */
