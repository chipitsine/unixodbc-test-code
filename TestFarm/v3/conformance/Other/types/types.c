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

#define PROGNAME "types"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE "sqlprec:D"

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
		  SPRINTF(buffer," error_txt: %s"CRLF, error_txt);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		          if (esq_sql_code != -6000) \
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define MAXCOLS 23

#define C_LEN 0
SWORD sC[MAXCOLS];
SDWORD cbC[MAXCOLS];

UCHAR szDOUBLE[21], szSHORT[21];
SWORD sSHORT;
SDOUBLE sDOUBLE;
SQLLEN cbDOUBLE;

UCHAR szDEC7_2 [ 10 ];
UCHAR szDEC7   [ 9 ];
UCHAR szDEC15_15 [ 19 ];
UCHAR szTESTNAME [ 30 ];
SQLINTEGER sLONG;

SQLLEN cbDEC7_2, cbDEC7, cbDEC15_15, cbSHORT, cbLONG, cbTESTNAME;

void fetchout();
void GetRetcodeText( RETCODE, char *);
RETCODE out1( HSTMT );

int main(int argc, argv_t argv)
{

   HENV henv   = SQL_NULL_HENV;
   HENV FAR * phenv;
   HDBC hdbc   = SQL_NULL_HDBC;
   HDBC FAR * phdbc;
   HSTMT hstmt = SQL_NULL_HSTMT;
   HSTMT FAR * phstmt;
   UCHAR stmt [ 1024 ];
   UCHAR colstr [ 50 ];
   UCHAR sqlstate  [ 10 ];
   SDWORD esq_sql_code;
   UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
   SWORD len_error_txt = ERROR_TEXT_LEN;
   SWORD used_error_txt;

   RETCODE retcode;
   SWORD i;

   ProcessCmdArguments(argc,argv);

   SPRINTF(buffer," %s running."CRLF, PROGNAME);
   PRINTF(buffer);

   phenv = (HENV FAR *) &henv;
   phdbc = (HDBC FAR *) &hdbc;
   phstmt = (HSTMT FAR *) &hstmt;

   retcode = SQLAllocEnv(phenv);
   SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLAllocConnect(henv, phdbc);
   SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
      (SWORD) SQL_NTS,  ODBCPASSW,    (SWORD) SQL_NTS );
   SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLAllocStmt (hdbc, phstmt);
   retcode = SQLExecDirect (hstmt, "DROP TABLE TYPES", SQL_NTS);
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

   STRCPY(stmt, "create table types ( test char(10) key, dec7_2 decimal (7,2), dec15_15 decimal (15,15),\
       f7 fixed (7), sint smallint, i1 integer)");
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

   STRCPY(stmt, "insert types values ( 'zero', 0, 0, 0, 0, 0)");
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
/*                                               12345 12   12345678901234  1234567  12345  123456789  */
   STRCPY(stmt, "insert types values ( 'maxima', 99999.99, .99999999999999, 9999999, 32767, 247483647 )");
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

