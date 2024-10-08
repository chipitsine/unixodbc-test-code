/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : LONGVARA.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL,Einfgen von Daten (LONG-Varialen) 100K
**
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "LONGVARSA"
#define TABLE "TABLONGA"

#define MAX_NAME_LEN 50
#define MAX_GET_LONG 60

#define DATA_SIZE 64001
#define DATA_GET  28000
#define MAX_INSERT 16
//*-------------------------------------------------------------------------

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoLongvarsa:
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoLongDatatypeA(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];

	PTR			pToken;

	UWORD 			i;
	SDWORD 			data_len;
	SDWORD 			cbDataLen;
	SDWORD	  		received;
  	UDWORD			size=0;

	SWORD 			sqltype;
	UDWORD 			sqllen;
  	SWORD 			sqlscale;
  	SWORD 			nullable;

 	HGLOBAL			hglb;
  	UCHAR FAR		*data;
  	UCHAR FAR		*rc_data;

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
	lstrcat(create," (ALONG LONG ASCII, ID INTEGER)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," VALUES (?, 0)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt = '%s'\r\n",insert);

	hglb = GlobalAlloc(GPTR, DATA_SIZE);
	data = GlobalLock(hglb);

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

  	rc = SQLDescribeParam(*phstmt, 1, &sqltype, &sqllen, &sqlscale, &nullable);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLDescribeParam");

  	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                sqltype, sqllen, sqlscale, data, 0, &cbDataLen);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBinParameter");

	cbDataLen = SQL_DATA_AT_EXEC;
	rc = SQLExecute(*phstmt);
	if ((rc != SQL_SUCCESS) && (rc != SQL_NEED_DATA))
		ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	if  (rc == SQL_NEED_DATA) {
	  	hglb = GlobalAlloc(GPTR, DATA_SIZE);
	  	data = GlobalLock(hglb);

		/* ----- Fill buffer ----- */
	  	for (i=0; i < DATA_SIZE; i++)
	    		data[i] = i % 10 + '0';
	  	data_len = DATA_SIZE-1;
	  	data[data_len] = 0;

		rc = SQLParamData(*phstmt, &pToken);
		if ((rc != SQL_SUCCESS) && (rc != SQL_NEED_DATA))
			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLParamData");

	  	for (i=0; i < MAX_INSERT; i++) {
     			rc = SQLPutData(*phstmt, data, data_len);
     			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPutData");

			szLogPrintf(lpSrvr, FALSE,"\t--> Input data:%d (len:%d)\r\n",i+1,lstrlen(data));
     			size += lstrlen(data);
  		}
		szLogPrintf(lpSrvr, FALSE,"\t--> Input size: %d\r\n",size);
/* --- mysql ???
		rc = SQLParamData(*phstmt, &pToken);
		if ((rc != SQL_SUCCESS) && (rc != SQL_NEED_DATA))
			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLParamData");
*/
	}

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	GlobalUnlock(hglb);
	GlobalFree(hglb);

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT ");
	lstrcat(select,"ALONG ");
	lstrcat(select," FROM ");lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	szLogPrintf(lpSrvr, FALSE,"\tSelect Value --> \r\n");

	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv, *phdbc, *phstmt, lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	hglb    = GlobalAlloc(GPTR, DATA_SIZE);
	rc_data = GlobalLock(hglb);

	i=1; size=0;
	while (1) {
		rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, rc_data, DATA_GET+1, &received);
		if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
			ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

		if (rc == SQL_NO_DATA_FOUND) break;

		szLogPrintf(lpSrvr, FALSE,"\t%d. --> Ouput data: %d (received=%d)\r\n",
		i, lstrlen(rc_data), received);
		i++; size += lstrlen(rc_data);
	}
	szLogPrintf(lpSrvr, FALSE,"\t--> Output size: %d\r\n",size);

	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");

	GlobalUnlock(hglb);
	GlobalFree(hglb);

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
