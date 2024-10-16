/*



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
#define PROGNAME "SUBTRANS"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFSIZE 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
			   PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
                  PRINTF(buffer);\
		  SPRINTF(buffer," error_txt: %s"CRLF, error_txt);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ return(16); }\
		     break;\
		  default   :\
		       if (STRCMP(sqlstate,"07005"))\
		       if (STRCMP(sqlstate,"24000"))\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [BUFSIZE];
#define MAX_COL 30
#define LONGVARCHAR_LEN 65000
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
UWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
UDWORD pcbColDef[MAX_COL];
SWORD testlen, hostvarlen;
SCHAR *ptr, c;

#define CHARCOL_LEN 11
UCHAR szCHARCOL[CHARCOL_LEN];
UCHAR FAR *pHostVar;
#define INTCOL_LEN sizeof(sINTCOL)
SDWORD sINTCOL;
SQLLEN cbCHARCOL, cbINTCOL;
SDWORD cbHOSTVAR;

UCHAR szCHAR[11];
double  sDOUBLE;
double  sDECIMAL;
SDWORD cbDOUBLE, cbCHAR, cbDECIMAL;

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT hstmt1 = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code=0;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  RETCODE retcode;

  UDWORD vAutoCommit=1;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer,PROGNAME" running."CRLF);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer," SQLAllocEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  for (;;) {
     retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 varchar (200))", SQL_NTS);
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

     retcode = SQLExecDirect (hstmt, "commit work", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "BEGIN TRANSACTION", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "insert "TABLE" values ('row1')", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "ROLLBACK TRANSACTION", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "select * from "TABLE, SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFetch (hstmt);
     SPRINTF(buffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szCHARCOL, sizeof(szCHARCOL), &cbCHARCOL);
     SPRINTF(buffer," SQLGetData retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     SPRINTF(buffer, "'%s':%d"CRLF, szCHARCOL, cbCHARCOL);
     PRINTF(buffer);

     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt1);

     retcode = SQLExecDirect (hstmt, "begin tran", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "SUBTRANS begin", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "insert "TABLE" values ('row1')", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "COMMIT transact", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLPrepare (hstmt, "SUBTRANS end", SQL_NTS);
     SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecute (hstmt);
     SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "select * from "TABLE, SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFetch (hstmt);
     SPRINTF(buffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szCHARCOL, sizeof(szCHARCOL), &cbCHARCOL);
     SPRINTF(buffer," SQLGetData retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     SPRINTF(buffer, "'%s':%d"CRLF, szCHARCOL, cbCHARCOL);
     PRINTF(buffer);

     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     if (!vAutoCommit) {
	break;
     }

     vAutoCommit = 0;
     retcode = SQLSetConnectOption (hdbc, SQL_AUTOCOMMIT, vAutoCommit);
     SPRINTF(buffer," SQLSetConnectOption (SQL_AUTOCOMMIT=%d) retcode: %d"CRLF, vAutoCommit, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

  }
  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt (SQL_DROP) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt1);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

