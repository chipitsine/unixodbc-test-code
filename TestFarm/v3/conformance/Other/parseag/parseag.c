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
#include "tst.h"

#define PROGNAME "PARSEAG"
#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define CMD_NO_RESULT (1)
#define CMD_RESULT (2)

#if UNIXODBC_TEST_SAP_TEST_MODE
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			        SPRINTF(buffer, " sqlstate: %s\n", sqlstate);\
		                PRINTF(buffer);\
				SPRINTF(buffer," esq_sql_code: %d\n", esq_sql_code);\
		                PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		          if (esq_sql_code != -6000 &&\
			      esq_sql_code != -4004 &&\
			      esq_sql_code != 1051 ) \
			  exit(16);\
			  break; } }
#else
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
                           SQLError(henv,\
                                    hdbc,\
                                    hstmtx,\
                                    sqlstate,\
                                    &esq_sql_code,\
                                    error_txt,\
                                    len_error_txt,\
                                    &used_error_txt);\
			        SPRINTF(buffer, " sqlstate: %s\n", sqlstate);\
		                PRINTF(buffer);\
				SPRINTF(buffer," esq_sql_code: %d\n", esq_sql_code);\
		                PRINTF(buffer);\
                  switch (retcode) {\
                  case(100) :\
                          break;\
                  default:\
                     if (strcmp(sqlstate,"S0002") )\
                          return(16);\
                          break; } }

#endif

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define C1_LEN 101
#define MAX_STMT 20
UCHAR szC1[C1_LEN];
SQLLEN cbF1, cbC1;
#define C_LEN 18

UCHAR szCURSOR[C_LEN+1];
SWORD cbCURSOR;
UWORD i;

RETCODE command ( HENV henv, HDBC hdbc, UCHAR *stmt, SWORD cmd_mode, SDWORD stmt_len);

int main(int argc, argv_t argv)
{
  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC hdbc2   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  UCHAR stmt [ 3000 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  RETCODE retcode;
  UCHAR szChar[50];
  SQLLEN cbValue;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc2);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
     ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc2, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
		       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  strcpy(stmt, "DROP TABLE "TABLE);
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);

  strcpy(stmt, "create table "TABLE" ( c1 char (50))");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);

  strcpy(stmt, "insert "TABLE" values ( '12345')");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc2, &hstmt2);
  SPRINTF(buffer," SQLAllocStmt (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "select c1 from "TABLE" where c1 = ?");
  retcode = SQLPrepare (hstmt, stmt, STRLEN(stmt));
  ERROR_CHECK(hstmt);

  cbValue = SQL_NTS;
  STRCPY(szChar, "12345");
  retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			     SQL_CHAR, 50, 0, szChar, 0, &cbValue);
  SPRINTF(buffer," SQLBindParameter retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(stmt, "alter table "TABLE" add c2 char (50)");
  retcode = SQLExecDirect (hstmt2, stmt, STRLEN(stmt));
  SPRINTF(buffer," SQLExecDirect (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt2);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szC1,
     sizeof(szC1), &cbC1);
  SPRINTF(buffer,"'%s':%d"CRLF, szC1, cbC1);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt2, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt2);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLTransact(henv, hdbc2, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc2);
  SPRINTF(buffer," SQLDisconnect (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc2);
  SPRINTF(buffer," SQLFreeConnect (2) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer, PROGNAME" ending."CRLF);
  PRINTF(buffer);

  return(0);
}

RETCODE command ( HENV henv,
HDBC hdbc,
UCHAR *stmt,
SWORD cmd_mode,
SDWORD stmt_len)
{
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  phstmt = (HSTMT FAR *) &hstmt;

  retcode = SQLAllocStmt (hdbc, phstmt);

  SPRINTF(buffer,"%s"CRLF, stmt);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  if (retcode == SQL_SUCCESS) {
     retcode = SQLExecDirect (hstmt, stmt, stmt_len);
     ERROR_CHECK(hstmt);
     switch (cmd_mode) {
	 case (CMD_NO_RESULT) :
		 break;
         case (CMD_RESULT) :
	    while (retcode == SQL_SUCCESS) {
	       retcode = SQLFetch (hstmt);
	       SPRINTF(buffer," SQLFetch retcode: '%d'\n", retcode);
	       PRINTF(buffer);
	       ERROR_CHECK(hstmt);
	    }
	    break;
	 default :
           break;
     }
  }
  if (retcode == SQL_SUCCESS) {
     retcode = SQLFreeStmt(hstmt, SQL_DROP);
  }
  return (retcode);
}
