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
#define PROGNAME "LGETDATA"
#include "tst.h"
#include "time.h"
#include "stdlib.h"

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
                           RETCODE errret;\
                           errret = SQLError(henv, hdbc, hstmtx,\
                              sqlstate, &esq_sql_code, error_txt,\
                                    len_error_txt, &used_error_txt);\
                           SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
                         PRINTF(buffer);\
                  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
                         PRINTF(buffer);\
                  SPRINTF(buffer, " error_txt: %s"CRLF, error_txt);\
                         PRINTF(buffer);\
                     if (errret == SQL_SUCCESS)\
                  switch (retcode) {\
                  case(SQL_NO_DATA_FOUND) :\
                  case(SQL_NEED_DATA) :\
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

UCHAR driverver [ 11 ];
#define MAXCOL 30
#define COLLEN 10
#define MAXROWS 10
SWORD col;

UCHAR buffer [BUFSIZE];
UCHAR szSTRING1[] = "ABCDEFGHIJKLMNOPQRSTUV";
UCHAR szSTRING2[] = "1234567";
UCHAR szSTRING3[] = "abcdefghijklmnopqrstuv";

#define CHAR_LEN 120
#define LONGVARCHAR_LEN 30000
UCHAR FAR *szCHAR;
UCHAR szCHAR1[CHAR_LEN+1];
UCHAR FAR *szLONGVARCHAR;
UCHAR FAR *szLONGVARCHAR2;
UCHAR FAR *szStmtBuffer;
double  sDOUBLE;
double  sDECIMAL;
SDWORD cbDOUBLE, cbCHAR, cbLONGVARCHAR, cbLONGVARCHAR2, cbDECIMAL;
#define BOUNDVAL 999

#define SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue) \
SQLSetParam (hstmt, ipar, fCType, fSqlType, cbColDef, ibScale, rgbValue, pcbValue)

void fetchout();


HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;
SQLLEN *pcbValue, cbValue;
PTR rgbValue;

int main(int argc, argv_t argv)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UWORD ipar;
  SCHAR *ptr;
  SWORD i, j;
  UCHAR c;
  time_t start, finish;
  RETCODE retcode;
  RETCODE retp;
  SWORD pccol;
  SWORD pToken;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  SPRINTF(buffer, " Insert and Select"CRLF"Rows = %d"CRLF"Columns %d Column Length %d"CRLF, MAXROWS, MAXCOL, COLLEN);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

  szStmtBuffer=(UCHAR FAR *)MALLOC(30*MAXCOL+200);
  if (!szStmtBuffer) {
     SPRINTF(buffer, "malloc fails (szStmtBuffer) program terminated."CRLF);
     PRINTF(buffer);
     return(255);
  }

  szCHAR=(UCHAR FAR *)MALLOC(COLLEN+1);
  if (!szCHAR) {
     SPRINTF(buffer, "malloc fails (szCHAR) program terminated."CRLF);
     PRINTF(buffer);
     return(255);
  }

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
                       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);

  SPRINTF(szStmtBuffer, "create table %s (col0 char(%d)", TABLE, COLLEN);
  for (i=1; i < MAXCOL; i++) {
     UCHAR lbuf[30];
     SPRINTF(lbuf, ", col%d char(%d)",i ,COLLEN);
     STRCAT(szStmtBuffer, lbuf);
  }
  STRCAT(szStmtBuffer, ")");
  retcode = SQLExecDirect (hstmt, szStmtBuffer, SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  pcbValue = &cbValue;
  for (i=1; i <= MAXCOL; i++) {
     ipar = i;
     retcode = SQLBindParameter (hstmt, ipar, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, szCHAR, 0, pcbValue);
     SPRINTF(buffer," SQLBindParameter (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

  SPRINTF(szStmtBuffer, "insert "TABLE" values (?");
  for (i=1; i < MAXCOL; i++) {
     STRCAT(szStmtBuffer, ", ?");
  }
  STRCAT(szStmtBuffer, ")");
  retcode = SQLPrepare (hstmt, szStmtBuffer, SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
  c='a';
  for (j=0; j < MAXROWS; j++,c++) {
     MEMSET(szCHAR, c, COLLEN);
     cbValue = COLLEN;
     retcode = SQLExecute (hstmt);
     SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     if (c == 'z')
        c = 'a'-1;
  }

  retcode = SQLPrepare (hstmt, "select * from "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLNumResultCols (hstmt, &pccol);
  SPRINTF(buffer, " SQLNumResultCols retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  SPRINTF(buffer, " pccol : %d"CRLF, pccol);
  PRINTF(buffer);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout();

  retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchout()
{
  RETCODE retcode=SQL_SUCCESS;
  UDWORD len, lbufs;
  SWORD col, row;
  row = 0;
  while (retcode == SQL_SUCCESS) {
     row++;
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
        SPRINTF(buffer, " SQLFetch retcode: %s"CRLF, cnvret(retcode));
        PRINTF(buffer);
        for (col=1; retcode == SQL_SUCCESS && col <= MAXCOL; col++) {
           retcode = SQLGetData (hstmt, col, SQL_C_CHAR, szCHAR, COLLEN+1, &cbValue);
           if (col == 1 || retcode != SQL_SUCCESS) {
              SPRINTF(buffer, " SQLGetData (%d) retcode: %s"CRLF, row, cnvret(retcode));
              PRINTF(buffer);
           }
           SPRINTF(buffer, "'%s'", szCHAR);
           PRINTF(buffer);
        }
        SPRINTF(buffer, CRLF);
        PRINTF(buffer);
     }
  }
}


