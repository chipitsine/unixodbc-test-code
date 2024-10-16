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

#define PROGNAME "TRANSIST"
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
		  SPRINTF(buffer," (%ld):%s"CRLF, esq_sql_code, error_txt);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ exit(16); }\
		     break;\
		  default   :\
		          if (STRCMP(sqlstate,"07001"))\
		          if (STRCMP(sqlstate,"24000"))\
		          if (STRCMP(sqlstate,"S1010"))\
			{ exit(16); }\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [BUFSIZE];
#define MAX_COL 30
#define LONGVARCHAR_LEN 65000
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
UWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
UDWORD pcbColDef[MAX_COL];
SWORD testlen, hostvarlen;
SCHAR *ptr, c;

#define CHARCOL_LEN 11
UCHAR szCHARCOL[CHARCOL_LEN];
UCHAR FAR *pHostVar;
#define INTCOL_LEN sizeof(sINTCOL)
SDWORD sINTCOL;
SQLLEN cbCHARCOL, cbINTCOL;
SDWORD cbHOSTVAR;
SDWORD cNUM;

UCHAR szCHAR[11];
double  sDOUBLE;
double  sDECIMAL;
SDWORD cbDOUBLE, cbCHAR, cbDECIMAL;
UCHAR FAR szDesc[33];
SWORD  cbDesc;
SQLLEN fDesc;

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UWORD ipar;
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
  SWORD pccol;

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;

  UCHAR stuff [ 80 ];

  int i, j, count;

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
  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (col1 integer, col2 integer)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (1, 10)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (2, 20)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, " Prepare and execute a select not fetch"CRLF);
  PRINTF(buffer);

  retcode = SQLPrepare (hstmt, "select * from "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, " Prepare a select statement and fetch one result"CRLF);
  PRINTF(buffer);

  retcode = SQLPrepare (hstmt, "select * from "TABLE" where col1=?", SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColAttributes(hstmt, 1, SQL_COLUMN_COUNT, szDesc, sizeof(szDesc), &cbDesc, &fDesc);
  SPRINTF(buffer, " SQLColAttributes (%d) retcode: %s"CRLF, fDesc, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 1;
  cNUM = 1;
  retcode = SQLSetParam (hstmt, ipar, SQL_C_LONG, SQL_INTEGER, (UDWORD) 0, 0, &cNUM, NULL);
  SPRINTF(buffer," SQLSetParam (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szCHARCOL, sizeof(szCHARCOL), &cbCHARCOL);
  SPRINTF(buffer," SQLGetData retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  SPRINTF(buffer, "'%s':%d"CRLF, szCHARCOL, cbCHARCOL);
  PRINTF(buffer);

  SPRINTF(buffer, " Close the result and execute the select again"CRLF);
  PRINTF(buffer);
  SPRINTF(buffer, " don't bound new columns"CRLF);
  PRINTF(buffer);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColAttributes(hstmt, 1, SQL_COLUMN_COUNT, szDesc, sizeof(szDesc), &cbDesc, &fDesc);
  SPRINTF(buffer, " SQLColAttributes (%d) retcode: %s"CRLF, fDesc, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLPrepare (hstmt, "select col2 from "TABLE" where col2=?", SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  cNUM = 20;
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szCHARCOL, sizeof(szCHARCOL), &cbCHARCOL);
  SPRINTF(buffer," SQLGetData retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  SPRINTF(buffer, "'%s':%d"CRLF, szCHARCOL, cbCHARCOL);
  PRINTF(buffer);

  SPRINTF(buffer, " Reset Params and execute the select again"CRLF);
  PRINTF(buffer);
  SPRINTF(buffer, " returns sqlstate 24000"CRLF);
  PRINTF(buffer);

  retcode = SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
  SPRINTF(buffer," SQLFreeStmt (SQL_RESET_PARAMS) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColAttributes(hstmt, 1, SQL_COLUMN_COUNT, szDesc, sizeof(szDesc), &cbDesc, &fDesc);
  SPRINTF(buffer, " SQLColAttributes (%d) retcode: %s"CRLF, fDesc, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  cNUM = 10;
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer," SQLFreeStmt (SQL_CLOSE) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  cNUM = 10;
  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szCHARCOL, sizeof(szCHARCOL), &cbCHARCOL);
  SPRINTF(buffer," SQLGetData retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  SPRINTF(buffer, "'%s':%d"CRLF, szCHARCOL, cbCHARCOL);
  PRINTF(buffer);

  /****** END TEST *********/

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt (SQL_DROP) retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

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
