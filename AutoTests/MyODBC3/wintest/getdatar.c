/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : GETDATAR.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGetData bis SQL_NO_DATA_FOUND
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "wintest.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLGETDATAEND"
#define TABLE "GETDATA2"

#define MAX_NAME_LEN 50
#define MAX_STMT  254

#define MAX_LONG 1024
#define MAX_CHAR 254
#define MIN_CHAR 20

#define VALUE_NULL "<NULL>"
//*-------------------------------------------------------------------------

int PrintString(UCHAR *, DWORD, UCHAR *, lpSERVERINFO);

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************

//*---------------------------------------------------------------------------------
//| DoSQLGetDataR:
//|     This test case will test the fixed numbers write and read from the
//|     database.
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoGetDataR(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_STMT];
	UCHAR                   insert[MAX_STMT];
	UCHAR                   select[MAX_STMT];
	UCHAR                   drop[MAX_STMT];

	UCHAR                   tabname[MAX_NAME_LEN];
	UCHAR                   buffer[MAX_NAME_LEN];

	UCHAR                   typ_char[MAX_NAME_LEN],
				rc_typ_char[MAX_NAME_LEN];
	SDWORD                  cbchar=SQL_NTS;

	DATE_STRUCT		typ_date,
				rc_typ_date;
	SDWORD                  cbDate=SQL_NTS;

	TIME_STRUCT		typ_time,
				rc_typ_time;
	SDWORD                  cbTime=SQL_NTS;

	SDWORD                  num,
				rc_typ_int;
	SDWORD                  cbnum=SQL_NTS;

	SFLOAT                  fnum,
				rc_typ_float;
	SDWORD                  cbfnum=SQL_NTS;

	SDOUBLE                 dnum,
				rc_typ_double;
	SDWORD                  cbdnum=SQL_NTS;

	HGLOBAL                 hglb;
	UCHAR FAR               *lvalue,
				*rc_typ_long;
	SDWORD                  cblvalue=SQL_NTS;

	SWORD                   i=0,j;

	struct _timeb           timebuf1, timebuf2;
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
	lstrcat(create," ( TYP_CHAR CHAR(254), TYP_INT INTEGER, TYP_DATE DATE, TYP_FLOAT FLOAT, TYP_FIXED DEC(15,2), TYP_LONG LONG, TYP_TIME TIME)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=0;
	/* --- Insert Table --------------------------------------------- */
	/* --- Insert DATE 1. as CHAR ---*/
	i++;
	lstrcpy(insert,"INSERT INTO ");
	lstrcat(insert,tabname);
	lstrcat(insert," (TYP_CHAR, TYP_INT, TYP_DATE, TYP_FLOAT, TYP_FIXED, TYP_LONG, TYP_TIME) ");
	lstrcat(insert,"VALUES (?,?,?,?,?,?,?)");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt %d. = '%s'\r\n",i,insert);

	hglb = GlobalAlloc(GPTR, MAX_LONG);
	lvalue = GlobalLock(hglb);

	lstrcpy(typ_char,"kkllmmnnoopprrssuuvvwwvvzz");
	typ_date.year=1994;typ_date.month=12;typ_date.day=20;
	typ_time.hour=13; typ_time.minute=24; typ_time.second=10;
	num=1234567; fnum= (SFLOAT) 4455667; dnum=223377889911.88;
	sprintf(buffer,"%.0f - 4.)'%.2f ",fnum,dnum); j=1; lstrcpy(lvalue,">");
	while (j < 11) {
	 lstrcat(lvalue,"0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
	 j++;
	}
	szLogPrintf(lpSrvr, FALSE,
		"\tInsert Stmt %d. : ->1.)'%s'- 2.)'%ld' - 3.)'%d'.'%d'.'%d' - 4.)'%s' 6.)'<LONG>' 7.)'%d':'%d':'%d'\r\n",i,typ_char,num,
	  	typ_date.year, typ_date.month, typ_date.day, buffer,
	        typ_time.hour, typ_time.minute, typ_time.second);

	GetTime(&timebuf1);       /* Get start time struct  */

	rc = SQLPrepare(*phstmt, insert, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
				SQL_CHAR, MAX_NAME_LEN, 0, typ_char, 0, &cbchar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG,
				SQL_INTEGER, 0, 0, &num, 0, &cbnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_DATE,
			      SQL_DATE, 0, 0, &typ_date, 0, &cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT,
				SQL_FLOAT, 0, 0, &fnum, 0, &cbfnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 5, SQL_PARAM_INPUT, SQL_C_DOUBLE,
				SQL_DOUBLE, 0, 0, &dnum, 0, &cbdnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR,
				SQL_LONGVARCHAR, MAX_LONG, 0, lvalue, 0, &cblvalue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 7, SQL_PARAM_INPUT, SQL_C_TIME,
			      SQL_TIME, 0, 0, &typ_time, 0, &cbTime);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLExecute(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Insert Stmt 1.(BindParam)", lpSrvr, 0);/*Output time*/

	GlobalUnlock(hglb);
	GlobalFree(hglb);
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	lstrcpy(select,"SELECT ");
	lstrcat(select," TYP_CHAR, TYP_INT, TYP_DATE, TYP_FLOAT, TYP_FIXED, TYP_LONG, TYP_TIME ");
	lstrcat(select," FROM ");
	lstrcat(select,tabname);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	i=0;
	/* --- 1. --- */
	/* Get 1. date value as CHAR datatype */
	i++;
	rc = SQLFetch(*phstmt);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

/* --- 2.1. CHAR --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbchar = SQL_NTS;
	rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, rc_typ_char, (MIN_CHAR)+1, &cbchar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 1.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row CHAR as CHAR", lpSrvr, 0);/*Output time*/

	if (cbchar ==  SQL_NULL_DATA) lstrcpy(rc_typ_char, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.1.: ->'%s'\r\n",i, rc_typ_char);

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbchar = SQL_NTS;
	rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, rc_typ_char, (MIN_CHAR)+1, &cbchar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 1.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row CHAR as CHAR", lpSrvr, 0);/*Output time*/

	if (cbchar ==  SQL_NULL_DATA) lstrcpy(rc_typ_char, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.1.: ->'%s'\r\n",i, rc_typ_char);

	/* -- GetData 3. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbchar = SQL_NTS;
	rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, rc_typ_char, (MIN_CHAR)+1, &cbchar);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 1.3.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row CHAR as CHAR", lpSrvr, 0);/*Output time*/

	if (cbchar ==  SQL_NULL_DATA) lstrcpy(rc_typ_char, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	 szLogPrintf(lpSrvr, FALSE,"\tValue %d.1.: ->'%s'\r\n",i, rc_typ_char);

/* --- 2.2. INTEGER --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbnum = SQL_NTS;
	rc = SQLGetData(*phstmt, 2, SQL_C_LONG, &rc_typ_int, 0,&cbnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 2.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row INTEGER as LONG", lpSrvr, 0);/*Output time*/

	if (cbnum ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
 	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.2.: ->'%ld'\r\n",i,rc_typ_int);

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbnum = SQL_NTS;
	rc = SQLGetData(*phstmt, 2, SQL_C_LONG, &rc_typ_int, 0,&cbnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 2.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row INTEGER as LONG", lpSrvr, 0);/*Output time*/

	if (cbnum ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
          szLogPrintf(lpSrvr, FALSE,"\tValue %d.2.: ->'%ld'\r\n",i,rc_typ_int);

/* --- 2.3. DATE --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbDate = SQL_NTS;
	rc = SQLGetData(*phstmt, 3, SQL_C_DATE, &rc_typ_date, 0,&cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 3.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row DATE as DATE_STRUCT", lpSrvr, 0);/*Output time*/

	if (cbDate ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
 	  szLogPrintf(lpSrvr, FALSE,
		"\tValue %d.3.: ->'%d'-'%d'-'%d'\r\n",i,
	  	rc_typ_date.year, rc_typ_date.month, rc_typ_date.day);

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbDate = SQL_NTS;
	rc = SQLGetData(*phstmt, 3, SQL_C_DATE, &rc_typ_date, 0,&cbDate);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 3.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row DATE as DATE_STRUCT", lpSrvr, 0);/*Output time*/

	if (cbDate ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,
		"\tValue %d.3.: ->'%d'-'%d'-'%d'\r\n",i,
	  	rc_typ_date.year, rc_typ_date.month, rc_typ_date.day);

/* --- 2.4. FLOAT --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbfnum = SQL_NTS;
	rc = SQLGetData(*phstmt, 4, SQL_C_FLOAT, &rc_typ_float, 0,&cbfnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 4.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row FLOAT as FLOAT", lpSrvr, 0);/*Output time*/

	sprintf(buffer,"%.0f",rc_typ_float);
	if (cbfnum ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.4.: ->'%s'\r\n",i,buffer);

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbfnum = SQL_NTS;
	rc = SQLGetData(*phstmt, 4, SQL_C_FLOAT, &rc_typ_float, 0,&cbfnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 4.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row FLOAT as FLOAT", lpSrvr, 0);/*Output time*/

	sprintf(buffer,"%.0f",rc_typ_float);
	if (cbfnum ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.4.: ->'%s'\r\n",i,buffer);

/* --- 2.5. FIXED --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbdnum = SQL_NTS;
	rc = SQLGetData(*phstmt, 5, SQL_C_DOUBLE, &rc_typ_double, 0,&cbdnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 5.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row FIXED as DOUBLE", lpSrvr, 0);/*Output time*/

	sprintf(buffer,"%.2f",rc_typ_double);
	if (cbdnum ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.5.: ->'%s'\r\n",i,buffer);

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbdnum = SQL_NTS;
	rc = SQLGetData(*phstmt, 5, SQL_C_DOUBLE, &rc_typ_double, 0,&cbdnum);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 5.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row FIXED as DOUBLE", lpSrvr, 0);/*Output time*/

	sprintf(buffer,"%.2f",rc_typ_double);
	if (cbdnum ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,"\tValue %d.5.: ->'%s'\r\n",i,buffer);

/* --- 2.6. LONG --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	hglb = GlobalAlloc(GPTR, MAX_LONG);
	rc_typ_long = GlobalLock(hglb);

	cblvalue = SQL_NTS;
	rc = SQLGetData(*phstmt, 6, SQL_C_CHAR, rc_typ_long, MAX_LONG/2, &cblvalue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 6.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row LONG as CHAR", lpSrvr, 0);/*Output time*/

	if (cblvalue ==  SQL_NULL_DATA) lstrcpy(rc_typ_long, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND) {
		sprintf(buffer, "\tValue %d.6.: -> ", i);
		PrintString(rc_typ_long, lstrlen(rc_typ_long), buffer, lpSrvr);
	}

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cblvalue = SQL_NTS;
	rc = SQLGetData(*phstmt, 6, SQL_C_CHAR, rc_typ_long, MAX_LONG/2, &cblvalue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 6.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row LONG as CHAR", lpSrvr, 0);/*Output time*/

	if (cblvalue ==  SQL_NULL_DATA) lstrcpy(rc_typ_long, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND) {
		sprintf(buffer, "\tValue %d.6.: -> ", i);
		PrintString(rc_typ_long, lstrlen(rc_typ_long), buffer, lpSrvr);
	}
	/* -- GetData 3. -- */
	GetTime(&timebuf1);       /* Get start time struct  */


	cblvalue = SQL_NTS;
	rc = SQLGetData(*phstmt, 6, SQL_C_CHAR, rc_typ_long, MAX_LONG/2, &cblvalue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 6.3.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row LONG as CHAR", lpSrvr, 0);/*Output time*/

	if (cblvalue ==  SQL_NULL_DATA) lstrcpy(rc_typ_long, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND) {
		sprintf(buffer, "\tValue %d.6.: -> ", i);
		PrintString(rc_typ_long, lstrlen(rc_typ_long), buffer, lpSrvr);
	}

	GlobalUnlock(hglb);
	GlobalFree(hglb);
/* --- 2.7. TIME --- */
	/* -- GetData 1. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbTime = SQL_NTS;
	rc = SQLGetData(*phstmt, 7, SQL_C_TIME, &rc_typ_time, 0,&cbTime);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 7.1.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row TIME as TIME_STRUCT", lpSrvr, 0);/*Output time*/

	if (cbDate ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
 	  szLogPrintf(lpSrvr, FALSE,
		"\tValue %d.7.: ->'%d'-'%d'-'%d'\r\n",i,
	  	rc_typ_time.hour, rc_typ_time.minute, rc_typ_time.second);

	/* -- GetData 2. -- */
	GetTime(&timebuf1);       /* Get start time struct  */

	cbTime = SQL_NTS;
	rc = SQLGetData(*phstmt, 7, SQL_C_TIME, &rc_typ_time, 0,&cbTime);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData 7.2.");

	GetTime(&timebuf2);       /* Get finish time struct */

	CheckTime(timebuf1, timebuf2, "Get row TIME as TIME_STRUCT", lpSrvr, 0);/*Output time*/

	if (cbDate ==  SQL_NULL_DATA) lstrcpy(buffer, VALUE_NULL);
	if (rc != SQL_NO_DATA_FOUND)
	  szLogPrintf(lpSrvr, FALSE,
		"\tValue %d.7.: ->'%d'-'%d'-'%d'\r\n",i,
	  	rc_typ_time.hour, rc_typ_time.minute, rc_typ_time.second);

	/* -------------------------------------------------------------- */

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
