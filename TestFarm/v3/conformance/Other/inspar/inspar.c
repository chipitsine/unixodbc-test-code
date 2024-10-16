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
#define PROGNAME "INSPAR"
#include "tst.h"

#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqldial:db2dia30"

#define ODBCUSER  "DOMAIN"
#define ODBCPASSW "SYSTEM"
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
		  printf(" esq_sql_code: %d"CRLF, esq_sql_code);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (esq_sql_code != -6000 &&\
			esq_sql_code != -4004 &&\
			!STRCMP(sqlstate,"01004"))\
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
UDWORD pcbColDef[MAX_COL];

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
  UDWORD vAutoCommit;

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

  vAutoCommit = 0;
  retcode = SQLSetConnectOption (hdbc, SQL_AUTOCOMMIT, vAutoCommit);
  SPRINTF(buffer," SQLSetConnectOption (SQL_AUTOCOMMIT=%d) retcode: %d"CRLF, vAutoCommit, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLAllocStmt (hdbc, phstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 char(100))", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(szCHAR, "COL");
  retcode = SQLSetParam (hstmt, 1, SQL_C_CHAR, SQL_CHAR, (UDWORD) 100, 0, &szCHAR, NULL);
  SPRINTF(buffer, " SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(szCHAR, "AAABBBCCC");
  cbCHAR  = SQL_NTS;
  retcode = SQLSetParam (hstmt, 1, SQL_C_CHAR, SQL_CHAR, (UDWORD) 100, 0, &szCHAR, &cbCHAR);
  SPRINTF(buffer, " SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect (hstmt, "SELECT col1 from "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCHAR, sizeof(szCHAR), &cbCHAR);
  SPRINTF(buffer, " SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer, " SQLFetch retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, " szCHAR(%ld)='%s'"CRLF, cbCHAR, szCHAR);
  PRINTF(buffer);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " SQLFreeStmt (SQL_DROP) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

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
