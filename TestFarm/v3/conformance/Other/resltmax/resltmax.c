/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : resltmax.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test: Ergebnismenge auf maximum von Zeilen beschraenken.
**
** Date         : 26.03.96
** l.aenderung  : 13:13pm 03-26-1996 (Harun)
******************************************************************************



    ========== licence begin  GPL
    Copyright (c) 2000-2004 SAP AG

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
    ========== licence end



*/
#include "tst.h"

/* ---------------------------------------------------------------------- */
#define PROGNAME "resltmax"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ODBCUSER1  "ODBC1"
#define ODBCPASSW1 "ODBC1"
#define ODBCTABLE  "ODBCTEST"
#define ODBCSCRIPT "proc2inst"
#define DBPROC1	   "INSERT_TEST"
#define DBPROC2	   "SELECT_TEST"

#define ERROR_CHECK(hstmtx)  \
    if (rc != SQL_SUCCESS) {\
	   SQLError(henv, hdbc, hstmtx, sqlstate, &esq_sql_code, error_txt,\
		    len_error_txt, &used_error_txt);\
	   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
           PRINTF(buffer);\
	   SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
           PRINTF(buffer);\
           switch (rc) {\
		  case(1) :\
			  break;\
		  case(-1) :\
			  break;\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16); \
			  break; } \
	}

 UCHAR sqlstate  [ 10 ];
 SDWORD esq_sql_code;
 UCHAR error_txt  [ 101 ] ;
 SWORD len_error_txt = 100;
 SWORD used_error_txt;
 UCHAR buffer[1024];
/* ----------------------------------------------------------------------- */

#define TESTNAME "ResultsetMax"
#define TABLE "RESULTMAX"

#define MAX_NAME_LEN	512
#define MAX_STR_LEN	30
#define MAX_ROWS_INPUT  20
#define MAX_ROWS	10

#define TXTCOPY "TEST: ResultsetMax"

/* ------------------------------------------------------------------------ */
/*
 -------------------------------------------------------------------------
| DoResultMax:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 -------------------------------------------------------------------------
*/
int main(int argc, argv_t argv)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	HENV henv   = SQL_NULL_HENV;
	HENV FAR * phenv;
	HDBC hdbc   = SQL_NULL_HDBC;
	HDBC FAR * phdbc;
	HSTMT hstmt = SQL_NULL_HSTMT;
	HSTMT FAR * phstmt;

/* ---------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   buf[MAX_NAME_LEN+1];

	UCHAR                   szString[MAX_STR_LEN];
	SWORD			sNumber;
	SQLLEN			cbNumber=SQL_NTS;
	SQLLEN			cbString=SQL_NTS;

	UCHAR			szString_rc[MAX_NAME_LEN];
	SWORD			sNumber_rc;
	SQLLEN			cbNumber_rc = SQL_NTS;
	SQLLEN			cbString_rc = SQL_NTS;

	UDWORD			irow, crow;
	SWORD			cAbort=0;
/* ---------------------------------------------------------------------har- */

 ProcessCmdArguments(argc,argv);

 SPRINTF(buffer, " %s running.\n", PROGNAME);
 PRINTF(buffer);

 phenv = (HENV FAR *) &henv;
 phdbc = (HDBC FAR *) &hdbc;
 phstmt = (HSTMT FAR *) &hstmt;

 rc = SQLAllocEnv(phenv);
 SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocConnect(henv, phdbc);
 SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLConnect(hdbc, DBNODE, SQL_NTS, ODBCUSER, SQL_NTS, ODBCPASSW, SQL_NTS );
 SPRINTF(buffer," SQLConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocStmt (hdbc, phstmt);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

/* --- ResultsetMaximun ------------------------------------------------- */
	/* --- Create Table --------------------------------------------- */
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,TABLE);
	STRCAT(create," ( \"NUM\" SMALLINT, STRING CHAR(30)) ");
	SPRINTF(buffer,"Create Stmt = '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	STRCPY(insert,"INSERT INTO ");STRCAT(insert,TABLE);
	STRCAT(insert," VALUES (?, ?) ");
	SPRINTF(buffer,"Insert Stmt = '%s'"CRLF,insert);
	PRINTF(buffer);

	rc = SQLPrepare(hstmt, insert, SQL_NTS);
	SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_INTEGER, 0, 0, &sNumber, 0, &cbNumber);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
		SQL_CHAR, MAX_STR_LEN, 0, szString, MAX_STR_LEN, &cbString);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	irow=1;
	cbString=SQL_NTS; cbNumber=SQL_NTS;
	SPRINTF(buffer,"Insert Values ->"CRLF);
	PRINTF(buffer);
	while (irow < MAX_ROWS_INPUT+1) {
		sNumber = irow;
		STRCPY(szString,TXTCOPY);
		SPRINTF(buf,"%d.",irow);
		STRCAT(szString,buf);

		rc = SQLExecute(hstmt);
		SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
		/* PRINTF(buffer); */
		ERROR_CHECK(NULL);
		irow++;
	}
	SPRINTF(buffer,"Insert into table (%s) -> %d. Values"CRLF,TABLE,irow-1);
	PRINTF(buffer);

	rc = SQLFreeStmt(hstmt, SQL_DROP);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);
	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	rc = SQLAllocStmt(hdbc, phstmt);
	SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	rc = SQLSetStmtOption(hstmt, SQL_MAX_ROWS, MAX_ROWS);
	SPRINTF(buffer," SQLSetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer," -> Set SQL_MAX_ROWS to : %d"CRLF, MAX_ROWS);
	PRINTF(buffer);

	STRCPY(select,"SELECT ");
	STRCAT(select," \"NUM\", \"STRING\" ");
	STRCAT(select," FROM ");
	STRCAT(select,TABLE);
	SPRINTF(buffer,"Select Stmt= '%s'"CRLF,select);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, select, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	if (rc == SQL_SUCCESS) {
	   rc = SQLBindCol(hstmt, 1, SQL_C_SSHORT, &sNumber_rc, 0, &cbNumber_rc);
	   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	   PRINTF(buffer);
	   ERROR_CHECK(NULL);

	   rc = SQLBindCol(hstmt, 2, SQL_C_CHAR, szString_rc, MAX_STR_LEN, &cbString_rc);
	   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	   PRINTF(buffer);
	   ERROR_CHECK(NULL);

	   irow=1;
	   while(1) {
	      rc = SQLFetch(hstmt);
	      SPRINTF(buffer," SQLFetch retcode: %d"CRLF, rc);
	      PRINTF(buffer);
	      ERROR_CHECK(NULL);

 	      if (rc == SQL_NO_DATA_FOUND) break;

	      SPRINTF(buffer," Row ->  %d  -  %s \n", sNumber_rc, szString_rc);
	      PRINTF(buffer);
	      irow += 1;
	   }
	}
	/* ------------------------------------------------------------------ */
	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Drop Table ----------------------------------------------- */
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,TABLE);
	SPRINTF(buffer,"Drop Stmt= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

/* ------------------------------------------------------------------------- */
  rc = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  rc = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  rc = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  rc = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
/* ------------------------------------------------------------------------- */