/*                                                12345 12    12345678901234   1234567   12345   123456789  */
   STRCPY(stmt, "insert types values ( 'minima', -99999.99, -.9999999999999, -9999999, -32768, -247483648 )");
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

   STRCPY(stmt, "insert types values ( 'truncated', 12345, .1234567, 12, 32767, 0 )");
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

   STRCPY(stmt, "insert types values ( 'null', NULL, NULL, NULL, NULL, NULL )");
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

   retcode = SQLExecDirect (hstmt, "select * from types where test = 'zero'", SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);

   retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szTESTNAME,
      20, &cbTESTNAME);
   retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szDEC7_2,
      10, &cbDEC7_2);
   retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szDEC15_15,
      19, &cbDEC15_15);
   retcode = SQLBindCol (hstmt, 4, SQL_C_CHAR, szDEC7,
      9, &cbDEC7);
   retcode = SQLBindCol (hstmt, 5, SQL_C_SHORT, &sSHORT,
      0, &cbSHORT);
   retcode = SQLBindCol (hstmt, 6, SQL_C_LONG, &sLONG,
      0, &cbLONG);

   STRCPY(szTESTNAME, "99999999999");
   cbTESTNAME = SQL_NULL_DATA;
   STRCPY(szDEC7_2, "XXXXXX");
   cbDEC7_2 = SQL_NULL_DATA;
   STRCPY(szDEC15_15, "XXXXXX");
   cbDEC15_15 = SQL_NULL_DATA;
   STRCPY(szDEC7, "****");
   cbDEC7 = SQL_NULL_DATA;
   sSHORT = 88;
   cbSHORT = 88;
   sLONG = 88;
   cbLONG = 88;

   retcode = out1(hstmt);
   if (retcode == SQL_ERROR) goto error_exit;

   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
   retcode = SQLExecDirect (hstmt, "select * from types where test = 'maxima'", SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);

   cbTESTNAME = SQL_NULL_DATA;
   cbDEC7_2 = SQL_NULL_DATA;
   cbDEC15_15 = SQL_NULL_DATA;
   cbDEC7 = SQL_NULL_DATA;
   retcode = out1(hstmt);

   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
   retcode = SQLExecDirect (hstmt, "select * from types where test = 'minima'", SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);

   cbTESTNAME = SQL_NULL_DATA;
   cbDEC7_2 = SQL_NULL_DATA;
   cbDEC15_15 = SQL_NULL_DATA;
   cbDEC7 = SQL_NULL_DATA;
   retcode = out1(hstmt);

   retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szDEC15_15,
      8, &cbDEC15_15);
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
   retcode = SQLExecDirect (hstmt, "select * from types where test = 'truncated'", SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);

   cbTESTNAME = SQL_NULL_DATA;
   cbDEC7_2 = SQL_NULL_DATA;
   cbDEC15_15 = SQL_NULL_DATA;
   cbDEC7 = SQL_NULL_DATA;
   retcode = out1(hstmt);

error_exit:

   retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
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

   SPRINTF(buffer," %s ending"CRLF, PROGNAME);
   PRINTF(buffer);

   return(0);
}

void fetchout (hstmt)
  HSTMT hstmt;
{
  RETCODE retcode;
  SWORD i;
  retcode = SQL_SUCCESS;
  retcode = SQLBindCol (hstmt, 1, SQL_C_DOUBLE, &sDOUBLE,
	                20, &cbDOUBLE);
  retcode = SQLBindCol (hstmt, 2, SQL_C_SHORT, &sSHORT,
	                0, &cbSHORT);
  while (retcode == SQL_SUCCESS) {
    retcode = SQLFetch (hstmt);
    if (retcode == SQL_SUCCESS ){
        SPRINTF(buffer, "DOUBLE = '%f' SHORT  = '%d'"CRLF, sDOUBLE, sSHORT);
        PRINTF(buffer);
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
      PRINTF(buffer);
    }
  }
}

RETCODE out1(HSTMT hstmt)
{
   RETCODE retcode;
   UCHAR msg[50];

   retcode = SQLFetch (hstmt);
   GetRetcodeText(retcode, msg);
   SPRINTF(buffer, "SQLFetch retcode : %i (%s)."CRLF, retcode, msg);
   PRINTF(buffer);

   SPRINTF(buffer, "%s-TEST "CRLF\
      "DEC7_2 = '%s':%d"CRLF\
      "DEC15  = '%s':%d"CRLF\
      "DEC7  = '%s':%d"CRLF\
      "SHORT = '%d':%d"CRLF\
      "LONG = '%i':%d"CRLF,
      szTESTNAME, szDEC7_2, cbDEC7_2,
      szDEC15_15, cbDEC15_15, szDEC7, cbDEC7,
      sSHORT, cbSHORT, sLONG, cbLONG);
   PRINTF(buffer);
   return (retcode);
}

void GetRetcodeText( RETCODE retcode, char *msg)
{
   switch( retcode ) {
      case SQL_SUCCESS:
         STRCPY(msg, "SQL_SUCCESS");
	 break;
      case SQL_ERROR:
         STRCPY(msg, "SQL_ERROR");
	 break;
      case SQL_SUCCESS_WITH_INFO:
         STRCPY(msg, "SQL_SUCCESS_WITH_INFO");
	 break;
      case SQL_NO_DATA_FOUND:
         STRCPY(msg, "SQL_NO_DATA_FOUND");
	 break;
      case SQL_STILL_EXECUTING:
         STRCPY(msg, "SQL_STILL_EXECUTING");
	 break;
      case SQL_INVALID_HANDLE:
         STRCPY(msg, "SQL_INVALID_HANDLE");
	 break;
      default :
         STRCPY(msg, "** unknown **");
	 break;
      }
}
