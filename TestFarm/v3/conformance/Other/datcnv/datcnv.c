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
#define PROGNAME "datcnv"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE "sqlam:A"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
		  printf(" error_txt: %s"CRLF, error_txt);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (STRCMP(sqlstate,"22005"))\
			if (STRCMP(sqlstate,"07006"))\
			   if (STRCMP(sqlstate,"22008"))\
			   if (esq_sql_code != -6000)\
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

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define MAXCOLS 23

#define DATE_LEN 11
#define TIME_LEN 9
#define TIMESTAMP_LEN DATE_LEN + TIME_LEN
#define ODBCDTEXT 10
UCHAR szDATE[DATE_LEN+ODBCDTEXT], szTIME[TIME_LEN+ODBCDTEXT], szTIMESTAMP[TIMESTAMP_LEN+ODBCDTEXT],
      szTIMESTAMP2[TIMESTAMP_LEN+ODBCDTEXT];
SQLLEN cbDATE, cbTIME, cbTIMESTAMP, cbTIMESTAMP2;
DATE_STRUCT sDATE;
TIME_STRUCT sTIME;
TIMESTAMP_STRUCT sTIMESTAMP, sTIMESTAMP2;

typedef struct {
   UCHAR szDATE[DATE_LEN+ODBCDTEXT];
   UCHAR szTIME[TIME_LEN+ODBCDTEXT];
   UCHAR szTIMESTAMP[TIMESTAMP_LEN+ODBCDTEXT];
} TestValuesStruct;

static TestValuesStruct TestValues[] = {
{"{d'0001-02-03'}", "{t'01:02:03'}", "{ts'0001-02-03 04:05:06'}"},
{"'0004-05-06'", "'04:05:06'", "'0004-05-06 07:08:09'"},
{"0007-08-09", "07:08:09", "0007-08-09 10:11:12"},
{"", "", ""}
};

RETCODE getrowcount(HSTMT);

int main(int argc, argv_t argv)
{
   HENV henv   = SQL_NULL_HENV;
   HDBC hdbc   = SQL_NULL_HDBC;
   HSTMT hstmt = SQL_NULL_HSTMT;
   HSTMT hstmt2 = SQL_NULL_HSTMT;
   UCHAR stmt [ 128 ];
   UCHAR stmt2 [ 128 ];
   UCHAR colstr [ 50 ];
   UCHAR sqlstate  [ 10 ];
   SDWORD esq_sql_code;
   UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ];
   SWORD len_error_txt = ERROR_TEXT_LEN;
   SWORD used_error_txt;
   TestValuesStruct *p;
   SWORD row;

   RETCODE retcode;
   SWORD i;
   SDWORD FAR pcrow;

   ProcessCmdArguments(argc,argv);

   SPRINTF(buffer," %s running."CRLF, PROGNAME);
   PRINTF(buffer);

   retcode = SQLAllocEnv(&henv);
   ERROR_CHECK(NULL);

   retcode = SQLAllocConnect(henv, &hdbc);
   ERROR_CHECK(NULL);

   retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
                 (SWORD) SQL_NTS,  ODBCPASSW,    (SWORD) SQL_NTS );
   ERROR_CHECK(NULL);

   retcode = SQLAllocStmt (hdbc, &hstmt);

   retcode = SQLAllocStmt (hdbc, &hstmt2);

   STRCPY(stmt, "create table "TABLE" ( \"ROW\" integer, \"DATE\" date , \"TIME\" time, \"TIMESTAMP\" timestamp)");
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   ERROR_CHECK(hstmt);
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

   retcode = SQLSetParam (hstmt, 1, SQL_C_SHORT, SQL_INTEGER, (UDWORD) 0, 0, &row, NULL);
   ERROR_CHECK(hstmt);

   retcode = SQLSetParam (hstmt, 2, SQL_C_CHAR, SQL_DATE, (UDWORD) 100, 0, &szDATE, NULL);
   ERROR_CHECK(hstmt);

   retcode = SQLSetParam (hstmt, 3, SQL_C_CHAR, SQL_TIME, (UDWORD) 100, 0, &szTIME, NULL);
   ERROR_CHECK(hstmt);

   retcode = SQLSetParam (hstmt, 4, SQL_C_CHAR, SQL_TIMESTAMP, (UDWORD) 100, 0, &szTIMESTAMP, NULL);
   ERROR_CHECK(hstmt);

   retcode = SQLSetParam (hstmt2, 1, SQL_C_SHORT, SQL_INTEGER, (UDWORD) 0, 0, &row, NULL);
   ERROR_CHECK(hstmt);

   retcode = SQLBindCol (hstmt2, 2, SQL_C_CHAR, szDATE,
      sizeof(szDATE), &cbDATE);
   ERROR_CHECK(hstmt2);
   retcode = SQLBindCol (hstmt2, 3, SQL_C_CHAR, szTIME,
      sizeof(szTIME), &cbTIME);
   ERROR_CHECK(hstmt2);
   retcode = SQLBindCol (hstmt2, 4, SQL_C_CHAR, szTIMESTAMP,
      sizeof(szTIMESTAMP), &cbTIMESTAMP);
   ERROR_CHECK(hstmt2);

   STRCPY(stmt, "insert "TABLE" values (?, ? , ?, ?) ");
   STRCPY(stmt2, "select * from "TABLE" where \"ROW\" = ?");

   row = 0;
   for (p=TestValues; p->szDATE[0] != '\0' && p->szTIME[0] != '\0'
      && p->szTIMESTAMP[0] != '\0'; p++) {
      row++;
      STRCPY(szDATE, p->szDATE);
      STRCPY(szTIME, p->szTIME);
      STRCPY(szTIMESTAMP, p->szTIMESTAMP);

      SPRINTF(buffer, "%s"CRLF, stmt);
      PRINTF(buffer);

      retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
      ERROR_CHECK(hstmt);

      SPRINTF(buffer, "'%s' '%s' '%s'"CRLF, szDATE, szTIME, szTIMESTAMP);
      PRINTF(buffer);

      SPRINTF(buffer, "%s"CRLF, stmt2);
      PRINTF(buffer);
      retcode = SQLExecDirect (hstmt2, stmt2, SQL_NTS);
      ERROR_CHECK(hstmt2);

      retcode = getrowcount(hstmt2);
      ERROR_CHECK(hstmt2);

      retcode = SQLFetch (hstmt2);
      ERROR_CHECK(hstmt2);

      SPRINTF(buffer, "'%s' '%s' '%s'"CRLF, szDATE, szTIME, szTIMESTAMP);
      PRINTF(buffer);

      retcode = SQLFreeStmt(hstmt2, SQL_CLOSE);
      ERROR_CHECK(hstmt2);
   }

   retcode = SQLFreeStmt(hstmt2, SQL_DROP);
   ERROR_CHECK(hstmt2);

   STRCPY(stmt, "drop table "TABLE);
   SPRINTF(buffer, "%s"CRLF, stmt);
   PRINTF(buffer);
   retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
   ERROR_CHECK(hstmt);

   retcode = SQLFreeStmt(hstmt, SQL_DROP);
   ERROR_CHECK(hstmt);

   retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
   ERROR_CHECK(NULL);

   retcode = SQLDisconnect(hdbc);
   ERROR_CHECK(NULL);

   retcode = SQLFreeConnect(hdbc);
   ERROR_CHECK(NULL);

   retcode = SQLFreeEnv(henv);
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
   return(retcode);
}
