/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLPARAM.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLPARAMDATA.
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLParamData"
#define TABLE "SPARAMDAT"

#define MAX_NAME_LEN 50

#define PARAM1 1
#define PARAM2 2
#define PARAM3 3
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSQLParamData:
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLParamData(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];

	SDWORD			cbValue;
	PTR			pToken;
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

/* ---------------------------------------------------------------------har- */

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname,TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (NAME CHAR(30), AGE SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (NAME, AGE) ");
	lstrcat(insert," VALUES (?, ?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	/* ----------------------------------------------------------------- */
	/* Specify data types and declare params as SQL_DATA_AT_EXEC for     */
	/* Name, Age, Date						     */

	cbValue = SQL_DATA_AT_EXEC;

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
				MAX_NAME_LEN, 0, (SWORD FAR *) PARAM1, 0, &cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT,
				SQL_SMALLINT, 0, 0, (SWORD FAR *) PARAM2, 0, &cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	if  (rc == SQL_NEED_DATA) {
		/* Call SQLParamData to begin SQL_DATA_AT_EXEC parameter     */
		/* processing and retrieve pToken for first SQL_DATA_AT_EXEC */
		/* parameter */

		rc = SQLParamData(*phstmt, &pToken);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLParamData");

		while  (rc == SQL_NEED_DATA) {
			/* Call SQParamData to declare all data has been sent for    */
			/* this SQL_DATA_AT_EXEC parameter, retrieve pToken for next */
			/* SQL_DATA_AT_EXEC parameter (if one exists), and get return*/
			/* code to determine if another SQL_DATA_AT_EXEC paramter exits */

			rc = SQLParamData(*phstmt, &pToken);
			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLParamData");
		}
	}

	/* --- Drop Table ----------------------------------------------- */
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
/* ************************************************************************* */
