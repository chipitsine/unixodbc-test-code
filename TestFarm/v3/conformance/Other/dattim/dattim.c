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
#define PROGNAME "dattim"
#include "tst.h"

#ifndef DEFAULTS
#define DBNODE "sqlprec:C"
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
				    sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
                  PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %s"CRLF, error_txt);\
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

#define DATE_LEN 10
#define TIME_LEN 8
#define FRAC_LEN 6
#define SEP_LEN  1
#define TIMESTAMP_LEN DATE_LEN +SEP_LEN + TIME_LEN + SEP_LEN + FRAC_LEN

UCHAR szDATE[DATE_LEN+1], szTIME[TIME_LEN+1], szTIMESTAMP[TIMESTAMP_LEN+1],
      szTIMESTAMP2[TIMESTAMP_LEN+1];
SQLLEN cbDATE, cbTIME, cbTIMESTAMP, cbTIMESTAMP2;
DATE_STRUCT sDATE;
TIME_STRUCT sTIME;
TIMESTAMP_STRUCT sTIMESTAMP, sTIMESTAMP2;

RETCODE getrowcount(HSTMT);

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
   SDWORD esq_sql_code=0;
   UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ];
   SWORD len_error_txt = ERROR_TEXT_LEN;
   SWORD used_error_txt;

   RETCODE retcode;
   SWORD i;
   SDWORD FAR pcrow;

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

   STRCPY(stmt, "create table dattim ( \"DATE\" date , \"TIME\" time, \"TIMESTAMP\" timestamp)");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);

   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

   STRCPY(stmt, "insert dattim values ( { d '1993-10-29'}, { t '14:32:09'}, { ts '1993-10-29 14:32:09.01'})");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

   STRCPY(stmt, "insert dattim values ( '1994-02-11', '11:35:09', '1994-02-11 11:35:09.01' )");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   STRCPY(stmt, "insert dattim values ( { d '1994-12-21'}, { t '12:45:00'}, { ts '1994-12-21 12:45:00.12'} )");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = getrowcount(hstmt);
   ERROR_CHECK(hstmt);

   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

   STRCPY(stmt, "select * from dattim");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = getrowcount(hstmt);
   ERROR_CHECK(hstmt);
   MEMSET(szDATE, '*', sizeof(szDATE));
   retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szDATE,
      DATE_LEN+1, &cbDATE);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);
   MEMSET(szTIME, '*', sizeof(szTIME));
   retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szTIME,
      TIME_LEN+1, &cbTIME);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);
   MEMSET(szTIME, '*', sizeof(szTIME));
   retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szTIMESTAMP,
      TIMESTAMP_LEN+1, &cbTIMESTAMP);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLFetch (hstmt);
   SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = getrowcount(hstmt);
   ERROR_CHECK(hstmt);

   SPRINTF(buffer, "DATE = '%s' : %d TIME  = '%s' : %d TIMESTAMP  = '%s' : %d"CRLF, szDATE, cbDATE, szTIME, cbTIME, szTIMESTAMP, cbTIMESTAMP);
   PRINTF(buffer);

   retcode = SQLFreeStmt(hstmt, SQL_UNBIND);
   SPRINTF(buffer," SQLFreeStmt (SQL_UNBIND) retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLBindCol (hstmt, 1, SQL_C_DATE, &sDATE,
      sizeof(DATE_STRUCT), &cbDATE);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLBindCol (hstmt, 2, SQL_C_TIME, &sTIME,
      sizeof(TIME_STRUCT), &cbTIME);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLBindCol (hstmt, 3, SQL_C_TIMESTAMP, &sTIMESTAMP,
      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLFetch (hstmt);
   SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = getrowcount(hstmt);
   ERROR_CHECK(hstmt);

   SPRINTF(buffer, "DATE = %d,%d,%d TIME  = %d,%d,%d TIMESTAMP = %d,%d,%d %d,%d,%d.%09d"CRLF,
       sDATE.year, sDATE.month, sDATE.day,
       sTIME.hour, sTIME.minute, sTIME.second,
       sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
       sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second, sTIMESTAMP.fraction );
   PRINTF(buffer);

   retcode = SQLFreeStmt(hstmt, SQL_UNBIND);
   SPRINTF(buffer," SQLFreeStmt (SQL_UNBIND) retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLGetData (hstmt, 1, SQL_C_TIMESTAMP, &sTIMESTAMP,
      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
   SPRINTF(buffer," SQLGetData retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   SPRINTF(buffer, "TIMESTAMP = %d,%d,%d,%d,%d,%d,%d"CRLF,
       sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
       sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second, sTIMESTAMP.fraction );
   PRINTF(buffer);

   retcode = SQLGetData (hstmt, 2, SQL_C_TIMESTAMP, &sTIMESTAMP,
      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
   SPRINTF(buffer," SQLGetData retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   SPRINTF(buffer, "TIMESTAMP = %d,%d,%d,%d,%d,%d,%d"CRLF,
       sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
       sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second, sTIMESTAMP.fraction );
   PRINTF(buffer);

   retcode = SQLGetData (hstmt, 3, SQL_C_TIMESTAMP, &sTIMESTAMP,
      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
   SPRINTF(buffer," SQLGetData retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   SPRINTF(buffer, "TIMESTAMP = %d,%d,%d,%d,%d,%d,%d"CRLF,
       sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
       sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second, sTIMESTAMP.fraction );
   PRINTF(buffer);

   retcode = SQLFreeStmt(hstmt, SQL_UNBIND);
   SPRINTF(buffer," SQLFreeStmt (SQL_UNBIND) retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLBindCol (hstmt, 1, SQL_C_TIMESTAMP, &sTIMESTAMP,
      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLBindCol (hstmt, 2, SQL_C_TIMESTAMP, &sTIMESTAMP2,
      sizeof(TIMESTAMP_STRUCT), &cbTIMESTAMP2);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = SQLFetch (hstmt);
   SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   retcode = getrowcount(hstmt);
   ERROR_CHECK(hstmt);

   SPRINTF(buffer, "TIMESTAMP = %d,%d,%d,%d,%d,%d,%d"CRLF,
       sTIMESTAMP.year, sTIMESTAMP.month, sTIMESTAMP.day,
       sTIMESTAMP.hour, sTIMESTAMP.minute, sTIMESTAMP.second, sTIMESTAMP.fraction );
   PRINTF(buffer);

   SPRINTF(buffer, "TIMESTAMP GREPWEG(%d,%d,%d,%d,%d,%d,%d)"CRLF,
       sTIMESTAMP2.year, sTIMESTAMP2.month, sTIMESTAMP2.day,
       sTIMESTAMP2.hour, sTIMESTAMP2.minute, sTIMESTAMP2.second, sTIMESTAMP2.fraction );
   PRINTF(buffer);

   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

   STRCPY(stmt, "drop table dattim");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

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

   SPRINTF(buffer," %s ending"CRLF, PROGNAME);
   PRINTF(buffer);

   return(0);
}

RETCODE getrowcount(hstmt)
HSTMT hstmt;
{
   RETCODE retcode;
   SQLLEN FAR pcrow;

   retcode = SQLRowCount(hstmt, &pcrow);
   SPRINTF(buffer," SQLRowCount retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   SPRINTF(buffer,"affected rows = %d."CRLF, pcrow);
   PRINTF(buffer);
   return(retcode);
}
