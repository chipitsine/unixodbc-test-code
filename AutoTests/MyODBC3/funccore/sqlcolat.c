/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLCOLAT.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLCOLATTRIBUTES
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funccore.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLColAttributes"
#define TABLE "COLATRIBUTES"
#define MAX_NAME_LEN 256

//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLColAttributes:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLColAttributes(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	UWORD			icol=1;
	UWORD                   fDescType;
	PTR                     rgbDesc;
	UCHAR                   rgbDescChar[MAX_NAME_LEN];
	SWORD			cbDescMax;
	SWORD			pcbDesc;
	SDWORD			pfDesc;
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

	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (ID INTEGER, NAME VARCHAR(80), AGE SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname,TABLE);
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," VALUES (10, 'TestName', 40)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Select Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname, TABLE);
	lstrcpy(select,"SELECT * FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- SQLColAttributes ---------------------------------------- */

	sprintf(buffer,"%d",icol);
	szLogPrintf(lpSrvr, FALSE,"\tSQLColAttributes Column: %s -> (ouput) \r\n",buffer);

	/* ***** 1. SQL_COLUMN_AUTO_INCREMNT */
	icol = 1; fDescType = SQL_COLUMN_AUTO_INCREMENT;cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 1.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t1.SQL_COLUMN_AUTO_INCREMT: '%s'\r\n",buffer);

	/* ***** 2. SQL_COLUMN_CASE_SENSITIVE */
	icol = 1; fDescType = SQL_COLUMN_CASE_SENSITIVE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 2.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t2.SQL_COLUMN_CASE_SENSITIVE: '%s'\r\n",buffer);

	/* ***** 3. SQL_COLUMN_COUNT */
	icol = 1; fDescType = SQL_COLUMN_COUNT; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 3.");

	szLogPrintf(lpSrvr, FALSE,"\t3.SQL_COLUMN_COUNT: '%d'\r\n",pfDesc);

	/* ***** 4. SQL_COLUMN_DISPLAY_SIZE */
	icol = 1; fDescType = SQL_COLUMN_DISPLAY_SIZE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 4.");
	szLogPrintf(lpSrvr, FALSE,"\t4.SQL_COLUMN_DISPLAY_SIZE: '%d'\r\n",pfDesc);

	/* ***** 5. SQL_COLUMN_LABEL (ODBC 2.0)*/
	icol = 1; fDescType = SQL_COLUMN_LABEL; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 5.");
	szLogPrintf(lpSrvr, FALSE,"\t5.SQL_COLUMN_LABEL (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* ***** 6. SQL_COLUMN_LENGTH */
	icol = 1; fDescType = SQL_COLUMN_LENGTH; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 6.");
	szLogPrintf(lpSrvr, FALSE,"\t6.SQL_COLUMN_LENGTH: '%d'\r\n",pfDesc);

	/* ***** 7. SQL_COLUMN_MONEY */
	icol = 1; fDescType = SQL_COLUMN_MONEY; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 7.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t7.SQL_COLUMN_MONEY: '%s'\r\n",buffer);

	/* ***** 8. SQL_COLUMN_NAME */
	icol = 1; fDescType = SQL_COLUMN_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 8.");
	szLogPrintf(lpSrvr, FALSE,"\t8.SQL_COLUMN_NAME: '%s'\r\n",rgbDescChar);

	/* ***** 9. SQL_COLUMN_NULLABLE */
	icol = 1; fDescType = SQL_COLUMN_NULLABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 9.");

	Get_pfNullable( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t9.SQL_COLUMN_NULLABLE: '%s'\r\n",buffer);

	/* *****10. SQL_COLUMN_OWNER_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_OWNER_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 10.");
	szLogPrintf(lpSrvr, FALSE,"\t10.SQL_COLUMN_OWNER_NAME (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* *****11. SQL_COLUMN_PRECISION */
	icol = 1; fDescType = SQL_COLUMN_PRECISION; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 11.");
	szLogPrintf(lpSrvr, FALSE,"\t11.SQL_COLUMN_PRECISION: '%d'\r\n",pfDesc);

	/* *****12. SQL_COLUMN_QUALIFIER_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_QUALIFIER_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 12.");
	szLogPrintf(lpSrvr, FALSE,"\t12.SQL_COLUMN_QUALIFIER_NAME (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* *****13. SQL_COLUMN_SCALE */
	icol = 1; fDescType = SQL_COLUMN_SCALE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 13.");
	szLogPrintf(lpSrvr, FALSE,"\t13.SQL_COLUMN_SCALE: '%d'\r\n",pfDesc);

	/* *****14. SQL_COLUMN_SEARCHABLE */
	icol = 1; fDescType = SQL_COLUMN_SEARCHABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 14.");

	Get_Searchable( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t14.SQL_COLUMN_SEARCHABLE: '%s'\r\n",buffer);

	/* *****15. SQL_COLUMN_TABLE_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_TABLE_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 15.");
	szLogPrintf(lpSrvr, FALSE,"\t15.SQL_COLUMN_TABLE_NAME (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* *****16. SQL_COLUMN_TYPE */
	icol = 1; fDescType = SQL_COLUMN_TYPE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 16.");
	szLogPrintf(lpSrvr, FALSE,"\t16.SQL_COLUMN_TYPE: '%d'\r\n",pfDesc);

	/* *****17. SQL_COLUMN_TYPE_NAME */
	icol = 1; fDescType = SQL_COLUMN_TYPE_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 17.");
	szLogPrintf(lpSrvr, FALSE,"\t17.SQL_COLUMN_TYPE_NAME: '%s'\r\n",rgbDescChar);

	/* *****18. SQL_COLUMN_UNSIGNED */
	icol = 1; fDescType = SQL_COLUMN_UNSIGNED;cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 18.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t18.SQL_COLUMN_UNSIGNED: '%s'\r\n",buffer);

	/* *****19. SQL_COLUMN_UPDATABLE */
	icol = 1; fDescType = SQL_COLUMN_UPDATABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLColAttributes 19.");

	Get_Updatable( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t19.SQL_COLUMN_UPDATABLE: '%s'\r\n",buffer);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
/* ---------------------------------------------------------------------har- */
	rc = SQLFreeStmt(*phstmt, SQL_DROP);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

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
