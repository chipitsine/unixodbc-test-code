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
** Purpose      : Auto Test DLL, Test von SQLColAttribute
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLColAttribute"

#define SQLSTMT1 "SELECT * FROM DUAL"
#define TABLE "COLATRIBUTE"
#define MAX_NAME_LEN 256
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLColAttribute:
//|
//|
//| Returns:
//*------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLColAttribute(HENV FAR * phenv, HDBC FAR * phdbc,
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
	/* --- Create Table --------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,tabname);
	lstrcat(create," (ID INTEGER, NAME VARCHAR(80), AGE SMALLINT)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname,TABLE);
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," VALUES (10, 'TestName', 40)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt= '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Select Table --------------------------------------------- */
	/* 1. */
	lstrcpy(tabname, TABLE);
	lstrcpy(select,"SELECT * FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- SQLColAttributes ---------------------------------------- */

	sprintf(buffer,"%d",icol);
	szLogPrintf(lpSrvr, FALSE,"\tSQLColAttributes Column: %s -> (ouput) \r\n",buffer);

	/* ***** 1. SQL_COLUMN_AUTO_INCREMNT */
	icol = 1; fDescType = SQL_COLUMN_AUTO_INCREMENT;cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 1.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t1.SQL_COLUMN_AUTO_INCREMT: '%s'\r\n",buffer);

	/* ***** 2. SQL_COLUMN_CASE_SENSITIVE */
	icol = 1; fDescType = SQL_COLUMN_CASE_SENSITIVE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 2.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t2.SQL_COLUMN_CASE_SENSITIVE: '%s'\r\n",buffer);

	/* ***** 3. SQL_COLUMN_COUNT */
	icol = 1; fDescType = SQL_COLUMN_COUNT; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 3.");

	szLogPrintf(lpSrvr, FALSE,"\t3.SQL_COLUMN_COUNT: '%d'\r\n",pfDesc);

	/* ***** 4. SQL_COLUMN_DISPLAY_SIZE */
	icol = 1; fDescType = SQL_COLUMN_DISPLAY_SIZE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 4.");
	szLogPrintf(lpSrvr, FALSE,"\t4.SQL_COLUMN_DISPLAY_SIZE: '%d'\r\n",pfDesc);

	/* ***** 5. SQL_COLUMN_LABEL (ODBC 2.0)*/
	icol = 1; fDescType = SQL_COLUMN_LABEL; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 5.");
	szLogPrintf(lpSrvr, FALSE,"\t5.SQL_COLUMN_LABEL (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* ***** 6. SQL_COLUMN_LENGTH */
	icol = 1; fDescType = SQL_COLUMN_LENGTH; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 6.");
	szLogPrintf(lpSrvr, FALSE,"\t6.SQL_COLUMN_LENGTH: '%d'\r\n",pfDesc);

	/* ***** 7. SQL_COLUMN_MONEY */
	icol = 1; fDescType = SQL_COLUMN_MONEY; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 7.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t7.SQL_COLUMN_MONEY: '%s'\r\n",buffer);

	/* ***** 8. SQL_COLUMN_NAME */
	icol = 1; fDescType = SQL_COLUMN_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 8.");
	szLogPrintf(lpSrvr, FALSE,"\t8.SQL_COLUMN_NAME: '%s'\r\n",rgbDescChar);

	/* ***** 9. SQL_COLUMN_NULLABLE */
	icol = 1; fDescType = SQL_COLUMN_NULLABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 9.");

	Get_pfNullable( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t9.SQL_COLUMN_NULLABLE: '%s'\r\n",buffer);

	/* *****10. SQL_COLUMN_OWNER_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_OWNER_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 10.");
	szLogPrintf(lpSrvr, FALSE,"\t10.SQL_COLUMN_OWNER_NAME (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* *****11. SQL_COLUMN_PRECISION */
	icol = 1; fDescType = SQL_COLUMN_PRECISION; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 11.");
	szLogPrintf(lpSrvr, FALSE,"\t11.SQL_COLUMN_PRECISION: '%d'\r\n",pfDesc);

	/* *****12. SQL_COLUMN_QUALIFIER_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_QUALIFIER_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 12.");
	szLogPrintf(lpSrvr, FALSE,"\t12.SQL_COLUMN_QUALIFIER_NAME (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* *****13. SQL_COLUMN_SCALE */
	icol = 1; fDescType = SQL_COLUMN_SCALE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 13.");
	szLogPrintf(lpSrvr, FALSE,"\t13.SQL_COLUMN_SCALE: '%d'\r\n",pfDesc);

	/* *****14. SQL_COLUMN_SEARCHABLE */
	icol = 1; fDescType = SQL_COLUMN_SEARCHABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 14.");

	Get_Searchable( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t14.SQL_COLUMN_SEARCHABLE: '%s'\r\n",buffer);

	/* *****15. SQL_COLUMN_TABLE_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_TABLE_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
#if (ODBCVER >= 0x0200)
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
#endif
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 15.");
	szLogPrintf(lpSrvr, FALSE,"\t15.SQL_COLUMN_TABLE_NAME (ODBC 2.0): '%s'\r\n",rgbDescChar);

	/* *****16. SQL_COLUMN_TYPE */
	icol = 1; fDescType = SQL_COLUMN_TYPE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 16.");
	szLogPrintf(lpSrvr, FALSE,"\t16.SQL_COLUMN_TYPE: '%d'\r\n",pfDesc);

	/* *****17. SQL_COLUMN_TYPE_NAME */
	icol = 1; fDescType = SQL_COLUMN_TYPE_NAME; cbDescMax=MAX_NAME_LEN;lstrcpy(rgbDescChar,"\0");
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 17.");
	szLogPrintf(lpSrvr, FALSE,"\t17.SQL_COLUMN_TYPE_NAME: '%s'\r\n",rgbDescChar);

	/* *****18. SQL_COLUMN_UNSIGNED */
	icol = 1; fDescType = SQL_COLUMN_UNSIGNED;cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 18.");

	Get_BoolValue( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t18.SQL_COLUMN_UNSIGNED: '%s'\r\n",buffer);

	/* *****19. SQL_COLUMN_UPDATABLE */
	icol = 1; fDescType = SQL_COLUMN_UPDATABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttribute(*phstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLColAttribute 19.");

	Get_Updatable( (SWORD) pfDesc, buffer);
	szLogPrintf(lpSrvr, FALSE,"\t19.SQL_COLUMN_UPDATABLE: '%s'\r\n",buffer);

	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	/* --- Drop Table ----------------------------------------------- */
	lstrcpy(tabname, TABLE);
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

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
