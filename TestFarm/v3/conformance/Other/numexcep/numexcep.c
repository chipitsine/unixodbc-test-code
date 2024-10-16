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
#define PROGNAME "NUMEXECP"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "SAPDB"
#endif

#define MAXLOOP 10

#define ERROR_CHECK(henvx, hdbcx, hstmtx) if (retcode != SQL_SUCCESS) {\
			   SQLError(henvx,\
				    hdbcx,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			   SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
			   PRINTF(buffer);\
			   SPRINTF(buffer, " sqltext: %s"CRLF, error_txt);\
			   PRINTF(buffer);\
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
		  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		     if (esq_sql_code != -6000)\
			  return(16);\
			  break; } }

#define F1_LEN 101
UCHAR szConnStrOut[F1_LEN]; SWORD cbConnStrOut;
UCHAR buffer [1024];
HENV henv   = SQL_NULL_HENV;
HENV henv2  = SQL_NULL_HENV;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC hdbc2   = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT hstmt2 = SQL_NULL_HSTMT;
UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
SWORD len_error_txt = ERROR_TEXT_LEN;
SWORD used_error_txt;
RETCODE retcode;
UDWORD i;


int main(int argc, argv_t argv)
{
  SDWORD S1[4];
  SQLLEN S1I[4];
  SQLROWSETSIZE crow;
  int i;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, " %s running."CRLF, PROGNAME);
  PRINTF(buffer);
  szConnStrOut[0] = '\0';
  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, "SQLAllocEnv '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, "SQLAllocConnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
		       ODBCUSER,  (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, "SQLConnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocStmt : '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);

  retcode = SQLExecDirect (hstmt, "create table "TABLE" (s1 int, s2 int)", SQL_NTS);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ( 0, 0)", SQL_NTS);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ( 0, NULL)", SQL_NTS);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ( NULL, 0)", SQL_NTS);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ( NULL, NULL)", SQL_NTS);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExecDirect (hstmt, "select s1/s2 from "TABLE, SQL_NTS);
  SPRINTF(buffer, "SQLExecDirect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  while (retcode == SQL_SUCCESS) {
    S1[0]=99;
    S1I[0]=99;
    retcode = SQLFetch (hstmt);
    SPRINTF(buffer, "SQLFetch '%s'"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(henv, hdbc, hstmt);
    if (retcode == SQL_SUCCESS) {
      retcode = SQLGetData (hstmt, 1, SQL_C_SLONG, &S1[0], sizeof(S1[0]), &S1I[0]);
      ERROR_CHECK(henv, hdbc, hstmt);
      SPRINTF(buffer, "S1=%ld;S1I=%ld"CRLF, S1[0], S1I[0]);
      PRINTF(buffer);
    }
  }

  retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
  SPRINTF(buffer, "SQLFreeStmt '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExecDirect (hstmt, "select s1/s2 from "TABLE, SQL_NTS);
  SPRINTF(buffer, "SQLExecDirect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLBindCol (hstmt, 1, SQL_C_SLONG, &S1[0],
     sizeof(S1[0]), &S1I[0]);
  SPRINTF(buffer, "SQLBindCol '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  while (retcode == SQL_SUCCESS) {
    S1[0]=99;
    S1I[0]=99;
    retcode = SQLFetch (hstmt);
    SPRINTF(buffer, "SQLFetch '%s'"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(henv, hdbc, hstmt);
    if (retcode == SQL_SUCCESS) {
      SPRINTF(buffer, "S1=%ld;S1I=%ld"CRLF, S1[0], S1I[0]);
      PRINTF(buffer);
    }
  }

  retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
  SPRINTF(buffer, "SQLFreeStmt '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  for (i=0;i < sizeof(S1)/sizeof(S1[0]);i++) {
    S1[i]=99;
    S1I[i]=99;
  }

  retcode = SQLSetStmtOption(hstmt, SQL_ROWSET_SIZE, 4);
  SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExecDirect (hstmt, "select s1/s2 from "TABLE, SQL_NTS);
  SPRINTF(buffer, "SQLExecDirect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLExtendedFetch(hstmt, SQL_FETCH_NEXT, 0, &crow, NULL);
  SPRINTF(buffer, " SQLExtendedFetch(%ld) retcode: '%s'"CRLF, crow, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  for (i=0;i < crow;i++) {
    SPRINTF(buffer, "S1=%ld;S1I=%ld"CRLF, S1[i], S1I[i]);
    PRINTF(buffer);
  }

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, "SQLDisconnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, "SQLFreeConnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, "SQLFreeEnv '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);

  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
