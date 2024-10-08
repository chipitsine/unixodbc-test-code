/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLNUMCS.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQL_NUMERIC_STRUCT (getdata)
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLNUMERICSTRUCTS"

#define STR_LEN			128+1
#define MAX_NAME_LEN		50
#define MAX_COL_NAME_LEN 	18

#define NULL_VALUE "<NULL>"
#define MAX_INSERT 256
#define VALUE_ADD  1111.98

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
/*
 ------------------------------------------------------------------------
| DoSQLNumericStruct:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 ------------------------------------------------------------------------
*/
SWORD FAR PASCAL DoSQLNumericStructS(HENV FAR * phenv, HDBC FAR * phdbc,
				  HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
    RETCODE rc;
    SWORD   sErr=0;
/* ------------------------------------------------------------------------- */
    UCHAR   buffer[512];
    UCHAR   stmt[256];
    SDWORD  local_value;
    SDOUBLE local_double;
    UCHAR   local_str[SQL_MAX_NUMERIC_LEN];
    SWORD   xi, ix, len;
    UDWORD  iValue;
    SDWORD  pcbValue;

    SQL_NUMERIC_STRUCT local_numeric;
/* ------------------------------------------------------------------------- */
    char *sqltable  = "NUMCSTRUCTS";
    char *sqlcreate = "create table %s (keyx int, numv decimal(10,3))";
    char *sqlinsert = "insert into %s values (%d, %f)";
    char *sqlselect = "select keyx, numv from %s";
    char *sqldrop   = "drop table %s";
/* ------------------------------------------------------------------------- */
    /* --- Connect DB ----------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
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

    /* --- Create Table -------------------------------------------------- */
    sprintf(stmt, sqlcreate, sqltable);
    szLogPrintf(lpSrvr,FALSE,"\t Create Table (%s)\r\n",stmt);

    rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

    /* --- Insert into Table --------------------------------------------- */
    local_double = 0;
    for (ix=0; ix < MAX_INSERT; ix ++) {

      /* Test spez.: local_double=10.001; */
      /* Insert positive value */
      sprintf(stmt, sqlinsert, sqltable, ix+1, local_double);
      szLogPrintf(lpSrvr,FALSE,"\t Insert Table %d (%s)\r\n",ix+1,stmt);

      rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
      DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

      /* Insert negative value */
      sprintf(stmt, sqlinsert, sqltable, ix+1, (local_double*(-1)));
      szLogPrintf(lpSrvr,FALSE,"\t Insert Table %d (%s)\r\n",ix+1,stmt);

      rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
      DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

      local_double += VALUE_ADD;
    }

    rc = SQLFreeStmt(*phstmt, SQL_DROP);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

  /* --- Select Table --------------------------------------------------- */
  /* --- Select table Data SQL_NUMERIC_STRUCT from table ---------------- */
    rc = SQLAllocStmt(*phdbc, phstmt);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocStmt");

    sprintf(stmt, sqlselect, sqltable);
    szLogPrintf(lpSrvr,FALSE,"\t Select Table (%s)\r\n",stmt);

    rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

    szLogPrintf(lpSrvr,FALSE,"\t Fetch Table \r\n");

    rc=SQL_SUCCESS;
    while (rc != SQL_NO_DATA_FOUND) {
      rc = SQLFetch(*phstmt);
      DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetch");

      if (rc != SQL_NO_DATA_FOUND) {
	rc = SQLGetData(*phstmt, (UWORD)1, SQL_C_ULONG, &iValue, sizeof(iValue),
			&pcbValue);
	if (rc != SQL_SUCCESS) {
          DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetData");
	}
	rc = SQLGetData(*phstmt, (UWORD)2, SQL_C_NUMERIC, &local_numeric,
			sizeof(SQL_NUMERIC_STRUCT), &pcbValue);
	if (rc != SQL_SUCCESS) {
          DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetData");
	}
        /* convert SQL_NUMERIC_STRUCT to local_value */
	local_value = 0;
        for (xi=0; xi < sizeof(local_value); xi++) {
	   local_value += ((unsigned char) local_numeric.val[xi] << (xi*8));
	}
        if (local_numeric.sign == 0) {
	       sprintf(local_str,"-%d",local_value);
	} else sprintf(local_str,"%d",local_value);
        if (local_numeric.scale > 0) {
		len = local_numeric.precision - local_numeric.scale;
		for (xi=local_numeric.precision; xi > len;xi--)
			local_str[xi+1] = local_str[xi];
		local_str[xi+1] = '.';
		local_str[local_numeric.precision+1]='\0';
	}

        /* Output SQL_NUMERIC_STRUCT value */
	sprintf(buffer, "keyx: %d - numv: %s (prec:%d scale:%d sign:%d)",
	       iValue, local_str, local_numeric.precision,
	       local_numeric.scale, local_numeric.sign);
        szLogPrintf(lpSrvr,FALSE,"\t%s\r\n",buffer);
      }
    }

  rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

  /* --- Drop Table --------------------------------------------------- */
  sprintf(stmt, sqldrop, sqltable);
  sprintf(buffer,"Drop Table (%s)\n", stmt);
  szLogPrintf(lpSrvr,FALSE,"\t Drop Table (%s)\r\n",stmt);

  rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

  rc = SQLFreeStmt(*phstmt, SQL_DROP);
  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

  /* --- Disconnect -------------------------------------------------- */
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
