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
#define PROGNAME "mfetch"

#include "tst.h"
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
				SPRINTF(buffer," esq_sql_code: %ld\n", esq_sql_code);\
		                PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		          if (esq_sql_code != -6000) \
			  return(16);\
			  break; } }

RETCODE command (HENV henv, HDBC hdbc, UCHAR *stmt, SWORD cmd_mode, SDWORD stmt_len);

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define F1_LEN 101
#define MAX_STMT 20
UCHAR szF1[F1_LEN];
SWORD sC1;
SQLLEN cbF1, cbC1;
#define C_LEN 18

UCHAR szCURSOR[C_LEN+1];
SWORD cbCURSOR;
UWORD i;

int main(int argc, argv_t argv)
{
  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmtarr[MAX_STMT];
  UCHAR stmt [ 3000 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code = 0;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  RETCODE retcode;

  UWORD i;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running.\n\n");
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
		       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  strcpy(stmt, "create table "TABLE" ( i1 int, c1 char (100))");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);

  strcpy(stmt, "insert "TABLE" values ( 10, '*** erste Zeile ***')");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);
  ERROR_CHECK(NULL);

  strcpy(stmt, "insert "TABLE" values ( 20, '*** zweite Zeile ***')");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);
  ERROR_CHECK(NULL);

  strcpy(stmt, "insert "TABLE" values ( 20, '*** dritte Zeile ***')");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);
  ERROR_CHECK(NULL);

  strcpy(stmt, "select * from "TABLE);
  retcode = command( henv, hdbc, stmt, CMD_RESULT, SQL_NTS);
  ERROR_CHECK(NULL);

  for (i=0; i < MAX_STMT; i++) {
     retcode = SQLAllocStmt (hdbc, &hstmtarr[i]);
     ERROR_CHECK(NULL);
  }

  STRCPY(stmt, "select * from "TABLE);
  for (i=0; i < MAX_STMT; i++) {
     retcode = SQLExecDirect (hstmtarr[i], stmt, STRLEN(stmt));
     ERROR_CHECK(hstmtarr[i]);

     retcode = SQLGetCursorName (hstmtarr[i], szCURSOR,
	sizeof(szCURSOR), &cbCURSOR);
     SPRINTF(buffer,"%s"CRLF, szCURSOR);
     PRINTF(buffer);
     ERROR_CHECK(hstmtarr[i]);

     retcode = SQLFetch (hstmtarr[i]);
     ERROR_CHECK(hstmtarr[i]);

     retcode = SQLGetData (hstmtarr[i], 1, SQL_C_CHAR, szF1,
	sizeof(szF1), &cbF1);
     SPRINTF(buffer,"%s"CRLF, szF1);
     PRINTF(buffer);
     ERROR_CHECK(hstmtarr[i]);
  }

  for (i=0; i < MAX_STMT / 2; i++) {
     retcode = SQLFreeStmt(hstmtarr[i], SQL_DROP);
     SPRINTF(buffer,"SQLFreeStmt (%d) retcode: %d"CRLF, i, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmtarr[i]);
  }

  for (i=0; i < MAX_STMT / 2; i++) {
     retcode = SQLAllocStmt (hdbc, &hstmtarr[i]);
     SPRINTF(buffer,"SQLAllocStmt (%d) retcode: %d"CRLF, i, retcode);
     PRINTF(buffer);
     ERROR_CHECK(NULL);
  }

  for (i=0; i < MAX_STMT /2; i++) {
     retcode = SQLExecDirect (hstmtarr[i], stmt, STRLEN(stmt));
     ERROR_CHECK(hstmtarr[i]);

     retcode = SQLGetCursorName (hstmtarr[i], szCURSOR,
	sizeof(szCURSOR), &cbCURSOR);
     SPRINTF(buffer,"%s"CRLF, szCURSOR);
     PRINTF(buffer);
     ERROR_CHECK(hstmtarr[i]);

     retcode = SQLFetch (hstmtarr[i]);
     ERROR_CHECK(hstmtarr[i]);

     retcode = SQLGetData (hstmtarr[i], 1, SQL_C_CHAR, szF1,
	sizeof(szF1), &cbF1);
     SPRINTF(buffer,"%s"CRLF, szF1);
     PRINTF(buffer);
     ERROR_CHECK(hstmtarr[i]);
  }

  for (i=MAX_STMT / 2; i < MAX_STMT; i++) {
     retcode = SQLFreeStmt(hstmtarr[i], SQL_DROP);
     SPRINTF(buffer,"SQLFreeStmt (%d) retcode: %d"CRLF, i, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmtarr[i]);
  }

  strcpy(stmt, "drop table "TABLE);
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_NTS);
  ERROR_CHECK(NULL);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, retcode);
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

RETCODE command (HENV henv,
HDBC hdbc,
UCHAR *stmt,
SWORD cmd_mode,
SDWORD stmt_len)
{
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code = 0;
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
  if (retcode == SQL_SUCCESS || retcode == SQL_NO_DATA_FOUND) {
     retcode = SQLFreeStmt(hstmt, SQL_DROP);
  }
  return (retcode);
}
