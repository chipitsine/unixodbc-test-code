/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : TIMESTAMP.C
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
#define TESTNAME "TIMESTAMP"
#define TABLE "TIMESTM1"

#define MAX_NAME_LEN 50
#define VALUE_NULL "<NULL>"
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoTimestamp:
//|     This test case will test the fixed numbers write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoTimestamp(HENV FAR * phenv, HDBC FAR * phdbc,
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

	TIMESTAMP_STRUCT	typ_timestamp,
				rc_typ_timestamp;
	UCHAR                   rcValue[MAX_NAME_LEN];

	SDWORD                  cbTimestamp=SQL_NTS;

	SWORD                   i=0;
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
	lstrcat(create," ( TYP_TIMESTAMP TIMESTAMP )");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=0;
	/* --- Insert Table --------------------------------------------- */
	/* --- Insert TIMESTAMP 1. as CHAR ---*/
	i++;
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_TIMESTAMP) ");
 	lstrcat(insert,"VALUES ({ts '1994-12-20 13:02:15.9999'})");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt %d.= '%s'\r\n",i,insert);

	szLogPrintf(lpSrvr, FALSE,
		"\tInsert Value %d. (CHAR) : ->'1994-12-20 13:02:15.9999'\r\n",i);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert TIMESTAMP 2. as CHAR ---*/
	i++;
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_TIMESTAMP) ");
	lstrcat(insert,"VALUES ('1994-12-20 13:02:15.999')");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt %d. = '%s'\r\n",i,insert);

	szLogPrintf(lpSrvr, FALSE,
	  "\tInsert Value %d. (CHAR) : ->'1994-12-20 13:02:15.999'\r\n",i);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert TIMESTAMP 3. as TIMESTAMP ---*/
	i++;
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_TIMESTAMP) ");
	lstrcat(insert,"VALUES (?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt %d. = '%s'\r\n",i,insert);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_TIMESTAMP,
			      SQL_TIMESTAMP, 0, 0, &typ_timestamp, 0, &cbTimestamp);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	typ_timestamp.year=1994;typ_timestamp.month=12;typ_timestamp.day=20;
	typ_timestamp.hour=13;typ_timestamp.minute=02;typ_timestamp.second=15;
	typ_timestamp.fraction=999999;
	szLogPrintf(lpSrvr, FALSE,
		"\tInsert Value %d. (TIMESTAMP) : ->'%d'-'%d'-'%d' '%d':'%d':'%d'.'%ld'\r\n",i,
	  	typ_timestamp.year, typ_timestamp.month, typ_timestamp.day,
		typ_timestamp.hour, typ_timestamp.minute, typ_timestamp.second,
		typ_timestamp.fraction);

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */

	lstrcpy(select,"SELECT ");
	lstrcat(select," TYP_TIMESTAMP ");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=0;
	/* --- 1. --- */
	/* Get 1. timestamp value as CHAR datatype */
	i++;
	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv, *phdbc, *phstmt, lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	cbTimestamp = SQL_NTS;
	rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, &rcValue, MAX_NAME_LEN,
			&cbTimestamp);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr, FALSE,"\t Get row %d. (CHAR) --> \r\n",i);
	if (cbTimestamp ==  SQL_NULL_DATA) lstrcpy(rcValue, VALUE_NULL);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Value %d.: ->'%s'\r\n",i, rcValue);

	/* --- 2. --- */
	/* Get 1. timestamp value as CHAR datatype */
	i++;
	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv, *phdbc, *phstmt, lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	cbTimestamp = SQL_NTS;
	rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, &rcValue, MAX_NAME_LEN,
			&cbTimestamp);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr, FALSE,"\t Get row %d. (CHAR) --> \r\n",i);
	if (cbTimestamp ==  SQL_NULL_DATA) lstrcpy(rcValue, VALUE_NULL);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Value %d.: ->'%s'\r\n",i, rcValue);

	/* --- 3. --- */
	/* Get 3. timestamp value as TIMESTAMP datatype */
	i++;
	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv, *phdbc, *phstmt, lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	cbTimestamp = SQL_NTS;
	rc = SQLGetData(*phstmt, 1, SQL_C_TIMESTAMP, &rc_typ_timestamp, 0,
			&cbTimestamp);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	szLogPrintf(lpSrvr, FALSE,"\t Get row %d. (TIMESTAMP) --> \r\n",i);
	if (cbTimestamp ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	szLogPrintf(lpSrvr, FALSE,
		"\tSelect Value %d.: ->'%d'-'%d'-'%d' '%d':'%d':'%d'.'%ld'\r\n",i,
	  	rc_typ_timestamp.year, rc_typ_timestamp.month, rc_typ_timestamp.day,
		rc_typ_timestamp.hour, rc_typ_timestamp.minute, rc_typ_timestamp.second,
		rc_typ_timestamp.fraction);

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
