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
#define PROGNAME "lonchar"
#include "tst.h"

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
#define MAX_COL 30
UWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
SQLULEN pcbColDef[MAX_COL];

UCHAR buffer [BUFLEN];

#define CHAR_LEN 120
#define LONGVARCHAR_LEN 65000
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
UCHAR szLONGVARCHAR2[LONGVARCHAR_LEN+1];
double  sDOUBLE;
double  sDECIMAL;
SQLLEN cbDOUBLE, cbCHAR, cbLONG, cbLONGVARCHAR, *pcbLONGVARCHAR,
cbLONGVARCHAR2, *pcbLONGVARCHAR2, cbDECIMAL;
#define ENDLEN SQL_NULL_DATA-1
typedef struct {
   SWORD len1;
   SWORD len2;
} TestLenStruct;

#define LIMITVALUES
static TestLenStruct TestLen[] = {
{254, SQL_NULL_DATA},
#ifdef LIMITVALUES
{SQL_NULL_DATA, 10},
{10, 10},
{255, 255},
{256, 0},
{1024, 1},
{8240, 8240},
{0, 10000},
{32764, 20},
{32765, 20},
{SQL_NULL_DATA, SQL_NULL_DATA},
#endif
{ENDLEN, ENDLEN}
};

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

  int j;
  UDWORD vAutoCommit;
  TestLenStruct *p;

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

  vAutoCommit = 0;
  retcode = SQLSetConnectOption (hdbc, SQL_AUTOCOMMIT, vAutoCommit);
  SPRINTF(buffer," SQLSetConnectOption (SQL_AUTOCOMMIT=%d) retcode: %d"CRLF, vAutoCommit, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "drop table "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 integer, col2 long, col3 long)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  for (p = TestLen; p->len1 > ENDLEN; p++) {
     ipar = 1;
     cNUM=p->len1;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_LONG, SQL_INTEGER, (UDWORD) 0, 0, &cNUM, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     if (p->len1 != SQL_NULL_DATA) {
	pcbLONGVARCHAR = NULL;
	ptr = (SCHAR FAR *)szLONGVARCHAR;
	c = 'Z';
	for (j = 0; j < p->len1; j++, ptr++) {
	   if (c == 'Z')
		c = 'A';
	   else
		c++;
           *ptr = c;
	}
	*ptr = '\0';
	if (p->len1 > 0)
	   *(ptr-1) = '*';
     }
     else {
	pcbLONGVARCHAR = &cbLONGVARCHAR;
	cbLONGVARCHAR = p->len1;
     }

     ipar = 2;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARCHAR, (UDWORD) p->len1, 0, szLONGVARCHAR, pcbLONGVARCHAR);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     if (p->len2 != SQL_NULL_DATA) {
	pcbLONGVARCHAR2 = NULL;
	ptr = (SCHAR FAR *)szLONGVARCHAR2;
	c = 'z';
	for (j = 0; j < p->len2; j++, ptr++) {
	   if (c == 'z')
		c = 'a';
	   else
		c++;
           *ptr = c;
	}
	*ptr = '\0';
	if (p->len2 > 0)
	   *(ptr-1) = '#';
     }
     else {
	pcbLONGVARCHAR2 = &cbLONGVARCHAR2;
	cbLONGVARCHAR2 = p->len2;
     }

     ipar = 3;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARCHAR, (UDWORD) p->len2, 0, szLONGVARCHAR2, pcbLONGVARCHAR2);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     SPRINTF(buffer, "insert ... len1 = %d len2 = %d"CRLF, p->len1, p->len2);
     PRINTF(buffer);
     retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?, ?)", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
#ifdef SELECT
     retcode = SQLPrepare (hstmt, "select * from "TABLE" where col1=?", SQL_NTS);
     SPRINTF(buffer, " SQLPrepare retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLNumResultCols (hstmt, &pccol);
     SPRINTF(buffer, " SQLNumResultCols retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     SPRINTF(buffer, " pccol : %d"CRLF, pccol);
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
     retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szLONGVARCHAR2, sizeof(szLONGVARCHAR2), &cbLONGVARCHAR2);
     retcode = SQLExecute (hstmt);
     SPRINTF(buffer, " SQLExecute retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     fetchout();
#endif
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
     MEMSET(szLONGVARCHAR, 'X', sizeof(szLONGVARCHAR));
     MEMSET(szLONGVARCHAR2, 'x', sizeof(szLONGVARCHAR2));
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
	SPRINTF(buffer, " SQLFetch retcode: '%d'"CRLF, retcode);
	PRINTF(buffer);
	SPRINTF(buffer, "col1(%ld) = '%s'"CRLF, cbCHAR, szCHAR);
	PRINTF(buffer);
	SPRINTF(buffer, "len1 = %d"CRLF, cbLONGVARCHAR);
	PRINTF(buffer);
	len = STRLEN(szLONGVARCHAR);
	if (len < BUFLEN)
	   SPRINTF(buffer, "'%s'"CRLF, szLONGVARCHAR);
	else
	   SPRINTF(buffer, "'%.*s"CRLF"........"CRLF"%s'"CRLF,
	      BUFLEN/3, szLONGVARCHAR, ((char *)szLONGVARCHAR+len-BUFLEN/3));
	PRINTF(buffer);

	SPRINTF(buffer, "len2 = %d"CRLF, cbLONGVARCHAR2);
	PRINTF(buffer);
	len = STRLEN(szLONGVARCHAR2);
	if (len < BUFLEN)
	   SPRINTF(buffer, "'%s'"CRLF, szLONGVARCHAR2);
	else
	   SPRINTF(buffer, "'%.*s"CRLF"........"CRLF"%s'"CRLF,
	      BUFLEN/3, szLONGVARCHAR2, ((char *)szLONGVARCHAR2+len-BUFLEN/3));
	PRINTF(buffer);
     }
  }
}
