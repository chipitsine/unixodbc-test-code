/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGETFN.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGetFunctions
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLGetFunctions"

#define TABLE	""
#define SQLSTMT1 "SELECT * FROM DUAL"
#define MAX_NAME_LEN 50

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLGetFunctions:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetFunctions(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UWORD			fFunction;
	UWORD			pfExists[100];

        UCHAR                   buffer[MAX_NAME_LEN];

/* ------------------------------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	/* - Connect ------------------------------------------------------- */
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

	/* ----------------------------------------------------------------- */

/* --- SQLGetFunctions ----------------------------------------------------- */

	fFunction = SQL_API_ALL_FUNCTIONS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetFunctions");

/* --- Output -------------------------------------------------------------- */

        szLogPrintf(lpSrvr, FALSE,"\tConformance Core --> \r\n");

	/* --- Core Conformance -------------------------------------------- */
        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLALLOCCONNECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLALLOCCONNECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLALLOCENV], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLALLOCENV = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLALLOCSTMT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLALLOCSTMT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLBINDCOL], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLBINDCOL = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLCANCEL], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLCANCEL = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLCOLATTRIBUTES], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLCOLATTRIBUTES = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLCONNECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLCONNECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLDESCRIBECOL], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLDESCRIBECOL = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLDISCONNECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLDISCONNECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLERROR], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLERROR = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLEXECDIRECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLEXECDIRECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLFETCH], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLFETCH = '%s'\r\n",buffer);

	Get_BoolValue( (SWORD) pfExists[SQL_API_SQLFREECONNECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLFREECONNECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLFREEENV], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLFREEENV = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLFREESTMT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLFREESTMT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETCURSORNAME], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETCURSORNAME = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLNUMRESULTCOLS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLNUMRESULTCOLS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPREPARE], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPREPARE = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLROWCOUNT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLROWCOUNT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSETCURSORNAME], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETCURSORNAME = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSETPARAM], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETPARAM = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLTRANSACT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLTRANSACT = '%s'\r\n",buffer);

	/* ----------------------------------------------------------------- */
        szLogPrintf(lpSrvr, FALSE,"\tConformance Level 1. --> \r\n");

	/* --- Level 1 Conformance ----------------------------------------- */
	Get_BoolValue( (SWORD) pfExists[SQL_API_SQLBINDPARAMETER], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLBINDPARAMETER = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLCOLUMNS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLCOLUMNS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLDRIVERCONNECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLDRIVERCONNECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETCONNECTOPTION], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETCONNECTOPTION = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETDATA], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETDATA = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETFUNCTIONS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETFUNCTIONS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETINFO], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETINFO = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETSTMTOPTION], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETSTMTOPTION = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLGETTYPEINFO], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLGETTYPEINFO = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPARAMDATA], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPARAMDATA = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPUTDATA], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPUTDATA = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSETCONNECTOPTION], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETCONNECTOPTION = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSETSTMTOPTION], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETSTMTOPTION = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSPECIALCOLUMNS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSPECIALCOLUMNS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSTATISTICS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSTATISTICS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLTABLES], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLTABLES = '%s'\r\n",buffer);

	/* ----------------------------------------------------------------- */
        szLogPrintf(lpSrvr, FALSE,"\tConformance Level 2. --> \r\n");

	/* --- Level 2 Conformance ----------------------------------------- */
        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLBROWSECONNECT], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLBROWSECONNECT = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLCOLUMNPRIVILEGES], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLCOLUMNPRIVILEGES = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLDATASOURCES], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLDATASOURCES = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLDESCRIBEPARAM], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLDESCRIBEPARAM = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLDRIVERS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLDRIVERS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLEXTENDEDFETCH], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLEXTENDEDFETCH = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLFOREIGNKEYS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLFOREIGNKEYS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLMORERESULTS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLMORERESULTS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLNATIVESQL], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLNATIVESQL = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLNUMPARAMS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLNUMPARAMS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPARAMOPTIONS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPARAMOPTIONS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPRIMARYKEYS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPRIMARYKEYS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPROCEDURECOLUMNS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPROCEDURECOLUMNS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLPROCEDURES], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLPROCEDURES = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSETPOS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETPOS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLSETSCROLLOPTIONS], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETSCROLLOPTIONS = '%s'\r\n",buffer);

        Get_BoolValue( (SWORD) pfExists[SQL_API_SQLTABLEPRIVILEGES], buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLTABLEPRIVILEGES = '%s'\r\n",buffer);

/* --- SQLGetFunctions 2. -------------------------------------------------- */

	fFunction = SQL_API_SQLFETCH;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetFunctions");

/* --- Output -------------------------------------------------------------- */

        Get_BoolValue( (SWORD) *pfExists, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLFETCH = '%s'\r\n",buffer);

/* --- SQLGetFunctions 3. -------------------------------------------------- */

	fFunction = SQL_API_SQLSETPOS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetFunctions");

/* --- Output -------------------------------------------------------------- */

        Get_BoolValue( (SWORD) *pfExists, buffer);
        szLogPrintf(lpSrvr, FALSE,"\tSQL_API_SQLSETPOS = '%s'\r\n",buffer);


	/* - Disconnect ---------------------------------------------------- */
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
