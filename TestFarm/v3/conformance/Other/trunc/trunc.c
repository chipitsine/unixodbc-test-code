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
#define PROGNAME "trunc"
#include "tst.h"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
                  switch (retcode) {\
		  case(100) :\
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

UCHAR szCHAR[11];
double  sDOUBLE;
double  sDECIMAL;
SQLLEN  cbDOUBLE, cbCHAR, cbDECIMAL;

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt2;
  UCHAR stmt [ 3000 ];
  UCHAR stmt2 [ 50 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  SDWORD size_1 = 0;
  SDWORD size_2 = 0;

  UCHAR name [ 21 ];
  SDWORD age = -1;

  SWORD description_type;
  UCHAR description [ 33 ];
  SDWORD description_num;
  SWORD description_size;
  UWORD icol;
  UWORD col_count = 2;

  RETCODE retcode;
  SWORD pccol;

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;

  UCHAR stuff [ 80 ];

  ProcessCmdArguments(argc,argv);

  printf(PROGNAME" running."CRLF);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  printf(" SQLAllocEnv retcode: %d"CRLF, retcode);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  printf(" SQLAllocConnect retcode: %d"CRLF, retcode);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  printf(" SQLConnect retcode: %d"CRLF, retcode);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE TRUNCAT", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "create table truncat (dec5 decimal (5,0), c10 char(10), dec72 decimal(7,2))", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert truncat values (111, '375.00', 375.00  )", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert truncat values (111, '-1000.00', -1000.00  )", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert truncat values (111, '222.33', 222.33  )", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLPrepare (hstmt, "select * from truncat", SQL_NTS);
  printf(" SQLPrepare retcode: %d"CRLF, retcode);
  retcode = SQLNumResultCols (hstmt, &pccol);
  printf(" SQLNumResultCols retcode: %d"CRLF, retcode);
  printf(" pccol : %ld"CRLF, pccol);

  printf(" COLNAME LEN TYPE PRECISION SCALE NULLABLE"CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
  for (col=1; col <= pccol; col++ ) {
     retcode = SQLDescribeCol (hstmt, col, szColName[col], sizeof(szColName[col]),
	&pcbColName[col], &pfSqlType[col], &pcbColDef[col], &pibScale[col], &pfNullable[col]);
     printf(" %7s %3d %4d %9d %5d %8d "CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
  }

  retcode = SQLBindCol (hstmt, 1, SQL_C_DOUBLE, &sDOUBLE,
      20, &cbDOUBLE);
  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szCHAR,
      11, &cbCHAR);
  retcode = SQLBindCol (hstmt, 3, SQL_C_DOUBLE, &sDECIMAL,
      20, &cbDECIMAL);
  retcode = SQLExecute (hstmt);


  while (retcode == SQL_SUCCESS) {
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
	printf(" SQLFetch retcode: '%d'"CRLF, retcode);
	printf( "dec5 = '%f' c10  = '%s' dec72 = '%f'"CRLF, sDOUBLE, szCHAR, sDECIMAL);
     }
  }


  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  printf(" SQLTransact retcode: %d"CRLF, retcode);

  retcode = SQLDisconnect(hdbc);
  printf(" SQLDisconnect retcode: %d"CRLF, retcode);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  printf(" SQLFreeConnect retcode: %d"CRLF, retcode);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  printf(" SQLFreeEnv retcode: %d"CRLF, retcode);
  ERROR_CHECK(NULL);
  printf(" %s ending"CRLF, PROGNAME);

  return(0);
}
