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
#define PROGNAME "passtr"

#define SQL_CONNECT_OPT_DRVR_START 1000
#define SQL_PASSTHROUGH SQL_CONNECT_OPT_DRVR_START+1
/* Factor for type SQL Datatypes numbers */
#define SQL_OUTPRM_FACTOR 100
#include "tst.h"

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
		     if (!STRCMP(sqlstate,"24000"))\
			break;\
		     if (!STRCMP(sqlstate,"37000"))\
			break;\
		  default   :\
			  exit(16);\
			  break; } }


static HSTMT hstmt = SQL_NULL_HSTMT;
UCHAR driverver [ 11 ];
UCHAR buffer [1024];

SWORD sInt;
UCHAR szDATE[100];
SQLLEN cbDATE;
SWORD npar;

/* Output SqlDescribeParam */
SWORD ipar;
SWORD  FAR fSqlType;
SQLULEN cbColDef;
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
  int i;

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
  SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLSetStmtOption (hstmt, SQL_PASSTHROUGH, TRUE);
  SPRINTF(buffer," SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "select date into ? from dual");
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
  cbDATE = sizeof(szDATE);
  retcode = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT_OUTPUT, SQL_C_CHAR, SQL_DATE, sizeof(szDATE)-1, 0, szDATE, cbDATE, &cbDATE);
  SPRINTF(buffer," SQLBindParameter (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  MEMSET(szDATE, '*', sizeof(szDATE));
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer,"szDATE=GREPWEG('%s')"CRLF, szDATE);
  PRINTF(buffer);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "select "TABLE" (date) from dual");
  retcode = SQLPrepare (hstmt, stmt, SQL_NTS);
  SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  if (retcode == SQL_ERROR) {
     if (!STRCMP(sqlstate,"37000")) {
	switch (esq_sql_code) {
	   case (-4023): {}
	   case (-3008): {
	      retcode = SQLSetStmtOption (hstmt, SQL_PASSTHROUGH, TRUE);
	      SPRINTF(buffer," SQLSetStmtOption retcode: %d"CRLF, retcode);
	      PRINTF(buffer);
	      ERROR_CHECK(hstmt);
	      SPRINTF(buffer,"%s"CRLF, stmt);
	      PRINTF(buffer);
	      retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
	      SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
	      PRINTF(buffer);
	      ERROR_CHECK(hstmt);
	      STRCPY(stmt, "fetch "TABLE" into ?");
	      SPRINTF(buffer,"%s"CRLF, stmt);
	      PRINTF(buffer);

	      ipar=1;
	      cbDATE = sizeof(szDATE);
	      retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_DATE, sizeof(szDATE)-1, 0, szDATE, &cbDATE);
	      SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
	      PRINTF(buffer);
	      ERROR_CHECK(hstmt);

	      MEMSET(szDATE, '*', sizeof(szDATE));
	      retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
	      SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
	      PRINTF(buffer);
	      ERROR_CHECK(hstmt);
	      SPRINTF(buffer,"szDATE=GREPWEG('%s')"CRLF, szDATE);
	      PRINTF(buffer);
	   }
	}
     }
  }
  else {
     retcode = SQLExecute(hstmt);
     SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szDATE, sizeof(szDATE)-1, &cbDATE);
     SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFetch (hstmt);
     SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     SPRINTF(buffer,"szDATE=GREPWEG('%s')"CRLF, szDATE);
     PRINTF(buffer);
  }

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

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

RETCODE command ( henv, hdbc, stmt, cmd_mode, cmd_type, stmt_len)
  HENV henv;
  HDBC hdbc;
  UCHAR *stmt;
  SWORD cmd_mode;
  SWORD cmd_type;
  SDWORD stmt_len;

{
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  HSTMT FAR * phstmt;
  /* Output SqlNumResultCols */
  SWORD pccol;
  /* Output SqlNumParams */
  SWORD npar;

  phstmt = (HSTMT FAR *) &hstmt;
  retcode = SQL_SUCCESS;
  if (hstmt == SQL_NULL_HSTMT) {
     retcode = SQLAllocStmt (hdbc, phstmt);
     SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
     PRINTF(buffer);
  }
  if (retcode == SQL_SUCCESS) {
     SPRINTF(buffer,"%s"CRLF, stmt);
     PRINTF(buffer);
     if (cmd_type == SQL_API_SQLEXECDIRECT) {
	retcode = SQLExecDirect (hstmt, stmt, stmt_len);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
     }
     else {
	retcode = SQLPrepare (hstmt, stmt, stmt_len);
	SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	retcode = SQLExecute (hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
     }
     retcode = SQLNumResultCols (hstmt, &pccol);
     SPRINTF(buffer, " SQLNumResultCols (%d) retcode: %d"CRLF, pccol, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLNumParams (hstmt, &npar);
     SPRINTF(buffer," SQLNumParams (%d) retcode: %d"CRLF, npar, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     switch (cmd_mode) {
	 case (CMD_NO_RESULT) :
		 break;
         case (CMD_RESULT) :
	    while (retcode == SQL_SUCCESS) {
	       retcode = SQLFetch (hstmt);
	       SPRINTF(buffer," SQLFetch retcode: '%d'"CRLF, retcode);
	       PRINTF(buffer);
	       ERROR_CHECK(hstmt);
	    }
	    if (retcode == SQL_NO_DATA_FOUND)
	        retcode = SQL_SUCCESS;
	    break;
	 default :
           break;
     }
  }
  if (retcode == SQL_SUCCESS) {
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) %#08lx retcode: %d"CRLF, hstmt, retcode);
     PRINTF(buffer);
  }
  return (retcode);
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
