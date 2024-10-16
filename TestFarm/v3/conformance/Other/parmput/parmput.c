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
#define PROGNAME "parmput"
#include "tst.h"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define TABLE PROGNAME

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
				    sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		          if (STRCMP(sqlstate, "07006"))\
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

void dspprm(  UWORD, SWORD FAR, UDWORD FAR, SWORD FAR, SWORD FAR);

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
  SWORD     *pcbInfoValue;


  /* Input SqlDescribeParam */
  UWORD ipar;
  /* Output SqlDescribeParam */
  SWORD   fSqlType;
  SQLULEN cbColDef;
  SWORD   ibScale;
  SWORD   fNullable;

  /* Input SetParam */
  SWORD sInt;
  UCHAR sChar[100];

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
  retcode = SQLExecDirect (hstmt, "create table PARAMPUT (i1 int, c10 char(10))", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLPrepare (hstmt, "insert PARAMPUT values (? ,?)", SQL_NTS);
  SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar =1;
  retcode = SQLDescribeParam (hstmt, ipar, &fSqlType, &cbColDef, &ibScale, &fNullable);
  SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  dspprm(ipar, fSqlType, cbColDef, ibScale, fNullable);

  ipar = 2;
  retcode = SQLDescribeParam (hstmt, ipar, &fSqlType, &cbColDef, &ibScale, &fNullable);
  SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  dspprm(ipar, fSqlType, cbColDef, ibScale, fNullable);

  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_SHORT, SQL_INTEGER, 0, 0, &sInt, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, sizeof(sChar)-1, 0, sChar, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* Assign Input Values */
  sInt = 99;
  STRCPY(sChar, "XXXX");

  SPRINTF(buffer,"Assign input values %d '%s'"CRLF, sInt, sChar);
  PRINTF(buffer);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(sChar, "1234");
  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, STRLEN(sChar), 0, sChar, NULL);
  SPRINTF(buffer," SQLSetParam (%d) '%s' retcode: %d"CRLF, ipar, sChar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(sChar, "5678");
  ipar = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_INTEGER, 10, 0, sChar, NULL);
  SPRINTF(buffer," SQLSetParam (%d) '%s' retcode: %d"CRLF, ipar, sChar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  sInt = 0X7FFFFFFF;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_SHORT, SQL_CHAR, 10, 0, &sInt, NULL);
  SPRINTF(buffer," SQLSetParam (%d) %d retcode: %d"CRLF, ipar, sInt, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
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

VOID dspprm(
UWORD ipar,
SWORD  FAR fSqlType,
UDWORD FAR cbColDef,
SWORD FAR ibScale,
SWORD FAR fNullable)
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
