/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : DATATYPE.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von allen Datatypen.
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "DATATYPE"
#define TABLE "DATATYPE1"

#define MAX_NAME_LEN 256
#define MAX_LONG     120

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
SWORD FAR PASCAL DoDatatype(HENV FAR * phenv, HDBC FAR * phdbc,
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

	UCHAR                   value[MAX_LONG],
				rc_value[MAX_LONG];

	SCHAR                   typ_char[MAX_NAME_LEN+1],
				typ_fixed[MAX_NAME_LEN+1],
				typ_date[MAX_NAME_LEN+1],
				typ_time[MAX_NAME_LEN+1];
	SDOUBLE                 typ_float;

	SDWORD                  cbChar,
				cbFloat,
				cbFixed,
				cbDate,
				cbTime,
				cbValue=SQL_NTS;

	SWORD                   i=0;
/* ---------------------------------------------------------------------har- */

	/* lstrcpy(prg_name, TESTNAME); */

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
	lstrcat(create," TYP_FIXED FIXED(15,2), TYP_LONG LONG,");
	lstrcat(create," TYP_DATE DATE, TYP_TIME TIME )");
	/* lstrcat(create," TYP_TIMESTAMP TIMESTAMP )"); */
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_CHAR, TYP_FLOAT, TYP_FIXED,");
	lstrcat(insert," TYP_DATE,TYP_TIME)");
	lstrcat(insert,"VALUES ('Dies ein Datatype-Test.', 123456789,");
	lstrcat(insert," 98765321.99, '1994-10-18', '12:45:38')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* Insert Long */
	lstrcpy(insert,"INSERT INTO ");lstrcat(insert, tabname);
	lstrcat(insert," (TYP_LONG) ");
	lstrcat(insert," VALUES (?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	cbValue = MAX_LONG;
	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			      SQL_LONGVARCHAR, MAX_LONG, 0, value, 0, &cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	lstrcpy(value,"<<< Dies ist eine LONG-Spalte >>>");
	szLogPrintf(lpSrvr, FALSE,
		"\tInsert Value %d.: ->'%s'\r\n",i,value);

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */

	lstrcpy(select,"SELECT ");
	lstrcat(select,"TYP_CHAR, TYP_FLOAT, TYP_FIXED, ");
	lstrcat(select,"TYP_LONG,TYP_DATE, TYP_TIME");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");
	i=0;
	/* Value 1. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 1. (CHAR) --> \r\n");

	rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, typ_char, sizeof(typ_char),
			&cbChar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.1: ->'%s'\r\n",i+1,typ_char);

	/* Value 2. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 2. (FLOAT) --> \r\n");

	rc = SQLGetData(*phstmt, 2, SQL_C_DOUBLE, &typ_float, sizeof(typ_float),
			&cbFloat);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	sprintf(buffer,"%.0f",typ_float);
	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.2: ->'%s'\r\n",i+1,buffer);

	/* Value 3. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 3. (FIXED) --> \r\n");

	rc = SQLGetData(*phstmt,3,SQL_C_DEFAULT,typ_fixed,sizeof(typ_fixed),
			&cbFixed);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.3: ->'%s'\r\n",i+1,typ_fixed);

	/* Value 4. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 4. (LONG) --> \r\n");

	rc = SQLGetData(*phstmt,4,SQL_C_CHAR,rc_value,MAX_LONG,&cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	if (cbValue ==  SQL_NULL_DATA) rc_value[0]='\0';
	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.4: ->'%s'\r\n",i+1,rc_value);

	/* Value 5. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 5. (DATE) --> \r\n");

	rc = SQLGetData(*phstmt,5,SQL_C_CHAR,typ_date,sizeof(typ_date),
			&cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.5: ->'%s'\r\n",i+1,typ_date);

	/* Value 6. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 6. (TIME) --> \r\n");

	rc = SQLGetData(*phstmt,6,SQL_C_CHAR,typ_time,sizeof(typ_time),
			&cbTime);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.6: ->'%s'\r\n",i+1,typ_time);

	rc = SQLFreeStmt(*phstmt, SQL_UNBIND);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* Select Long 2.*/
	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv, *phdbc, *phstmt, lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	/* Value 4. */
	szLogPrintf(lpSrvr, FALSE,"\t Get Column 4. (LONG) --> \r\n");

	cbValue = MAX_NAME_LEN+1;
	rc = SQLGetData(*phstmt,4,SQL_C_CHAR,rc_value,MAX_NAME_LEN,&cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	if (cbValue ==  SQL_NULL_DATA) rc_value[0]='\0';
	szLogPrintf(lpSrvr,FALSE,
		      "\tSelect Values %d. Col.4: ->'%s'\r\n",i+2,rc_value);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt= '%s'\r\n",drop);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif
	/* --- Last Row (SQL_ROLLBACK or SQL_COMMIT) --------------------- */

	/* rc = SQLTransact(*phenv, *phdbc, SQL_ROLLBACK); */
	rc = SQLTransact(*phenv, *phdbc, SQL_COMMIT);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");

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
