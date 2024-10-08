/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLPRMOP.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLParamOptions
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "func2lev.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLPARAMOPTIONS"
#define TABLE "TABPARAMOP"

#define MAX_NAME_LEN 80

#define STR_LEN 128+1
#define REM_LEN 254+1

#define CITY_LEN     30

//*-------------------------------------------------------------------------
//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*---------------------------------------------------------------------------------
//| DoSQLParamOptions:
//|
//|
//| Returns:
//|     Number of Errors or TEST_ABORTED
//*---------------------------------------------------------------------------------
SWORD FAR PASCAL DoSQLParamOptions(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR           create[(MAX_NAME_LEN+1)*2];
	UCHAR           insert[(MAX_NAME_LEN+1)*2];
	UCHAR           select[(MAX_NAME_LEN+1)*2];
	UCHAR           drop[MAX_NAME_LEN+1];
	UCHAR           buffer[MAX_NAME_LEN+1];

	UDWORD 		crow;
	UDWORD 		pirow;

	SDWORD  	cbValue[5];
	UCHAR   	szData[5][CITY_LEN];
	SDWORD  	sNum[5];
	SDOUBLE 	sFloat[5];
	SDOUBLE 	sDouble[5];
	DATE_STRUCT	sDate[5];
	TIME_STRUCT	sTime[5];

	UCHAR   	szData_ret[CITY_LEN];
	SDWORD  	sNum_ret;
	SDOUBLE 	sFloat_ret;
	SDOUBLE 	sDouble_ret;
	DATE_STRUCT	sDate_ret;
	TIME_STRUCT	sTime_ret;
	SDWORD  	pcbValue;
	SWORD   	i=0;
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
	/* --- Create Table  -------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE);
	lstrcat(create," (CITY_NAME CHAR(80), CITY_NUM INTEGER,");
	lstrcat(create," CITY_FLOAT FLOAT(10), CITY_FIXED FIXED(10,2),");
	lstrcat(create," CITY_DATE DATE, CITY_TIME TIME)");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt (Table: "TABLE")= '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	/* --- Insert Table  -------------------------------------------- */
	lstrcpy(insert,"INSERT INTO "TABLE" VALUES (?, ?, ?, ?, ?, ?) ");
	szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt (Table: "TABLE")= '%s'\r\n",insert);

	rc=SQLSetConnectOption(*phdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
  	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLSetConnectOption");

	rc=SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_DEFAULT,
			   	SQL_CHAR, CITY_LEN, 0, szData, 0 , cbValue);
 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc=SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_DEFAULT,
				SQL_INTEGER, 0, 0, &sNum, 0 , cbValue);
 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc=SQLBindParameter(*phstmt, 3, SQL_PARAM_INPUT, SQL_C_DEFAULT,
				SQL_FLOAT, 0, 0, &sFloat, 0 , cbValue);
 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc=SQLBindParameter(*phstmt, 4, SQL_PARAM_INPUT, SQL_C_DEFAULT,
				SQL_DOUBLE, 0, 2, &sDouble, 0 , cbValue);
 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 5, SQL_PARAM_INPUT, SQL_C_DEFAULT,
				SQL_DATE, 0, 0, &sDate, 0, cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc = SQLBindParameter(*phstmt, 6, SQL_PARAM_INPUT, SQL_C_DEFAULT,
				SQL_TIME, 0, 0, &sTime, 0, cbValue);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLBindParameter");

	rc=SQLPrepare(*phstmt, insert, SQL_NTS);
 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLPrepare");

	/* --- Execute 1. ----------------------------------------------- */
	lstrcpy(szData[0],"Boston");  sNum[0]=110;cbValue[0]=SQL_NTS;sFloat[0]=1111111;sDouble[0]=99999999.12;sDate[0].year=1965;sDate[0].month=4; sDate[0].day=2; sTime[0].hour=15;sTime[0].minute=10;sTime[0].second=30;
	lstrcpy(szData[1],"New York");sNum[1]=120;cbValue[1]=SQL_NTS;sFloat[1]=2222222;sDouble[1]=88888888.34;sDate[1].year=1970;sDate[1].month=6; sDate[1].day=12;sTime[1].hour=16;sTime[1].minute=11;sTime[1].second=31;
	lstrcpy(szData[2],"Keokuk");  sNum[2]=130;cbValue[2]=SQL_NTS;sFloat[2]=3333333;sDouble[2]=77777777.56;sDate[2].year=1975;sDate[2].month=8; sDate[2].day=22;sTime[2].hour=17;sTime[2].minute=12;sTime[2].second=32;
	lstrcpy(szData[3],"Seattle"); sNum[3]=140;cbValue[3]=SQL_NTS;sFloat[3]=4444444;sDouble[3]=66666666.78;sDate[3].year=1980;sDate[3].month=10;sDate[3].day=28;sTime[3].hour=18;sTime[3].minute=13;sTime[3].second=33;
	lstrcpy(szData[4],"Eugene");  sNum[4]=150;cbValue[4]=SQL_NTS;sFloat[4]=5555555;sDouble[4]=56565656.91;sDate[4].year=1985;sDate[4].month=12;sDate[4].day=31;sTime[4].hour=19;sTime[4].minute=14;sTime[4].second=34;

	crow=5;
	rc=SQLParamOptions(*phstmt, crow, &pirow);
 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLParamOptions");

	while (TRUE) {
		rc=SQLExecute(*phstmt);
	 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

		szLogPrintf(lpSrvr, FALSE,"\tRow number storage: '%d'\r\n",pirow);

		/* Done if execution was successful */
		if (rc != SQL_ERROR) {
			rc=SQLTransact(*phenv, *phdbc, SQL_COMMIT);
		 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");
			break;
		} else {
			rc=SQLTransact(*phenv, *phdbc, SQL_ROLLBACK);
		 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");
			break;
		}
	 }
	/* --- Execute 2. ----------------------------------------------- */
	lstrcpy(szData[0],"Berlin"); sNum[0]=1060;cbValue[0]=SQL_NTS;sFloat[0]=6666666;sDouble[0]=11111111.98;sDate[0].year=1988;sDate[0].month=3;sDate[0].day=18;sTime[0].hour=21;sTime[0].minute=45;sTime[0].second=50;
	lstrcpy(szData[1],"Hamburg");sNum[1]=1070;cbValue[1]=SQL_NTS;sFloat[1]=7777777;sDouble[1]=22222222.76;sDate[1].year=1989;sDate[1].month=4;sDate[1].day=19;sTime[1].hour=22;sTime[1].minute=46;sTime[1].second=51;
	lstrcpy(szData[2],"Kiel");   sNum[2]=1080;cbValue[2]=SQL_NTS;sFloat[2]=8888888;sDouble[2]=33333333.54;sDate[2].year=1990;sDate[2].month=5;sDate[2].day=20;sTime[2].hour=23;sTime[2].minute=47;sTime[2].second=52;
	lstrcpy(szData[3],"Bonn");   sNum[3]=1090;cbValue[3]=SQL_NTS;sFloat[3]=9999999;sDouble[3]=44444444.32;sDate[3].year=1991;sDate[3].month=6;sDate[3].day=21;sTime[3].hour=24;sTime[3].minute=48;sTime[3].second=53;
	lstrcpy(szData[4],"Koeln");  sNum[4]=1100;cbValue[4]=SQL_NTS;sFloat[4]=1234567;sDouble[4]=55555555.19;sDate[4].year=1992;sDate[4].month=7;sDate[4].day=22;sTime[4].hour=1; sTime[4].minute=49;sTime[4].second=54;

	while (TRUE) {
		rc=SQLExecute(*phstmt);
	 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecute");

		szLogPrintf(lpSrvr, FALSE,"\tRow number storage: '%d'\r\n",pirow);

		/* Done if execution was successful */
		if (rc != SQL_ERROR) {
			rc=SQLTransact(*phenv, *phdbc, SQL_COMMIT);
		 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");
			break;
		} else {
			rc=SQLTransact(*phenv, *phdbc, SQL_ROLLBACK);
		 	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLTransact");
			break;
		}
	 }
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Select Table  -------------------------------------------- */
	lstrcpy(select,"SELECT * FROM ");
	lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt (Table: "TABLE")= '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");

	while(TRUE) {
	  i++;
	  rc = SQLFetch(*phstmt);
	  ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFetch");

	  if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
	    rc = SQLGetData(*phstmt, 1, SQL_C_CHAR, szData_ret, MAX_NAME_LEN, &pcbValue);
 	    ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	    rc = SQLGetData(*phstmt, 2, SQL_C_SLONG, &sNum_ret, 0, &pcbValue);
 	    ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	    rc = SQLGetData(*phstmt, 3, SQL_C_DOUBLE, &sFloat_ret, 0, &pcbValue);
 	    ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	    rc = SQLGetData(*phstmt, 4, SQL_C_DOUBLE, &sDouble_ret, 0, &pcbValue);
 	    ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	    rc = SQLGetData(*phstmt, 5, SQL_C_DATE, &sDate_ret, 0, &pcbValue);
 	    ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");
	    rc = SQLGetData(*phstmt, 6, SQL_C_TIME, &sTime_ret, 0, &pcbValue);
 	    ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLGetData");

	    szLogPrintf(lpSrvr, FALSE,"\tRow %d :       Char  : '%s'\r\n",i,
			szData_ret);
	    szLogPrintf(lpSrvr, FALSE,"\t               Number : '%d'\r\n",
			sNum_ret);
	    sprintf(buffer,"%.0f",sFloat_ret);
	    szLogPrintf(lpSrvr, FALSE,"\t                    Float  : '%s'\r\n",
			buffer);
	    sprintf(buffer,"%.2f",sDouble_ret);
	    szLogPrintf(lpSrvr, FALSE,"\t                 Double : '%s'\r\n",
			buffer);
	    szLogPrintf(lpSrvr, FALSE,"\t                     Date : '%d-%d-%d'\r\n",
			sDate_ret.year, sDate_ret.month, sDate_ret.day);
	    szLogPrintf(lpSrvr, FALSE,"\t                     Time : '%d-%d-%d'\r\n",
			sTime_ret.hour, sTime_ret.minute, sTime_ret.second);
    	  } else { break; }
	}
/* ------------------------------------------------------------------------- */
	rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLFreeStmt");
	/* --- Drop Table ----------------------------------------------- */
#ifdef DROP_TABLE
	/* --- Drop Table  ---------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");
	lstrcat(drop,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt (Table: "TABLE")= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	ERRORCHECK(*phenv,*phdbc,*phstmt,lpSrvr,SQL_SUCCESS,rc,"SQLExecDirect");
#endif
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
