/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : DBPROC.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von DB-Proceduren
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "DBProcedure"
#define TABLENAME "DOMAIN.ODBCTEST"

#define OWNER	   	"DOMAIN"
#define PROGRAMM   	"ODBCTEST"
#define DBPROC_INS 	"INSERT_TEST"
#define DBPROC_SEL 	"SELECT_TEST"
#define DBPROC_INS_ODBC "INS_TEST"
#define DBPROC_SEL_ODBC "SEL_TEST"


#define MAX_NAME_LEN 50
#define NAME_LEN 60
#define DATE_TIME_LEN 12
#define REM_LEN 128

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoDatatype:
//|     This test case will test the fixed numbers write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoDBProc(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   procedure[MAX_NAME_LEN+1];
	UCHAR                   delete[MAX_NAME_LEN+1];

	SWORD                   i=0;

	UCHAR                   szName[NAME_LEN];
	UCHAR                   szDate[NAME_LEN];
	UCHAR                   szTime[NAME_LEN];
	SDWORD                  sFloat;
	SDWORD		        sFixed;
	SDWORD                  cbName=SQL_NTS,
				cbFixed=SQL_NTS,
				cbFloat=SQL_NTS,
				cbDate=SQL_NTS,
				cbTime=SQL_NTS;
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
	/* Call DBProcedure - Insert*/

	/* Call 1 */
/* --- mysql ????
	lstrcpy(procedure,"{ CALL "OWNER"."PROGRAMM"."DBPROC_INS" (?,?,?) }");
	szLogPrintf(lpSrvr, FALSE,"\tDBProc 1. Stmt = '%s'\r\n",procedure);

	rc =  SQLPrepare(*phstmt, procedure, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	/* Define parameter for return value (Employee ID) from procedure. * /
	SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
			 NAME_LEN, 0, szName, 0, &cbName);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_DOUBLE,
			 0, 0, &sFixed, 0, &cbFixed);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_DOUBLE,
			 0, 0, &sFloat, 0, &cbFloat);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	lstrcpy(szName,"TESTPROC");
	sFixed = 40;
	sFloat = 99;
	szLogPrintf(lpSrvr, FALSE,"\tInsert 1. values = \r\n");
	szLogPrintf(lpSrvr, FALSE,"\t1.) %s \r\n",szName);
	szLogPrintf(lpSrvr, FALSE,"\t2.) %d \r\n",sFixed);
	szLogPrintf(lpSrvr, FALSE,"\t3.) %d \r\n",sFloat);


	rc =  SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");
*/
	/* Call 2 */
/* --- mysql ????
	lstrcpy(procedure,"{ CALL "OWNER"."DBPROC_INS_ODBC" (?,?,?) }");
	szLogPrintf(lpSrvr, FALSE,"\tDBProc 1. Stmt = '%s'\r\n",procedure);

	rc =  SQLPrepare(*phstmt, procedure, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	lstrcpy(szName,"TEST_DB");
	sFixed = 23;
	sFloat = 76;
	szLogPrintf(lpSrvr, FALSE,"\tInsert 2. values = \r\n");
	szLogPrintf(lpSrvr, FALSE,"\t1.) %s \r\n",szName);
	szLogPrintf(lpSrvr, FALSE,"\t2.) %d \r\n",sFixed);
	szLogPrintf(lpSrvr, FALSE,"\t3.) %d \r\n",sFloat);

	rc =  SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");
*/
	/* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */

	/* rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK); */
	rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --------------------------------------------------------------- */

	/* Call DBProcedure - Select*/

	/* Call 1. */
/* --- mysql ????
	lstrcpy(procedure,"{ CALL "OWNER"."PROGRAMM"."DBPROC_SEL" (?,?,?,?,?) }");
	szLogPrintf(lpSrvr, FALSE,"\tDBProc 1. Stmt = '%s'\r\n",procedure);

	rc =  SQLPrepare(*phstmt, procedure, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	cbName = cbFixed = cbFloat = cbDate = cbTime = SQL_NTS;

	SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
			 NAME_LEN, 0, szName, 0, &cbName);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (IN)");

	SQLBindParameter(*phstmt, 2, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_DOUBLE,
			 5, 0, &sFixed, 0, &cbFixed);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (OUT)");

	SQLBindParameter(*phstmt, 3, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_DOUBLE,
			 5, 2, &sFloat, 0, &cbFloat);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (OUT)");

	cbDate = cbTime = DATE_TIME_LEN;

	SQLBindParameter(*phstmt, 4, SQL_PARAM_OUTPUT, SQL_C_CHAR, SQL_DATE,
			 NAME_LEN, 0, szDate, NAME_LEN, &cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (OUT)");

	SQLBindParameter(*phstmt, 5, SQL_PARAM_OUTPUT, SQL_C_CHAR, SQL_TIME,
			 NAME_LEN, 0, szTime, NAME_LEN, &cbTime);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (OUT)");

	lstrcpy(szName,"TEST"); sFixed=0; sFloat=0;

	rc =  SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	szLogPrintf(lpSrvr, FALSE,"\tSelect 1. values =\r\n");
	szLogPrintf(lpSrvr, FALSE,"\t1.) %s \r\n",szName);
	szLogPrintf(lpSrvr, FALSE,"\t2.) %d \r\n",sFixed);
	szLogPrintf(lpSrvr, FALSE,"\t3.) %d \r\n",sFloat);
	szLogPrintf(lpSrvr, FALSE,"\t4.) %s \r\n",szDate);
	szLogPrintf(lpSrvr, FALSE,"\t5.) %s \r\n",szTime);
*/
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* Call 2. */
/* --- mysql ????
	lstrcpy(procedure,"{ CALL "OWNER"."DBPROC_SEL_ODBC" (?,?,?,?,?) }");
	szLogPrintf(lpSrvr, FALSE,"\tDBProc 1. Stmt = '%s'\r\n",procedure);

	rc =  SQLPrepare(*phstmt, procedure, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	lstrcpy(szName,"ADABAS_D"); sFixed=0; sFloat=0;

	rc =  SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	szLogPrintf(lpSrvr, FALSE,"\tSelect 2. values =\r\n");
	szLogPrintf(lpSrvr, FALSE,"\t1.) %s \r\n",szName);
	szLogPrintf(lpSrvr, FALSE,"\t2.) %d \r\n",sFixed);
	szLogPrintf(lpSrvr, FALSE,"\t3.) %d \r\n",sFloat);
	szLogPrintf(lpSrvr, FALSE,"\t4.) %s \r\n",szDate);
	szLogPrintf(lpSrvr, FALSE,"\t5.) %s \r\n",szTime);
*/
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --------------------------------------------------------------- */
/*
	lstrcpy(delete,"DELETE "); lstrcat(delete,TABLENAME);
	szLogPrintf(lpSrvr, FALSE,"\tDelete Stmt = '%s'\r\n",delete);

	rc =  SQLExecDirect(*phstmt, delete, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
*/
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

