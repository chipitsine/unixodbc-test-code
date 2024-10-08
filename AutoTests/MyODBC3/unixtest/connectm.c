/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : CONNECTM.C
**
** Purpose      : Auto Test DLL, Test von CONNECT-MIX
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "unixtest.h"

/* ------------------------------------------------------------------------- */
#define TESTNAME "CONNECTM"
#define TABLE ""

#define MAX_CONNECTIONS 16

#define MAX_NAME_LEN 50
#define STRING_LEN 10

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoConnectMix:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoConnectMix(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                           sErr=0;
/* ------------------------------------------------------------------------- */
	HDBC 			hdbc[MAX_CONNECTIONS];

	SWORD			i;
/* ---------------------------------------------------------------------har- */

	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	/* ***** SQLAllocEnv */
	rc = SQLAllocEnv(phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocEnv");

	/* ***** SQLConnect */
	i = 2;
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLConnect");

  	i = 0;
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLConnect");

  	i = 1;
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLConnect");

	/* ***** SQLDisconnect */
	i = 2;
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	i = 3;
  	rc = SQLAllocConnect(*phenv, &hdbc[i]);
  	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	rc = SQLConnect(hdbc[i], lpSrvr->szValidServer0, SQL_NTS,
			lpSrvr->szValidLogin0, SQL_NTS,
			lpSrvr->szValidPassword0, SQL_NTS);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLConnect");
  	i = 0;
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	i = 1;
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	i = 3;
	rc = SQLDisconnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisconnect");

	/* ***** SQLFreeConnect */
	i = 2;
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	i = 1;
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	i = 0;
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	i = 3;
	rc = SQLFreeConnect(hdbc[i]);
	ERRORCHECK(*phenv,hdbc[i],*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeConnect");

	/* ***** SQLFreeEnv */
	rc = SQLFreeEnv(*phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeEnv");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}
/* ************************************************************************* */

