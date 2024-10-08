/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGETDA.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGETDATA.
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLGetdata"
#define TABLE "GETDATA"

#define MAX_NAME_LEN 256

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSQLGetData:
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetData(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];
	UCHAR                   buffer[MAX_NAME_LEN];

	SCHAR                   rgbValueChar[MAX_NAME_LEN+1],
				rgbValueFixed[MAX_NAME_LEN+1],
				rgbValueDate[MAX_NAME_LEN+1],
				rgbValueTime[MAX_NAME_LEN+1];
	SDOUBLE                 rgbValueFloat;

	UWORD			icol;
	SWORD			fCType;
	SDWORD			pcbValue;
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
	lstrcat(create," (TYP_CHAR CHAR(30), TYP_FLOAT FLOAT(15),");
	lstrcat(create," TYP_FIXED DECIMAL(15,2), TYP_DATE DATE, TYP_TIME TIME )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_CHAR, TYP_FLOAT, TYP_FIXED,");
	lstrcat(insert," TYP_DATE,TYP_TIME)");
	lstrcat(insert,"VALUES ('Dies ein Test.', 123456789,");
	lstrcat(insert," 98765321.99, '1994-12-08', '15:45:38')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT ");
	lstrcat(select,"TYP_CHAR, TYP_FLOAT, TYP_FIXED, ");
	lstrcat(select,"TYP_DATE,\"TYP_TIME\"");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	/* Value 1. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 1. (CHAR) --> \r\n");

	icol=1; fCType = SQL_C_CHAR;
	rc = SQLGetData(*phstmt, icol, fCType, rgbValueChar, sizeof(rgbValueChar),
			&pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values Col.1: ->'%s'\r\n",rgbValueChar);

	/* Value 2. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 2. (FLOAT) --> \r\n");

	icol=2; fCType = SQL_C_DOUBLE;
	rc = SQLGetData(*phstmt, icol, fCType, &rgbValueFloat, sizeof(rgbValueFloat),
			&pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	sprintf(buffer,"%.0f",rgbValueFloat);
	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values Col.2: ->'%s'\r\n",buffer);

	/* Value 3. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 3. (FIXED) --> \r\n");

	icol=3; fCType = SQL_C_DEFAULT;
	rc = SQLGetData(*phstmt,icol,fCType, rgbValueFixed, sizeof(rgbValueFixed),
			&pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values Col.3: ->'%s'\r\n",rgbValueFixed);

	/* Value 4. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 4. (DATE) --> \r\n");

	icol=4; fCType = SQL_C_CHAR;
	rc = SQLGetData(*phstmt,icol, fCType,rgbValueDate,sizeof(rgbValueDate),
			&pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values Col.4: ->'%s'\r\n",rgbValueDate);

	/* Value 5. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 5. (TIME) --> \r\n");

	icol=5; fCType = SQL_C_CHAR;
	rc = SQLGetData(*phstmt, icol, fCType, rgbValueTime, sizeof(rgbValueTime),
			&pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values Col.5: ->'%s'\r\n",rgbValueTime);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

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
