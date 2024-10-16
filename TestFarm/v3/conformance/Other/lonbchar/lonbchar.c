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
#define PROGNAME "LONBCHAR"
#include "tst.h"

#define TABLE PROGNAME

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
                  case(SQL_ERROR) : \
		     if (STRCMP(sqlstate,"22005"))\
			{ exit(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
			{ exit(16); }\
		     break;\
		  default   :\
			  exit(16);\
			  break; } }

#define BINARYSTRING "4c6F4E6e737472696e67"
HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;

UCHAR driverver [ 11 ];
#define MAX_COL 30
UWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
SQLULEN pcbColDef[MAX_COL];

UCHAR buffer [1024];

#define CHAR_LEN 120
#define BINARY_LEN 1000
#define LONGVARCHAR_LEN 10*CHAR_LEN
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
UCHAR szBINARY[BINARY_LEN+1];
double  sDOUBLE;
double  sDECIMAL;
SQLLEN cbDOUBLE, cbCHAR, cbBINARY, cbLONGVARCHAR, cbDECIMAL;

void fetch(void);
void fetch1(void);

int main(int argc, argv_t argv)
{

  UCHAR stmt2 [ 50 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  SDWORD size_1 = 0;
  SDWORD size_2 = 0;
  UWORD ipar;

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
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 char(100) byte, col2 long byte)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_BINARY, (UDWORD) (sizeof(szBINARY)-1), 0, szBINARY, &cbBINARY);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_LONGVARBINARY, (UDWORD) (sizeof(szLONGVARCHAR)-1), 0, szLONGVARCHAR, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(szLONGVARCHAR, BINARYSTRING);
  STRCPY(szBINARY, BINARYSTRING);
  cbBINARY = STRLEN(szBINARY);

  SPRINTF(buffer, "insert ... SQL_C_CHAR, SQL_C_LONGVARBINARY"CRLF);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?)", SQL_NTS);
  ERROR_CHECK(hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_BINARY, SQL_LONGVARBINARY, (UDWORD) (sizeof(szBINARY)-1), 0, szBINARY, &cbBINARY);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_BINARY, SQL_LONGVARBINARY, (UDWORD) (sizeof(szLONGVARCHAR)-1), 0, szLONGVARCHAR, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, "insert ... SQL_C_BINARY, SQL_LONGVARBINARY"CRLF);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?)", SQL_NTS);
  ERROR_CHECK(hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_BINARY, SQL_CHAR, (UDWORD) (sizeof(szBINARY)-1), 0, szBINARY, &cbBINARY);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_BINARY, SQL_CHAR, (UDWORD) (sizeof(szLONGVARCHAR)-1), 0, szLONGVARCHAR, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, "insert ... SQL_C_BINARY, SQL_CHAR"CRLF);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?)", SQL_NTS);
  ERROR_CHECK(hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_BINARY, SQL_LONGVARCHAR, (UDWORD) (sizeof(szBINARY)-1), 0, szBINARY, &cbBINARY);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_BINARY, SQL_LONGVARCHAR, (UDWORD) (sizeof(szLONGVARCHAR)-1), 0, szLONGVARCHAR, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, "insert ... SQL_C_BINARY, SQL_LONGVARCHAR"CRLF);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?)", SQL_NTS);
  ERROR_CHECK(hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLPrepare (hstmt, "select * from "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLNumResultCols (hstmt, &pccol);
  SPRINTF(buffer, " SQLNumResultCols retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  SPRINTF(buffer, " pccol : %ld"CRLF, pccol);
  PRINTF(buffer);

  SPRINTF(buffer, " COLNAME LEN TYPE PRECISION SCALE NULLABLE"CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
  PRINTF(buffer);
  for (col=1; col <= pccol; col++ ) {
     retcode = SQLDescribeCol (hstmt, col, szColName[col], NAME_LEN+1,
	&pcbColName[col], &pfSqlType[col], &pcbColDef[col], &pibScale[col], &pfNullable[col]);
     ERROR_CHECK(hstmt);
     SPRINTF(buffer, " % 7s %3d %4d %9d %5d %8d "CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
     PRINTF(buffer);
  }

  SPRINTF(buffer, "SQL_LONGVARBINARY -> SQL_C_CHAR"CRLF, retcode);
  PRINTF(buffer);
  retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCHAR, sizeof(szCHAR), &cbCHAR);
  ERROR_CHECK(hstmt);
  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szLONGVARCHAR, sizeof(szLONGVARCHAR), &cbLONGVARCHAR);
  ERROR_CHECK(hstmt);
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  fetch();

  SPRINTF(buffer, "SQL_LONGVARBINARY -> SQL_C_CHAR (truncation)"CRLF, retcode);
  PRINTF(buffer);
  retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCHAR, sizeof(szCHAR), &cbCHAR);
  ERROR_CHECK(hstmt);
  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szLONGVARCHAR, strlen(BINARYSTRING), &cbLONGVARCHAR);
  ERROR_CHECK(hstmt);
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  fetch();

  SPRINTF(buffer, "SQL_LONGVARBINARY -> SQL_C_BINARY"CRLF, retcode);
  PRINTF(buffer);
  retcode = SQLBindCol (hstmt, 1, SQL_C_BINARY, szCHAR, sizeof(szCHAR), &cbCHAR);
  ERROR_CHECK(hstmt);
  retcode = SQLBindCol (hstmt, 2, SQL_C_BINARY, szBINARY, sizeof(szBINARY), &cbBINARY);
  ERROR_CHECK(hstmt);
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  fetch1();

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);

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

void fetch(void)
{
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  cbCHAR=SQL_NULL_DATA;
  cbLONGVARCHAR=SQL_NULL_DATA;
  MEMSET(szLONGVARCHAR, 0, sizeof(szLONGVARCHAR));
  retcode = SQLFetch (hstmt);
  if ( retcode != SQL_NO_DATA_FOUND ) {
     SPRINTF(buffer, " SQLFetch retcode: '%d'"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     SPRINTF(buffer, "col1(%d) = '%s'"CRLF, cbCHAR, szCHAR);
     PRINTF(buffer);
     SPRINTF(buffer, "col2(%d) = '%s'"CRLF, cbLONGVARCHAR, szLONGVARCHAR);
     PRINTF(buffer);
  }
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) retcode: '%d'"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
}

void fetch1(void)
{
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  cbBINARY=SQL_NULL_DATA;
  cbCHAR=SQL_NULL_DATA;
  MEMSET(szCHAR, '*', sizeof(szCHAR));
  MEMSET(szBINARY, '*', sizeof(szBINARY));
  retcode = SQLFetch (hstmt);
  if ( retcode != SQL_NO_DATA_FOUND ) {
     SPRINTF(buffer, " SQLFetch retcode: '%d'"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     SPRINTF(buffer, "col1(%d) = '%.*s'"CRLF, cbCHAR, cbCHAR+1, szCHAR);
     PRINTF(buffer);
     SPRINTF(buffer, "col2(%d) = '%.*s'"CRLF, cbBINARY, cbBINARY+1, szBINARY);
     PRINTF(buffer);
  }
}
