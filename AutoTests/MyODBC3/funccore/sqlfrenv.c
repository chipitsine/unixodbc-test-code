/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLFRENV.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLFREEENV
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funccore.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLFreeEnv"
#define TABLE ""
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLFreeEnv:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLFreeEnv(HENV FAR * phenv, HDBC FAR * phdbc,
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

	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ------------------------------------------------------------------------- */
