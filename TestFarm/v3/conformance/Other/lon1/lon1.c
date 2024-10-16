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
#define PROGNAME "LON1"
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
                           SQLError(henv, hdbc, hstmtx,\
                              sqlstate, &esq_sql_code, error_txt,\
                                    len_error_txt, &used_error_txt);\
                           SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
                           SPRINTF(buffer, " esq_sql_code: %d"CRLF, esq_sql_code);\
                           PRINTF(buffer);\
                           SPRINTF(buffer," error_txt: %s"CRLF, error_txt);\
                           PRINTF(buffer);\
                  switch (retcode) {\
                  case(SQL_NO_DATA_FOUND) :\
                  case(SQL_STILL_EXECUTING) :\
                          break;\
                  case(SQL_ERROR) : \
                     if (!STRCMP(sqlstate,"22005") &&\
                        !STRCMP(sqlstate,"22003") &&\
                           !STRCMP(sqlstate,"22008") &&\
                              !STRCMP(sqlstate,"07006"))\
                              { return(16); }\
                     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
                       if (STRCMP(sqlstate,"01004"))\
                          if (STRCMP(sqlstate,"22001"))\
                        { return(16); }\
                     break;\
                  default   :\
                          return(16);\
                          break; } }

UCHAR buffer [BUFSIZE];

HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;
#define MAXLONGVARCHAR 100000
int main(int argc, argv_t argv)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 302 ] ;
  SWORD len_error_txt = 300;
  SWORD used_error_txt;
  RETCODE retcode;
  SQLLEN cbLongVarcharLength;
  SQLLEN cbIntValue = 8;
  UCHAR  *szLongVarchar;

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

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect(hstmt, "DROP TABLE "TABLE, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect(hstmt, "create table "TABLE" ( i int, l long byte)", SQL_NTS);
  SPRINTF(buffer," SQLExecDirect (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  szLongVarchar = MALLOC(MAXLONGVARCHAR);
  if (szLongVarchar) {
     UCHAR FAR *ptr;
     UCHAR c;
     int j;
     ptr = szLongVarchar;
     c = 'Z';
     for (j = 1; j < MAXLONGVARCHAR; j++, ptr++)
        *ptr = c = (c == 'Z') ? 'A' : ++c;
     *ptr = '\0';
     cbLongVarcharLength = MAXLONGVARCHAR;
     retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
        SQL_INTEGER, 10, 0, &cbIntValue, 0, NULL);
     SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY,
        SQL_LONGVARCHAR, MAXLONGVARCHAR, 0, szLongVarchar, MAXLONGVARCHAR, &cbLongVarcharLength);
     SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect(hstmt, "insert into "TABLE" values (?, ?)", SQL_NTS);
     SPRINTF(buffer," SQLExecDirect (%s)"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLBindCol(hstmt, 1, SQL_C_SLONG, &cbIntValue, 0, NULL);
     SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLBindCol(hstmt, 2, SQL_C_BINARY, szLongVarchar, MAXLONGVARCHAR, &cbLongVarcharLength);
     SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect(hstmt, "select i, l from "TABLE, SQL_NTS);
     SPRINTF(buffer," SQLExecDirect (%s)"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     cbLongVarcharLength = MAXLONGVARCHAR;
     MEMSET(szLongVarchar, 'x', cbLongVarcharLength);

     retcode = SQLFetch(hstmt);
     SPRINTF(buffer," SQLFetch (%s)"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     ptr = szLongVarchar;
     c = 'Z';
     for (j = 1; j < MAXLONGVARCHAR; j++, ptr++)
        if (*ptr != (c = (c == 'Z') ? 'A' : ++c)) {
           SPRINTF(buffer," Error in row"CRLF);
           PRINTF(buffer);
           break;
        }
  }

  retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
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

