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
#define PROGNAME "muconn"
#include "tst.h"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif
#define USER1  ODBCUSER
#define PASSW1  ODBCPASSW

#define USER2   USER1
#define PASSW2  PASSW1

#define ERROR_CHECK(henvx, hdbcx, hstmtx) if (retcode != SQL_SUCCESS) {\
			   SQLError(henvx,\
				    hdbcx,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			   SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
			   PRINTF(buffer);\
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
		  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HDBC hdbc2   = SQL_NULL_HDBC;
  HDBC FAR * phdbc2;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt2;
  UCHAR stmt [ 3000 ];
  UCHAR stmt2 [ 50 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
  SWORD len_error_txt = ERROR_TEXT_LEN;
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
  SWORD safe_cnt;

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;

  UCHAR stuff [ 80 ];

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, " %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phdbc2 = (HDBC FAR *) &hdbc2;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " 1. SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLAllocConnect(henv, phdbc2);
  SPRINTF(buffer, " 2. SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc2, SQL_NULL_HSTMT);

  retcode = SQLConnect(hdbc,
		       DBNODE,
		       (SWORD) SQL_NTS,
		       USER1,
		       (SWORD) SQL_NTS,
		       PASSW1,
		       (SWORD) SQL_NTS );
  SPRINTF(buffer, " 1. SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLConnect(hdbc2,
		       DBNODE,
		       (SWORD) SQL_NTS,
		       USER2,
		       (SWORD) SQL_NTS,
		       PASSW2,
		       (SWORD) SQL_NTS );
  SPRINTF(buffer, " 2. SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc2, SQL_NULL_HSTMT);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "create table muconn1 (id int)", SQL_NTS);
  SPRINTF(buffer, " 1. SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, "select * from muconn1", SQL_NTS);
  SPRINTF(buffer, " 1. SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " 1. SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, hstmt);

  retcode = SQLAllocStmt (hdbc2, phstmt2);
  retcode = SQLExecDirect (hstmt2, "create table muconn2 (name char(10))", SQL_NTS);
  SPRINTF(buffer, " 2. SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt2, "select * from muconn2", SQL_NTS);
  SPRINTF(buffer, " 2. SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc2, hstmt2);
  retcode = SQLFreeStmt(hstmt2, SQL_DROP);
  SPRINTF(buffer, " 2. SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT);
  ERROR_CHECK(henv, hdbc2, hstmt2);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer, " 1. SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLTransact(henv, hdbc2, SQL_ROLLBACK);
  SPRINTF(buffer, " 2. SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc2, SQL_NULL_HSTMT);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " 1. SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLDisconnect(hdbc2);
  SPRINTF(buffer, " 2. SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc2, SQL_NULL_HSTMT);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " 1. SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc, SQL_NULL_HSTMT);

  retcode = SQLFreeConnect(hdbc2);
  SPRINTF(buffer, " 2. SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc2, SQL_NULL_HSTMT);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);

  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
