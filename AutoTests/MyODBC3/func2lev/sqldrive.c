/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLDRIVE.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLDrivers (Driver Manager)
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLDrivers"

#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLDrivers:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLDrivers(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UWORD	fDirection;

	UCHAR 	szDriverDesc[STR_LEN],
		szDriverAttributes[STR_LEN];

	SWORD 	pcbDriverDesc,
		pcbDrvrAttr;

	/* Declare storage locations for bytes available to return */
	SWORD 	cbDriverDescMax,
		cbDrvrAttrMax;

	SWORD   i=1;
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
	/* ***************************************************************** */
	/* *** I. SQLDrivers *********************************************** */
	/* ***************************************************************** */
	szLogPrintf(lpSrvr, FALSE,"\tI.) SQLDrivers (Driver Manager) -> '\r\n");

	fDirection = SQL_FETCH_FIRST;
	cbDriverDescMax = cbDrvrAttrMax = STR_LEN;
	i=1;
	szLogPrintf(lpSrvr, FALSE,"\t|Nr|DriverDesc|DriverAttributes|Desc Total|Attribute Total|\r\n");

	while (rc != SQL_NO_DATA_FOUND) {
		rc = SQLDrivers(*phenv, fDirection,
				szDriverDesc, cbDriverDescMax, &pcbDriverDesc,
				szDriverAttributes, cbDrvrAttrMax,
				&pcbDrvrAttr);
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDrivers?");

		if (rc != SQL_NO_DATA_FOUND) {
			szLogPrintf(lpSrvr, FALSE,"\t %d. - '%s' - '%s' - ", i,
					szDriverDesc, szDriverAttributes);
			szLogPrintf(lpSrvr, FALSE,"\t '%d' - '%d'\r\n",
					pcbDriverDesc, pcbDrvrAttr);

			fDirection = SQL_FETCH_NEXT; i++;
		}
	}

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
/* ------------------------------------------------------------------------ */
