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
#include <tst.h>

#define PROGNAME "23000"
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
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ return(16); }\
		     break;\
		  default   :\
		       if (STRCMP(sqlstate,"23000"))\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [BUFSIZE];
#define MAX_COL 30
#define LONGVARCHAR_LEN 65000
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
UWORD col;
SWORD testlen, hostvarlen;
SCHAR *ptr, c;

#define CHARCOL_LEN 11
UCHAR szCHARCOL[CHARCOL_LEN];
UCHAR FAR *pHostVar;
#define INTCOL_LEN sizeof(sINTCOL)
SDWORD sINTCOL;
SDWORD cbCHARCOL, cbINTCOL;
SDWORD cbHOSTVAR;

UCHAR szCHAR[11];
double  sDOUBLE;
double  sDECIMAL;
SQLLEN cbDOUBLE, cbCHAR, cbDECIMAL, cbValue;

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT hstmt1 = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer,PROGNAME" running."CRLF);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer," SQLAllocEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE \""TABLE"\"", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "create table \""TABLE"\" (col1 char (10) not null)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
     SQL_CHAR, 10, 0, NULL, 0, &cbValue);
  SPRINTF(buffer," SQLBindParameter retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  cbValue = SQL_NULL_DATA;

  retcode = SQLExecDirect (hstmt, "insert \""TABLE"\" values (?)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt (SQL_DROP) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt1);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
