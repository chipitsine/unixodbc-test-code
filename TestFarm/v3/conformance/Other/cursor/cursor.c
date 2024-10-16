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

#define PROGNAME "cursor"
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

#define ERROR_CHECK(hstmtx)	 if (retcode != SQL_SUCCESS) {\
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
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
		  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define C_LEN 18

UCHAR szCURSOR[C_LEN+1];
SQLLEN cbCURSOR;

#define SET_CURSOR TRUE
#define LAST_CURSOR FALSE
#define RET_OK "0"

typedef struct tagCURSOR {
       UWORD fOption;
       UCHAR sCursorName  [ 19 ];
       UCHAR sStauts  [ 10 ];
       } CURSOR;

CURSOR sCursor[] = { { SET_CURSOR, "CURSOR1", RET_OK },
                     { SET_CURSOR, "lower_cursor", RET_OK },
		     { LAST_CURSOR, "", RET_OK }
		   };



int main(int argc, argv_t argv)
{
  HENV henv;
  HDBC hdbc = SQL_NULL_HANDLE;
  HDBC hdbc2;
  HSTMT hstmt;
  HSTMT hstmt2;
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
  int i;

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;

  UCHAR stuff [ 80 ];

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, " %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, USER1, (SWORD) SQL_NTS,
		       PASSW1, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE CC", SQL_NTS);
  retcode = SQLExecDirect (hstmt, "create table CC (\"cursor\" varchar(18))", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLPrepare (hstmt, "insert cc values (?)", SQL_NTS);
  SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  for( i=0; ( sCursor[i].fOption != LAST_CURSOR); i++) {

     retcode = SQLSetParam (hstmt, 1, SQL_C_CHAR, SQL_CHAR, STRLEN(sCursor[i].sCursorName), 0, sCursor[i].sCursorName, NULL);
     SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     SPRINTF(buffer,"Cursorname='%s'"CRLF, sCursor[i].sCursorName);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecute (hstmt);
     SPRINTF(buffer," SQLExecute (insert) retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  for( i=0; ( sCursor[i].fOption != LAST_CURSOR); i++) {

     retcode = SQLSetParam (hstmt, 1, SQL_C_CHAR, SQL_CHAR, STRLEN(sCursor[i].sCursorName), 0, sCursor[i].sCursorName, NULL);
     SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     SPRINTF(buffer,"Cursorname='%s'"CRLF, sCursor[i].sCursorName);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLSetCursorName (hstmt, sCursor[i].sCursorName,
	(SWORD)STRLEN(sCursor[i].sCursorName));
     SPRINTF(buffer," SQLSetCursorName retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     cbCURSOR=sizeof(szCURSOR);
     retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCURSOR,
      sizeof(szCURSOR), &cbCURSOR);
     SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecDirect (hstmt, "select \"cursor\" from cc where \"cursor\" = ?", SQL_NTS);
     SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFetch (hstmt);
     SPRINTF(buffer, " SQLFetch retcode: '%d'\n", retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     SPRINTF(buffer, " CursorName = %s cursor = %s\n", sCursor[i].sCursorName, szCURSOR);
     PRINTF(buffer);
  }

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* special for testing SQLSetCursor after SQLPrepare */
  for( i=0; ( sCursor[i].fOption != LAST_CURSOR); i++) {

     retcode = SQLSetParam (hstmt, 1, SQL_C_CHAR, SQL_CHAR, STRLEN(sCursor[i].sCursorName), 0, sCursor[i].sCursorName, NULL);
     SPRINTF(buffer," SQLSetParam retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     SPRINTF(buffer,"Cursorname='%s'"CRLF, sCursor[i].sCursorName);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLPrepare (hstmt, "select \"cursor\" from cc where \"cursor\" = ?", SQL_NTS);
     SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLSetCursorName (hstmt, sCursor[i].sCursorName,
	(SWORD)STRLEN(sCursor[i].sCursorName));
     SPRINTF(buffer," SQLSetCursorName retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     cbCURSOR=sizeof(szCURSOR);
     retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szCURSOR,
      sizeof(szCURSOR), &cbCURSOR);
     SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLExecute (hstmt);
     SPRINTF(buffer," SQLExecute retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFetch (hstmt);
     SPRINTF(buffer, " SQLFetch retcode: '%d'\n", retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);

     SPRINTF(buffer, " CursorName = %s cursor = %s\n", sCursor[i].sCursorName, szCURSOR);
     PRINTF(buffer);
  }

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);

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
