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
#define PROGNAME "parmpu1"
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
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

void dspprm( UWORD ipar, SWORD  FAR fSqlType, UDWORD FAR cbColDef,
SWORD FAR ibScale, SWORD FAR fNullable);
void fetchout(HSTMT hstmt);

UCHAR sCharA[254];
UCHAR sCharB[254];
UCHAR sCharC[254];
SQLLEN cbCharA, cbCharB, cbCharC;

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  RETCODE retcode;

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;


  /* Input SqlDescribeParam */
  UWORD ipar;
  /* Output SqlDescribeParam */
  SWORD  FAR fSqlType;
  UDWORD FAR cbColDef;
  SWORD FAR ibScale;
  SWORD FAR fNullable;

  /* Input SetParam */
  SQLLEN sInt;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer," %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
     ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt2);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE ABC", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "CREATE TABLE ABC (A char (10), B char(10), C char(10))", SQL_NTS);

  retcode = SQLPrepare (hstmt, "insert ABC (A, B, C) VALUES (?,?,?)", SQL_NTS);
  SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  MEMSET(sCharA, 0xFF, sizeof(sCharA));
  MEMSET(sCharB, 0xFF, sizeof(sCharB));
  MEMSET(sCharC, 0xFF, sizeof(sCharC));

  MEMSET(sCharA, 'A', 3);
  sInt = 3;
  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, 10, 0, sCharA, &sInt);
  SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  MEMSET(sCharB, 'B', 3);
  sInt = 3;
  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, 10, 0, sCharB, &sInt);
  SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  MEMSET(sCharC, 'C', 3);
  sInt = 3;
  ipar = 3;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, 10, 0, sCharC, &sInt);
  SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  MEMSET(sCharA, 0xFF, sizeof(sCharA));
  MEMSET(sCharB, 0xFF, sizeof(sCharB));
  MEMSET(sCharC, 0xFF, sizeof(sCharC));

  STRCPY(sCharA, "123");
  retcode = SQLSetParam (hstmt, 1, SQL_C_CHAR, SQL_CHAR, sizeof(sCharA), 0,
     sCharA, NULL);
  SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(sCharB, "4567");
  retcode = SQLSetParam (hstmt, 2, SQL_C_CHAR, SQL_CHAR, sizeof(sCharB), 0, sCharB, NULL);
  SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(sCharC, "789");
  retcode = SQLSetParam (hstmt, 3, SQL_C_CHAR, SQL_CHAR, sizeof(sCharC), 0,
     sCharC, NULL);
  SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_UNBIND);

  retcode = SQLExecDirect (hstmt, "select * from ABC", SQL_NTS);
  printf(" SQLExecDirect retcode: %d"CRLF, retcode);
  ERROR_CHECK(hstmt);

  fetchout( hstmt );

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  SPRINTF(buffer," SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void dspprm( UWORD ipar, SWORD  FAR fSqlType, UDWORD FAR cbColDef,
SWORD FAR ibScale, SWORD FAR fNullable)
{
 UCHAR FAR * str_ptr;
 UCHAR FAR literal[100];
   str_ptr = literal;

   switch (fSqlType) {
     case (SQL_CHAR) :
	STRCPY(str_ptr, "SQL_CHAR");
	break;
     case (SQL_NUMERIC) :
	STRCPY(str_ptr, "SQL_NUMERIC");
	break;
     case (SQL_DECIMAL) :
	STRCPY(str_ptr, "SQL_DECIMAL");
	break;
     case (SQL_INTEGER) :
	STRCPY(str_ptr, "SQL_INTEGER");
	break;
     case (SQL_SMALLINT) :
	STRCPY(str_ptr, "SQL_SMALLINT");
	break;
     case (SQL_FLOAT) :
	STRCPY(str_ptr, "SQL_FLOAT");
	break;
     case (SQL_REAL) :
	STRCPY(str_ptr, "SQL_REAL");
	break;
     case (SQL_DOUBLE) :
	STRCPY(str_ptr, "SQL_DOUBLE");
	break;
     case (SQL_VARCHAR) :
	STRCPY(str_ptr, "SQL_VARCHAR");
	break;
     case (SQL_DATE) :
	STRCPY(str_ptr, "SQL_DATE");
	break;
     case (SQL_TIME) :
	STRCPY(str_ptr, "SQL_TIME");
	break;
     case (SQL_TIMESTAMP) :
	STRCPY(str_ptr, "SQL_TIMESTAMP");
	break;
     default :
	SPRINTF(str_ptr, "** unknown type : %d **", fSqlType);
	break;
     }
  SPRINTF(buffer, "par=%d, SqlType=%s, ColDef=%d, Scale=%d, Nullable=%d"CRLF,
   ipar, str_ptr, cbColDef, ibScale, fNullable);
  PRINTF(buffer);
}

void fetchout (HSTMT hstmt)
{
  RETCODE retcode;
  SWORD i;
  retcode = SQL_SUCCESS;

  MEMSET(sCharA, 0xFF, sizeof(sCharA));
  MEMSET(sCharB, 0xFF, sizeof(sCharB));
  MEMSET(sCharC, 0xFF, sizeof(sCharC));

  SPRINTF(buffer, "cbCharA:%d, cbCharB:%d, cbCharC:%d"CRLF, cbCharA, cbCharB, cbCharC );
  PRINTF(buffer);
  retcode = SQLBindCol (hstmt, 1, SQL_C_DEFAULT, sCharA,
     sizeof(sCharA), &cbCharA);
  retcode = SQLBindCol (hstmt, 2, SQL_C_DEFAULT, sCharB,
     sizeof(sCharA), &cbCharB);
  retcode = SQLBindCol (hstmt, 3, SQL_C_DEFAULT, sCharC,
     sizeof(sCharA), &cbCharC);
  while (retcode == SQL_SUCCESS) {
    retcode = SQLFetch (hstmt);
    if (retcode == SQL_SUCCESS ){
        SPRINTF(buffer, "%s:%d"CRLF"%s:%d"CRLF"%s:%d"CRLF,
	   sCharA, cbCharA, sCharB, cbCharB, sCharC, cbCharC );
        PRINTF(buffer);
	cbCharA=cbCharB=cbCharC=0;
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %d\n", retcode);
      PRINTF(buffer);
    }
  }
}

