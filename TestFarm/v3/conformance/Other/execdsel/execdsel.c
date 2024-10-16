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
#include "tst.h"

#define PROGNAME "execdsel"
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


#if UNIXODBC_TEST_SAP_TEST_MODE
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
				SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
		                PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		     if (esq_sql_code != 1051) \
			  exit(16);\
			  break; } }
#else
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
				SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
		                PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		     if (strcmp(sqlstate,"S0002")) \
			  exit(16);\
			  break; } }
#endif


static HSTMT hstmt = SQL_NULL_HSTMT;
UCHAR driverver [ 11 ];
UCHAR buffer [1024];

SWORD sInt;
UCHAR sChar[100];

RETCODE command ();

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
  SDWORD esq_sql_code=0;
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
  UWORD ipar;

  RETCODE retcode;
  SWORD safe_cnt;

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


  retcode = SQLGetInfo( hdbc,
    SQL_DRIVER_VER,
    (PTR) driverver,
    sizeof(driverver),
    NULL);

  driverver[8] = '\0';

  SPRINTF(buffer,"retcode = %d, driverver = \"%s\""CRLF, retcode, driverver);
  PRINTF(buffer);

  strcpy(stmt, "DROP TABLE ATMP");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_API_SQLEXECDIRECT, SQL_NTS);

  strcpy(stmt, "create table atmp ( f1 int, c1 char (10))");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_API_SQLEXECDIRECT, SQL_NTS);
  ERROR_CHECK(NULL);
  strcpy(stmt, "insert atmp values ( 10, 'ABCDEFGHIJ')");
  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_API_SQLEXECDIRECT, SQL_NTS);
  ERROR_CHECK(NULL);

  strcpy(stmt, "select * from atmp");
  retcode = command( henv, hdbc, stmt, CMD_RESULT, SQL_API_SQLEXECDIRECT, SQL_NTS);
  ERROR_CHECK(NULL);

  strcpy(stmt, "insert atmp values ( ?, ?)");

  ipar =1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_SHORT, SQL_INTEGER, 0, 0, &sInt, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_CHAR, SQL_CHAR, sizeof(sChar)-1, 0, sChar, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %d"CRLF, ipar, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  sInt = 99;
  STRCPY(sChar, "XXXX");

  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_API_SQLEXECDIRECT, SQL_NTS);
  ERROR_CHECK(NULL);

  sInt = 1000;
  STRCPY(sChar, "tausend");

  retcode = command( henv, hdbc, stmt, CMD_NO_RESULT, SQL_API_SQLPREPARE, SQL_NTS);
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
  SDWORD esq_sql_code=0;
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
     if (retcode!=SQL_ERROR) {
        retcode = SQLNumParams (hstmt, &npar);
        SPRINTF(buffer," SQLNumParams (%d) retcode: %d"CRLF, npar, retcode);
        PRINTF(buffer);
        ERROR_CHECK(hstmt);

        switch (cmd_mode) {
	    case (CMD_NO_RESULT) :
		    break;
            case (CMD_RESULT) :
        retcode = SQLNumResultCols (hstmt, &pccol);
        SPRINTF(buffer, " SQLNumResultCols (%d) retcode: %d"CRLF, pccol, retcode);
        PRINTF(buffer);
        ERROR_CHECK(hstmt);

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
  }
  if (retcode == SQL_SUCCESS) {
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %d"CRLF, retcode);
     PRINTF(buffer);
  }
  return (retcode);
}
