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

#define PROGNAME "LONCHAR"
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
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
		  printf(" error_txt: %s"CRLF, error_txt);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (!STRCMP(sqlstate,"22005") &&\
			!STRCMP(sqlstate,"22003") &&\
			   !STRCMP(sqlstate,"22008") &&\
			      !STRCMP(sqlstate,"07006"))\
			      { exit(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ exit(16); }\
		     break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
#define MAX_COL 30
UWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
SQLULEN pcbColDef[MAX_COL];

UCHAR buffer [BUFSIZE];

#define CHAR_LEN 120
#define LONGVARCHAR_LEN 65000
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
double  sDOUBLE;
double  sDECIMAL;
SQLLEN cbDOUBLE, cbCHAR, cbLONGVARCHAR, *pcbLONGVARCHAR, cbDECIMAL;
#define ENDLEN SQL_NULL_DATA-1
SWORD testlen[] = {SQL_NULL_DATA, 10, 254, 255, 256, 1024, 4119,
4120, 4121, 8240, 32764, 32765, ENDLEN};

void fetchout();

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
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UWORD ipar;
  SCHAR *ptr, c;
  SDWORD cNUM;

  UWORD icol;

  RETCODE retcode;
  SWORD pccol;

  int i, j;

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

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 integer, col2 long)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  for (i=0; testlen[i] > ENDLEN; i++) {
     ipar = 1;
     cNUM=testlen[i];
     retcode = SQLSetParam (hstmt, ipar, SQL_C_LONG, SQL_INTEGER, (UDWORD) 0, 0, &cNUM, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     if (testlen[i] != SQL_NULL_DATA) {
	pcbLONGVARCHAR = NULL;
	ptr = (SCHAR FAR*)szLONGVARCHAR;
	c = 'Z';
	for (j = 0; j < testlen[i]; j++, ptr++)
	   *ptr = c = (c == 'Z') ? 'A' : ++c;
	*ptr = '\0';
	if (testlen[i] > 0)
	   *(ptr-1) = '*';
     }
     else {
	pcbLONGVARCHAR = &cbLONGVARCHAR;
	cbLONGVARCHAR = testlen[i];
     }

     ipar = 2;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARCHAR, (UDWORD) testlen[i], 0, szLONGVARCHAR, pcbLONGVARCHAR);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     SPRINTF(buffer, "insert ... len = %ld"CRLF, testlen[i]);
     PRINTF(buffer);
     retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?)", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

     retcode = SQLPrepare (hstmt, "select * from "TABLE" where col1=?", SQL_NTS);
     SPRINTF(buffer, " SQLPrepare retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLNumResultCols (hstmt, &pccol);
     SPRINTF(buffer, " SQLNumResultCols retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     SPRINTF(buffer, " pccol : %ld"CRLF, pccol);
     PRINTF(buffer);

     SPRINTF(buffer, " COLNAME LEN TYPE PRECISION SCALE NULLABLE"CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
     PRINTF(buffer);
     for (col=1; col <= pccol; col++ ) {
	retcode = SQLDescribeCol (hstmt, col, szColName[col], NAME_LEN+1,
	   &pcbColName[col], &pfSqlType[col], &pcbColDef[col], &pibScale[col], &pfNullable[col]);
	SPRINTF(buffer, " % 7s %3d %4d %9d %5d %8d "CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
	PRINTF(buffer);
     }

     ipar = 1;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_LONG, SQL_INTEGER, (UDWORD) 0, 0, &cNUM, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     cbLONGVARCHAR = 0;
     retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCHAR, sizeof(szCHAR), &cbCHAR);
     retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szLONGVARCHAR, sizeof(szLONGVARCHAR), &cbLONGVARCHAR);
     retcode = SQLExecute (hstmt);
     SPRINTF(buffer, " SQLExecute retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     fetchout();

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

void fetchout()
{
  RETCODE retcode=SQL_SUCCESS;
  UDWORD len;
  while (retcode == SQL_SUCCESS) {
     cbCHAR=0;
     cbLONGVARCHAR=0;
     MEMSET(szLONGVARCHAR, 'x', sizeof(szLONGVARCHAR));
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
	SPRINTF(buffer, " SQLFetch retcode: '%d'"CRLF, retcode);
	PRINTF(buffer);
	SPRINTF(buffer, "col1(%d) = '%s'"CRLF, cbCHAR, szCHAR);
	PRINTF(buffer);
	SPRINTF(buffer, "len = %d"CRLF, cbLONGVARCHAR);
	PRINTF(buffer);
	len = STRLEN(szLONGVARCHAR);
	if (len < BUFSIZE)
	   SPRINTF(buffer, "'%s'"CRLF, szLONGVARCHAR);
	else
	   SPRINTF(buffer, "'%.*s"CRLF"........"CRLF"%s'"CRLF,
	      BUFSIZE/3, szLONGVARCHAR, ((char *)szLONGVARCHAR+len-BUFSIZE/3));
	PRINTF(buffer);
     }
  }
}
