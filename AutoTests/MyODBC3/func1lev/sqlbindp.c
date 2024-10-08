/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLBINDP.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLBINDPARAMETER
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func1lev.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLBindParameter"
#define TABLE "BINBPARAM"

#define MAX_NAME_LEN 256
#define MAX_SCALE 2

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSQLBindParameter:
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLBindParameter(HENV FAR * phenv, HDBC FAR * phdbc,
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

	SCHAR                   rgbValueChar[MAX_NAME_LEN+1];
	SWORD                   rgbValueSmallint;
	SDWORD                  rgbValueInteger;
	SFLOAT			rgbValueFloat;
	SDOUBLE			rgbValueDouble;
	DATE_STRUCT		rgbValueDate;
	TIME_STRUCT             rgbValueTime;

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
	lstrcat(create," (TYP_CHAR CHAR(30), TYP_SMALLINT SMALLINT, TYP_INTEGER INTEGER,");
	lstrcat(create," TYP_FLOAT FLOAT, TYP_DOUBLE DECIMAL(13,2), TYP_DATE DATE, TYP_TIME TIME) ");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_CHAR, TYP_SMALLINT, TYP_INTEGER, TYP_FLOAT, TYP_DOUBLE, TYP_DATE, TYP_TIME )");
	lstrcat(insert,"VALUES (?, ?, ?, ?, ?, ?, ?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	/* *** SQLBindParameter -------------------------------------------- */
	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_CHAR, MAX_NAME_LEN, 0, rgbValueChar, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_CHAR)");

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_SMALLINT, 0, 0, &rgbValueSmallint, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_SMALLINT)");

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG,
			SQL_INTEGER, 0, 0, &rgbValueInteger, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_INTEGER)");

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT,
			SQL_FLOAT, 0, 0, &rgbValueFloat, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_FLOAT)");

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 5, SQL_PARAM_INPUT, SQL_C_DOUBLE,
			SQL_DOUBLE, 0, MAX_SCALE, &rgbValueDouble, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_DOUBLE)");

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 6, SQL_PARAM_INPUT, SQL_C_DATE,
			SQL_DATE, 0, 0, &rgbValueDate, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_DATE)");

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(*phstmt, 7, SQL_PARAM_INPUT, SQL_C_TIME,
			SQL_TIME, 0, 0, &rgbValueTime, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter (SQL_TIME)");

	/* *** Set values -------------------------------------------------- */
	lstrcpy(rgbValueChar,"Testname");
	rgbValueSmallint = 44;
	rgbValueInteger = 1234567890;
	rgbValueFloat    = (SFLOAT) 1234;
	rgbValueDouble   = 1234567890.12;
	rgbValueDate.year=1994;rgbValueDate.month=12;rgbValueDate.day=8;
	rgbValueTime.hour=11;rgbValueTime.minute=55;rgbValueTime.second=30;

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	lstrcpy(rgbValueChar,"---");
	rgbValueSmallint = 0;
	rgbValueInteger  = 0;
	rgbValueFloat    = (SFLOAT) 0;
	rgbValueDouble   = 0;
	rgbValueDate.year=0;rgbValueDate.month=0;rgbValueDate.day=0;
	rgbValueTime.hour=0;rgbValueTime.minute=0;rgbValueTime.second=0;

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT ");
	lstrcat(select,"TYP_CHAR, TYP_SMALLINT, TYP_INTEGER, TYP_FLOAT, TYP_DOUBLE, TYP_DATE, TYP_TIME ");
	lstrcat(select," FROM ");lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 1, SQL_C_CHAR, rgbValueChar, MAX_NAME_LEN, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_CHAR)");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 2, SQL_C_SHORT, &rgbValueSmallint, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_SMALLINT)");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 3, SQL_C_LONG, &rgbValueInteger, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_INTEGER)");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 4, SQL_C_FLOAT, &rgbValueFloat, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_FLOAT)");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 5, SQL_C_DOUBLE, &rgbValueDouble, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_DOUBLE)");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 6, SQL_C_DATE, &rgbValueDate, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_DATE)");

	pcbValue = SQL_NTS;
	rc = SQLBindCol(*phstmt, 7, SQL_C_TIME, &rgbValueTime, 0, &pcbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindCol (SQL_TIME)");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	szLogPrintf(lpSrvr, FALSE,"\tSelect Values --> \r\n");
	szLogPrintf(lpSrvr, FALSE,"\trgbValueChar         = '%s'\r\n",rgbValueChar);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueSmallint     = '%d'\r\n",rgbValueSmallint);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueInteger      = '%ld'\r\n",rgbValueInteger);
	sprintf(buffer,"%.0f",rgbValueFloat);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueFloat        = '%s'\r\n",buffer);
	sprintf(buffer,"%.2f",rgbValueDouble);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueDouble       = '%s'\r\n",buffer);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueDate(Year)   = '%d'\r\n",rgbValueDate.year);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueDate(Month)  = '%d'\r\n",rgbValueDate.month);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueDate(Day)    = '%d'\r\n",rgbValueDate.day);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueTime(Hour)   = '%d'\r\n",rgbValueTime.hour);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueTime(Minute) = '%d'\r\n",rgbValueTime.minute);
	szLogPrintf(lpSrvr, FALSE,"\trgbValueTime(second) = '%d'\r\n",rgbValueTime.second);

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
