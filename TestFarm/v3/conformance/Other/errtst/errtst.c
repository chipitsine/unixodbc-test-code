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

#define PROGNAME "ERRTST"
#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFSIZE 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   RETCODE errret;\
			   errret = SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
		  printf(" error_txt: %s"CRLF, error_txt);\
		     if (errret == SQL_SUCCESS)\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (!STRCMP(sqlstate,"22005") &&\
			!STRCMP(sqlstate,"22003") &&\
			   !STRCMP(sqlstate,"22008") &&\
			      !STRCMP(sqlstate,"07006"))\
			      { return(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ return(16); }\
		     break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
#define MAX_COL 30
UWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
UDWORD pcbColDef[MAX_COL];

UCHAR buffer [BUFSIZE];

#define CHAR_LEN 120
#define LONGVARCHAR_LEN 65000
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
double  sDOUBLE;
double  sDECIMAL;
SDWORD cbDOUBLE, cbCHAR, cbLONGVARCHAR, cbDECIMAL;
#define BOUNDVAL 999

SWORD testlen[] = {254, -1};

void fetchout();

HENV henv   = SQL_NULL_HENV;
HDBC hdbc   = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;
SDWORD *pcbValue, cbValue;
PTR rgbValue;

int main(int argc, argv_t argv)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UCHAR SqlState[100];
  SDWORD SqlCode;
  UCHAR ErrorText[512];
  SWORD UsedErrorText;

  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);

  retcode = SQLExecDirect (hstmt, "SELECT * FROM UNKNOWN", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLError(henv, hdbc, hstmt, SqlState, &SqlCode, ErrorText,
     sizeof(ErrorText), &UsedErrorText);
  SPRINTF(buffer, " SQLError retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  SPRINTF(buffer, "'%s'"CRLF"'%d'"CRLF"'%s'"CRLF"'%d'"CRLF, SqlState, SqlCode, ErrorText, UsedErrorText);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "XXXXX", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLError(henv, hdbc, hstmt, SqlState, &SqlCode, ErrorText,
     sizeof(ErrorText), &UsedErrorText);
  SPRINTF(buffer, " SQLError retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  SPRINTF(buffer, "'%s'"CRLF"'%d'"CRLF"'%s'"CRLF"'%d'"CRLF, SqlState, SqlCode, ErrorText, UsedErrorText);
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "create table "TABLE" (x char(10) key)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "CREATE TABLE "TABLE" (X char(10) key)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLError(henv, hdbc, hstmt, SqlState, &SqlCode, ErrorText,
     sizeof(ErrorText), &UsedErrorText);
  SPRINTF(buffer, " SQLError retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ('x')", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLError(henv, hdbc, hstmt, SqlState, &SqlCode, ErrorText,
     sizeof(ErrorText), &UsedErrorText);
  SPRINTF(buffer, " SQLError retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLExecDirect (hstmt, "insert into "TABLE" values ('x')", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLError(henv, hdbc, hstmt, SqlState, &SqlCode, ErrorText,
     sizeof(ErrorText), &UsedErrorText);
  SPRINTF(buffer, " SQLError retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  SPRINTF(buffer, "'%s'"CRLF"'%d'"CRLF"'%s'"CRLF"'%d'"CRLF, SqlState, SqlCode, ErrorText, UsedErrorText);
  PRINTF(buffer);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchout()
{
  RETCODE retcode=SQL_SUCCESS;
  UDWORD len;
  while (retcode == SQL_SUCCESS) {
     cbCHAR=SQL_NULL_DATA;
     cbLONGVARCHAR=SQL_NULL_DATA;
     MEMSET(szLONGVARCHAR, 0, sizeof(szLONGVARCHAR));
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
	SPRINTF(buffer, " SQLFetch retcode: '%s'"CRLF, cnvret(retcode));
	PRINTF(buffer);
	SPRINTF(buffer, "col1(%d) = '%s'"CRLF, cbCHAR, szCHAR);
	PRINTF(buffer);
	len = (szLONGVARCHAR != NULL) ? STRLEN(szLONGVARCHAR) : cbLONGVARCHAR;
	if (len < BUFSIZE)
	   SPRINTF(buffer, "'%.*s'"CRLF, len, szLONGVARCHAR);
	else
	   SPRINTF(buffer, "'%.*s"CRLF"........"CRLF"%.*s'"CRLF,
	      BUFSIZE/3, szLONGVARCHAR, BUFSIZE/3, ((char *)szLONGVARCHAR+len-BUFSIZE/3));
	PRINTF(buffer);
     }
  }
}
