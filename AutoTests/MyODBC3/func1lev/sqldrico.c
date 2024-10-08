/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDRICO.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLDRIVERCONNECT
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLDriverConnect"
#define TABLE ""

#define MAX_LEN  300

#define OPT_DIRECT_CONNECT
#define OPT_DRIVER_CONNECT_NO
#define OPT_USER_CONNECT_NO

#define DCON_DSN "DSN="
#define DCON_UID "UID="
#define DCON_PWD "PWD="
#define DCON_SEP ";"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLDriverConnect:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLDriverConnect(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	HWND			hwnd;
	UCHAR 			szConnStrIn[MAX_LEN];
	SWORD			cbConnStrIn;
	UCHAR			szConnStrOut[MAX_LEN];
	SWORD			cbConnStrOutMax;
	SWORD 			pcbConnStrOut;
	UWORD			fDriverCompletion;

/* ---------------------------------------------------------------------har- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	rc = SQLAllocEnv(phenv);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocEnv");

	rc = SQLAllocConnect(*phenv, phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLAllocConnect");

	/*Set parameter */
	hwnd=lpSrvr->hwnd;
	lstrcpy(szConnStrIn,"");
	cbConnStrIn=SQL_NTS;
	lstrcpy(szConnStrOut,"");
	cbConnStrOutMax=MAX_LEN;
	pcbConnStrOut=0;
	fDriverCompletion= SQL_DRIVER_COMPLETE;
						/* or.  SQL_DRIVER_PROMPT  */
						/* or.  SQL_DRIVER_COMPLETE */
						/* or.  SQL_DRIVER_COMPLETE_REQUIRED */
						/* or.  SQL_DRIVER_NOPROMT */

#ifdef OPT_DIRECT_CONNECT
	lstrcpy(szConnStrIn,DCON_DSN);
	lstrcat(szConnStrIn,lpSrvr->szValidServer0);
	lstrcat(szConnStrIn,DCON_SEP);lstrcat(szConnStrIn,DCON_UID);
	lstrcat(szConnStrIn,lpSrvr->szValidLogin0);
	lstrcat(szConnStrIn,DCON_SEP);lstrcat(szConnStrIn,DCON_PWD);
	lstrcat(szConnStrIn,lpSrvr->szValidPassword0);
#endif

#ifdef OPT_DRIVER_CONNECT
	lstrcpy(szConnStrIn,DCON_DSN);
	lstrcat(szConnStrIn,lpSrvr->szValidServer0);
	lstrcat(szConnStrIn,DCON_SEP);lstrcat(szConnStrIn,DCON_UID);
	lstrcat(szConnStrIn,lpSrvr->szValidLogin0);
#endif
	szLogPrintf(lpSrvr, FALSE,"\t Connection string : '%s' \r\n",szConnStrIn);

	rc = SQLDriverConnect(*phdbc, hwnd, szConnStrIn, cbConnStrIn,
				szConnStrOut, cbConnStrOutMax, &pcbConnStrOut,
				fDriverCompletion);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDriverConnect");

	rc = SQLDisconnect(*phdbc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDisConnect");

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
