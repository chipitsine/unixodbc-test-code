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
#define PROGNAME "compon"
#include "tst.h"
#define SQL_CONNECT_OPT_DRVR_START 1000
#define SQL_COMPONENT (SQL_CONNECT_OPT_DRVR_START+3)
#define TABLE PROGNAME
#define SELECT
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#define CONTROLUSER "DBA"
#define CONTROLPASSWORD "DBA"
#endif

#define BUFLEN 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
                           SQLError(henv, hdbc, hstmtx,\
                              sqlstate, &esq_sql_code, error_txt,\
                                    len_error_txt, &used_error_txt);\
                           SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
                         PRINTF(buffer);\
                  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
                         PRINTF(buffer);\
                  SPRINTF(buffer, " error_txt: %s"CRLF, error_txt);\
                         PRINTF(buffer);\
                  switch (retcode) {\
                  case(100) :\
                          break;\
                  case(SQL_ERROR) : \
                     if (STRCMP(sqlstate,"S1000") && esq_sql_code != -22002 ) \
                     if (STRCMP(sqlstate,"IM002")) \
                     if (STRCMP(sqlstate,"08003")) \
                     if (STRCMP(sqlstate,"08001") && esq_sql_code != -715 ) \
		     { return(16); }\
                     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
                       if (STRCMP(sqlstate,"01004"))\
                          if (STRCMP(sqlstate,"22001"))\
                        { return(16); }\
                     break;\
                  case(SQL_NEED_DATA) : \
                     break;\
                  default   :\
                          return(16);\
                          break; } }

#define LEN_ERROR_TXT 200
UCHAR szConnStrIn[256];
UCHAR szConnStrOut[256];
SWORD cbConnStrOut;

UCHAR buffer [BUFLEN];

HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;

int main(int argc, argv_t argv)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ LEN_ERROR_TXT ] ;
  SWORD len_error_txt = LEN_ERROR_TXT;
  SWORD used_error_txt;
  RETCODE retcode;
  UWORD vParam;
  char buf[256], *q = szConnStrOut;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(szConnStrIn, "UID=%s;DSN=%s;pwd=%s;COMPOnent=3",CONTROLUSER,DBNODE,CONTROLPASSWORD);
  SPRINTF(buffer," szConnStrIn='%s'"CRLF, szConnStrIn);
  PRINTF(buffer);
  retcode = SQLDriverConnect (hdbc, NULL, szConnStrIn, SQL_NTS,
  szConnStrOut, sizeof(szConnStrOut), &cbConnStrOut, SQL_DRIVER_NOPROMPT);
  SPRINTF(buffer," SQLDriverConnect retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  SPRINTF(buffer," szConnStrOut=GREPWEG('%s')"CRLF, szConnStrOut);
  PRINTF(buffer);

  while (1) {
    char *p, *p1;
    p = strchr (q, ';');
    if (p == NULL)
      break;
    strncpy (buf, q, p-q);
    buf[p-q] = '\0';
    p1 = strchr (buf, '=');
    *p1 = '\0';
    q = p + 1;
    SPRINTF(buffer,"  %s"CRLF, buf);
    PRINTF(buffer);
  }


  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt(hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect(hstmt, "select * from monitor", SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLSetConnectOption(hdbc, SQL_COMPONENT, 3);
  SPRINTF(buffer, " SQLSetConnectOption: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLGetConnectOption(hdbc, SQL_COMPONENT, &vParam);
  SPRINTF(buffer, " SQLGetConnectOption(SQL_COMPONENT = %ld): %d"CRLF, (long int) vParam, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, SQL_NTS, CONTROLUSER, SQL_NTS, CONTROLPASSWORD, SQL_NTS);
  SPRINTF(buffer," SQLConnect retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt(hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect(hstmt, "select * from monitor", SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

