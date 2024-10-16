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
#define PROGNAME "LONCHAR2"
#define LONG3
#include "tst.h"
#include <stdlib.h>
#include <ctype.h>

#define TABLE PROGNAME
#define SELECT
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFLEN 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
				esq_sql_code = 0;\
                           SQLError(henv, hdbc, hstmtx,\
                              sqlstate, &esq_sql_code, error_txt,\
                                    len_error_txt, &used_error_txt);\
                           SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
                         PRINTF(buffer);\
                  SPRINTF(buffer, " esq_sql_code: %d"CRLF, esq_sql_code);\
                         PRINTF(buffer);\
                  SPRINTF(buffer, " error_txt: %s"CRLF, error_txt);\
                         PRINTF(buffer);\
                  switch (retcode) {\
                  case(100) :\
                          break;\
                  case(SQL_ERROR) : \
                     if (STRCMP(sqlstate,"S1000") && esq_sql_code != -22002 ) \
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

VOID FillLong(UCHAR FAR *ptr, SDWORD len, UCHAR s);

UCHAR driverver [ 11 ];

UCHAR buffer [BUFLEN];
#define MAXLONGCOLUMNS 2
#define CHAR_LEN 120
#define LONGVARCHAR_LEN 65000
#define LEN_ERROR_TXT 400

UCHAR szCHAR[CHAR_LEN+1];
UCHAR FAR *szLONGVARCHAR;
SDWORD cbDOUBLE, cbCHAR, cbLONG,
cbLONGVARCHAR, *pcbLONGVARCHAR[MAXLONGCOLUMNS], cbDECIMAL;

#define ENDLEN SQL_NULL_DATA-1

typedef struct {
   SQLLEN cbValue1;
   SDWORD len1;
   SQLLEN cbValue2;
   SDWORD len2;
} TestLenStruct;

#define MAXBLOCK 16000

static TestLenStruct TestLen[] = {
{SQL_DATA_AT_EXEC, SQL_NULL_DATA, SQL_DATA_AT_EXEC, SQL_NULL_DATA},
{SQL_DATA_AT_EXEC, SQL_NULL_DATA, SQL_DATA_AT_EXEC, 254},
{SQL_DATA_AT_EXEC, 254, SQL_DATA_AT_EXEC, SQL_NULL_DATA},

{SQL_DATA_AT_EXEC, 0, SQL_DATA_AT_EXEC, 0},
{SQL_DATA_AT_EXEC, 254, SQL_DATA_AT_EXEC, 254},
{SQL_NULL_DATA, 0, SQL_DATA_AT_EXEC, 254},
{SQL_DATA_AT_EXEC, 254, SQL_NULL_DATA, 0},
{SQL_NULL_DATA, 0, SQL_NULL_DATA, 0},

{SQL_DATA_AT_EXEC, SQL_NULL_DATA, SQL_DATA_AT_EXEC, 254},
{SQL_DATA_AT_EXEC, 254, SQL_DATA_AT_EXEC, SQL_NULL_DATA},
{SQL_DATA_AT_EXEC, SQL_NULL_DATA, SQL_DATA_AT_EXEC, SQL_NULL_DATA},

{SQL_NULL_DATA, 0, SQL_DATA_AT_EXEC, 123456},
{SQL_DATA_AT_EXEC, 32000, SQL_NULL_DATA, 0},
{SQL_DATA_AT_EXEC, 123456, SQL_DATA_AT_EXEC, 20000},
{ENDLEN, ENDLEN, ENDLEN, ENDLEN}
};

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
  UWORD ipar;
  SDWORD cNUM;

  RETCODE retcode;
  SQLLEN pToken;        /* must be as long as a pointer! */

  UDWORD vAutoCommit;
  TestLenStruct *p;
  RETCODE ret;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

  szLONGVARCHAR=(UCHAR FAR *)MALLOC(MAXBLOCK+1);
  if (!szLONGVARCHAR) {
     SPRINTF(buffer, "malloc fails program terminated."CRLF);
     PRINTF(buffer);
     return(255);
  }

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
                       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  vAutoCommit = 1;
  retcode = SQLSetConnectOption (hdbc, SQL_AUTOCOMMIT, vAutoCommit);
  SPRINTF(buffer," SQLSetConnectOption (SQL_AUTOCOMMIT=%d) retcode: %d"CRLF, vAutoCommit, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
#ifdef LONG3
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 integer, col2 long, col3 long)", SQL_NTS);
#else
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 integer, col2 long)", SQL_NTS);
#endif
  ERROR_CHECK(hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  for (p = TestLen; p->len1 > ENDLEN; p++) {
     ipar = 1;
     cNUM=p->len1;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_LONG, SQL_INTEGER, (UDWORD) 0, 0, &cNUM, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     ipar = 2;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARCHAR, (UDWORD) p->len1, 0, (PTR)ipar, &p->cbValue1);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

#ifdef LONG3
     ipar = 3;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARCHAR, (UDWORD) p->len1, 0, (PTR)ipar, &p->cbValue2);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     retcode = SQLExecDirect (hstmt, "insert into "TABLE" values (?, ?, ?)", SQL_NTS);
#else
     retcode = SQLExecDirect (hstmt, "insert into "TABLE" values (?, ?)", SQL_NTS);
#endif
     ret = retcode;
     SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     while (ret == SQL_NEED_DATA) {
        retcode = SQLParamData(hstmt,  (PTR *)&pToken);
        ret = retcode;
        SPRINTF(buffer, " SQLParamData(%ld) retcode: '%s'"CRLF, pToken, cnvret(retcode));
        PRINTF(buffer);
        ERROR_CHECK(hstmt);
        if (retcode == SQL_NEED_DATA) {
           switch(pToken) {
              SDWORD i;
              SDWORD j;
              case 1:{
                 break;
              }
              case 2: {}
              case 3:{
		 SDWORD len=p->len1;
                 UCHAR c='A';
		 if (pToken == 3) {
		    len = p->len2;
		    c = 'a';
		 }
                 j = len;
                 for (i=len; i >= 0 || len == SQL_NULL_DATA; i -= MAXBLOCK) {
                    j = len;
                    if (j != SQL_NULL_DATA) {
                       j = (i > MAXBLOCK) ? MAXBLOCK : i;
                       FillLong(szLONGVARCHAR, j, c);
                       c++;
                    }
                    cbLONGVARCHAR = j;
                    retcode = SQLPutData(hstmt, szLONGVARCHAR, cbLONGVARCHAR);
                    SPRINTF(buffer, " SQLPutData (%d) retcode: '%s'"CRLF, cbLONGVARCHAR, cnvret(retcode));
                    PRINTF(buffer);
                    ERROR_CHECK(hstmt);
		    if (retcode == SQL_ERROR || j == SQL_NULL_DATA) {
			if (retcode == SQL_ERROR)
				ret = retcode;
		       break;
		    }
		 }
                 break;
              }
           }
        }
     }
     retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
     SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

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

VOID FillLong(UCHAR FAR *ptr, SDWORD len, UCHAR x)
{
   UCHAR c, e, s;
   SDWORD j;
   s = isalpha(x) ? x : (UCHAR)'A';
   e = islower(s) ? (UCHAR)'z' : (UCHAR)'Z';
   c = s;
   for (j = 0; j < len; j++, ptr++) {
      if ((UCHAR)(c-1) == e)
         c = islower(s) ? (UCHAR)'a' : (UCHAR)'A';
      *ptr = c++;
   }
   *ptr = '\0';
   return;
}
