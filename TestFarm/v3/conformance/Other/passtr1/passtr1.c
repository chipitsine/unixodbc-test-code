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
#define SQL_CONNECT_OPT_DRVR_START 1000
#define SQL_PASSTHROUGH SQL_CONNECT_OPT_DRVR_START+1
/* Factor for type SQL Datatypes numbers */
#define SQL_OUTPRM_FACTOR 100
#include "tst.h"

#define PROGNAME "PASSTR1"
#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define CMD_NO_RESULT (1)
#define CMD_RESULT (2)

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			        SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
		                PRINTF(buffer);\
				SPRINTF(buffer," esq_sql_code: %ld:%s"CRLF, esq_sql_code, error_txt);\
		                PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (!STRCMP(sqlstate,"S0002"))\
			      { exit(16); }\
		     break;\
		  default   :\
			  exit(16);\
			  break; } }


static HSTMT hstmt = SQL_NULL_HSTMT;
UCHAR driverver [ 11 ];
UCHAR buffer [1024];

SWORD sInt;
UCHAR szDATE[100];
SDWORD cbDATE;
UCHAR szCHAR[11];
SQLLEN cbCHAR;
UCHAR szNUM[15];
SQLLEN cbNUM;
UCHAR szCHAR83[84];
SQLLEN cbCHAR83;
SWORD npar;

/* Output SqlDescribeParam */
SWORD ipar;
SWORD  FAR fSqlType;
SQLLEN FAR cbColDef;
SWORD FAR ibScale;
SWORD FAR fNullable;

VOID dspprm(UWORD ipar, SWORD  FAR fSqlType, UDWORD FAR cbColDef,
SWORD FAR ibScale, SWORD FAR fNullable);

int main(int argc, argv_t argv)
{
  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
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

  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
     ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "create table "TABLE" (ACCOUNT_KEY CHAR(10), ACCOUNT_VALUE fixed(12), ACCOUNT_FILLER char(83))", SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ('0', 0, 'xxxx')", SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLSetStmtOption (hstmt, SQL_PASSTHROUGH, TRUE);
  SPRINTF(buffer," SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "select * into :a, :b, :c from "TABLE);
  retcode = SQLPrepare (hstmt, stmt, SQL_NTS);
  SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLNumParams (hstmt, &npar);
  SPRINTF(buffer," SQLNumParams (%d) retcode: %d"CRLF, npar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  for (ipar=1; ipar <= npar; ipar++) {
     retcode = SQLDescribeParam (hstmt, ipar, &fSqlType, &cbColDef, &ibScale, &fNullable);
     SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     dspprm(ipar, fSqlType, cbColDef, ibScale, fNullable);
  }

  ipar=1;
  cbCHAR = sizeof(szCHAR);
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, 10, 0, szCHAR, &cbCHAR);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar=2;
  cbNUM = sizeof(szNUM);
  retcode = SQLSetParam (hstmt, ipar, SQL_C_DEFAULT, SQL_DECIMAL, 12, 0, szNUM, &cbNUM);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar=3;
  cbCHAR83 = sizeof(szCHAR83);
  retcode = SQLSetParam (hstmt, ipar, SQL_C_DEFAULT, SQL_CHAR, 83, 0, szCHAR83, &cbCHAR83);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer,"szCHAR='%s' szNUM='%s' szCHAR83='%s'"CRLF, szCHAR, szNUM, szCHAR83);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "drop table "TABLE, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer, PROGNAME" ending."CRLF);
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
 char *str_ptr;
 UCHAR FAR literal[100];
 SWORD  FAR fsqltype;
   str_ptr = literal;
   if (fSqlType >= SQL_OUTPRM_FACTOR)
      fsqltype = fSqlType / SQL_OUTPRM_FACTOR;
   switch (fsqltype) {
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
  SPRINTF(buffer, "par=%d, SqlType(%d)=%s, ColDef=%d, Scale=%d, Nullable=%d"CRLF,
   ipar, fSqlType, str_ptr, cbColDef, ibScale, fNullable);
  PRINTF(buffer);
}
