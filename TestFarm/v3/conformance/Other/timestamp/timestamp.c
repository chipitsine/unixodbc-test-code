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

#define PROGNAME "TIMESTAMPTAB"
#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
			      PRINTF(buffer);\
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
			      PRINTF(buffer);\
		  SPRINTF(buffer, " error_txt: %s"CRLF, error_txt);\
			      PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (STRCMP(sqlstate,"22005"))\
			if (STRCMP(sqlstate,"07006"))\
			{ exit(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ exit(16); }\
		     break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ 101 ] ;
SWORD len_error_txt = 100;
SWORD used_error_txt;
HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;

UCHAR buffer [1024];


int main(int argc, argv_t argv)
{
  UWORD ipar;
  RETCODE retcode;
  SQL_TIMESTAMP_STRUCT tstamp;

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
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect(drop) retcode: %d"CRLF, retcode);
  PRINTF(buffer);


  retcode = SQLExecDirect (hstmt, "CREATE TABLE "TABLE" (t timestamp)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect(create) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

#if UNIXODBC_TEST_SAP_TEST_MODE
  retcode = SQLPrepare (hstmt, "insert into "TABLE" set t = ?", SQL_NTS);
#else
  retcode = SQLPrepare (hstmt, "insert into "TABLE" VALUES (?)", SQL_NTS);
#endif
  SPRINTF(buffer, " SQLPrepare(insert) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

   retcode = SQLBindParameter (hstmt, 1, SQL_PARAM_INPUT, SQL_C_TIMESTAMP,
                               SQL_TIMESTAMP, 0, 0, &tstamp, sizeof(tstamp), NULL);
   ERROR_CHECK(hstmt);

   tstamp.year = 2003;
   tstamp.month = 4;
   tstamp.day = 11;
   tstamp.hour = 16;
   tstamp.minute = 12;
   tstamp.second = 11;
   tstamp.fraction = 123456789;

   retcode = SQLExecute(hstmt);
  SPRINTF(buffer, " SQLExecute(insert) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  memset (&tstamp, 0, sizeof(tstamp));

  retcode = SQLBindCol (hstmt, 1, SQL_C_TIMESTAMP, &tstamp, sizeof(tstamp), NULL);
  ERROR_CHECK(hstmt);


  retcode = SQLExecDirect (hstmt, "select * from "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect(select) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer, " SQLFetch retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);


  SPRINTF(buffer, "SQL_C_TIMESTAMP = %d,%d,%d,%d,%d,%d,%d\n", tstamp.year, tstamp.month, tstamp.day,
          tstamp.hour, tstamp.minute, tstamp.second, tstamp.fraction );
  PRINTF (buffer);
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

