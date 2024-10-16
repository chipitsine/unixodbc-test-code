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
#define PROGNAME "INSLONG"
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
			   RETCODE errret;\
			   errret = SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
		  printf(" error_txt: %s"CRLF, error_txt);\
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
SQLLEN pcbColDef[MAX_COL];

UCHAR buffer [BUFSIZE];

#define CHAR_LEN 120
#define LONGVARCHAR_LEN 65000
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szCHAR1[CHAR_LEN+1];
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
double  sDOUBLE;
double  sDECIMAL;
SQLLEN cbCHAR, cbCHAR1, cbLONGVARCHAR;
SDWORD cbDOUBLE, cbDECIMAL;
#define BOUNDVAL 999

typedef struct {
   SWORD testnum;
   SDWORD testlen;
   SWORD putlen;
} TestPutStruct;


static TestPutStruct TestPut[] = {
#ifndef LIMITVALUES
{1, 254, 254},
{2, 254, 10},
{3, 31999, 10000},
#endif
{4, 25000, 10000},
{0, 0, 0}
};

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
  SDWORD esq_sql_code = 0;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UWORD ipar;
  SCHAR *ptr, c;
  TestPutStruct *p;
  UWORD icol;

  RETCODE retcode;
  RETCODE retp;
  SWORD pccol;

  SDWORD j;
  SDWORD i;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

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
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (testnum integer, testlen integer, col2 long)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  for (p=TestPut; p->testnum != 0; p++) {
     ipar = 1;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_SHORT, SQL_INTEGER, (UDWORD) 0, 0, &p->testnum, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     ipar = 2;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_LONG, SQL_INTEGER, (UDWORD) 0, 0, &p->testlen, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     ipar = 3;
     cbValue = SQL_DATA_AT_EXEC;
     pcbValue = &cbValue;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARCHAR, (UDWORD) p->testlen, 0, (PTR)BOUNDVAL, pcbValue);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     ptr = (SCHAR FAR*)szLONGVARCHAR;
     c = 'Z';
     for (j = 0; j < p->testlen; j++, ptr++)
	*ptr = c = (c == 'Z') ? 'A' : ++c;
     *ptr = '\0';
     if (p->testlen > 0)
	*(ptr-1) = '*';

     SPRINTF(buffer, "insert ... testlen = %ld putlen = %ld"CRLF, p->testlen, p->putlen);
     PRINTF(buffer);
     retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?, ?)", SQL_NTS);
     SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     while (retcode == SQL_NEED_DATA) {
	retcode = SQLParamData (hstmt, &rgbValue);
	SPRINTF(buffer, " SQLParamData retcode: %s"CRLF, cnvret(retcode));
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	if (retcode == SQL_NEED_DATA) {
	   if (BOUNDVAL == (SQLLEN)rgbValue) {
	      SDWORD len;
	      len = p->testlen;
	      cbValue = p->putlen;
	      for (len = p->testlen, ptr=(SCHAR FAR*)szLONGVARCHAR; len > 0; len -= p->putlen, ptr+=p->putlen) {
		 retp = SQLPutData (hstmt, ptr, cbValue);
		 SPRINTF(buffer, " SQLPutData retcode: %s"CRLF, cnvret(retp));
		 PRINTF(buffer);
		 SPRINTF(buffer, " pcbValue = %ld"CRLF, *pcbValue);
		 PRINTF(buffer);
	      }
	   }
	}
     }
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer, " SQLFreeStmt retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     ipar = 1;
     retcode = SQLSetParam (hstmt, ipar, SQL_C_SHORT, SQL_INTEGER, (UDWORD) 0, 0, &p->testnum, NULL);
     SPRINTF(buffer," SQLSetParam (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLPrepare (hstmt, "select * from "TABLE" where testnum=?", SQL_NTS);
     SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLNumResultCols (hstmt, &pccol);
     SPRINTF(buffer, " SQLNumResultCols retcode: %s"CRLF, cnvret(retcode));
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

     retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCHAR, sizeof(szCHAR), &cbCHAR);
     retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szCHAR1, sizeof(szCHAR1), &cbCHAR1);
     retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szLONGVARCHAR, sizeof(szLONGVARCHAR), &cbLONGVARCHAR);
     retcode = SQLExecute (hstmt);
     SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     fetchout();

     retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
     SPRINTF(buffer, " SQLFreeStmt retcode: %s"CRLF, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

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
  UDWORD len;
  while (retcode == SQL_SUCCESS) {
     cbCHAR=SQL_NULL_DATA;
     cbLONGVARCHAR=SQL_NULL_DATA;
     MEMSET(szLONGVARCHAR, 0, sizeof(szLONGVARCHAR));
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
	SPRINTF(buffer, " SQLFetch retcode: '%s'"CRLF, cnvret(retcode));
	PRINTF(buffer);
	SPRINTF(buffer, "testnum(%d) = '%s'"CRLF, cbCHAR, szCHAR);
	PRINTF(buffer);
	SPRINTF(buffer, "testlen(%d) = '%s'"CRLF, cbCHAR1, szCHAR1);
	PRINTF(buffer);
	len = (cbLONGVARCHAR == SQL_NTS) ? STRLEN(szLONGVARCHAR) : cbLONGVARCHAR;
	SPRINTF(buffer, "cbLONGVARCHAR=%ld"CRLF, len);
	PRINTF(buffer);
	if (len < BUFSIZE)
	   SPRINTF(buffer, "'%.*s'"CRLF, len, szLONGVARCHAR);
	else
	   SPRINTF(buffer, "'%.*s"CRLF"........"CRLF"%.*s'"CRLF,
	      BUFSIZE/3, szLONGVARCHAR, BUFSIZE/3, ((char *)szLONGVARCHAR+len-BUFSIZE/3));
	PRINTF(buffer);
     }
  }
}
