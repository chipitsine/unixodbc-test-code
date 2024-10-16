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

#define PROGNAME "descsys"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"
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
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];
UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
SWORD len_error_txt = ERROR_TEXT_LEN;
SWORD used_error_txt;
HENV henv   = SQL_NULL_HENV;
HDBC hdbc   = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;


int main(int argc, argv_t argv)
{

  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer," statstc running."CRLF);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
		       ODBCUSER, (SWORD) SQL_NTS,
		       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetTypeInfo (hstmt, SQL_ALL_TYPES);
  SPRINTF(buffer, " SQLGetTypeInfo retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLTables (hstmt, "", SQL_NTS, ODBCUSER, SQL_NTS,
     "TAB1", SQL_NTS, NULL, 0);
  SPRINTF(buffer," SQLTables retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
     "TAB1", SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLColumns retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLStatistics (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
     "TAB1", SQL_NTS, SQL_INDEX_ALL, SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLSpecialColumns (hstmt, SQL_BEST_ROWID, NULL, 0, ODBCUSER, SQL_NTS,
     "TAB1", SQL_NTS, SQL_SCOPE_CURROW, SQL_NO_NULLS);
  SPRINTF(buffer, " SQLSpecialColumns retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLPrimaryKeys (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
     "TAB1", SQL_NTS);
  SPRINTF(buffer, " SQLPrimaryKeys retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* ***** SQLForeignKeys */
  retcode = SQLForeignKeys (hstmt, NULL, 0, ODBCUSER, SQL_NTS, "TAB1", SQL_NTS,
     NULL, 0, ODBCUSER, SQL_NTS, "TAB1", SQL_NTS);
  SPRINTF(buffer, " SQLForeignKeys retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* ***** SQLProcedures */
  retcode = SQLProcedures (hstmt, NULL, 0, ODBCUSER, SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLProcedures retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* ***** SQLProcedureColumns */
  retcode = SQLProcedureColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS, NULL, 0,
  								NULL, 0);
  SPRINTF(buffer, " SQLProcedureColumns retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* ***** SQLTablePrivileges */
  retcode = SQLTablePrivileges (hstmt, NULL, 0, ODBCUSER, SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLTablePrivileges retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* ***** SQLColumnPrivileges */
  retcode = SQLColumnPrivileges (hstmt, NULL, 0, ODBCUSER, SQL_NTS, NULL, 0,
  								NULL, 0);
  SPRINTF(buffer, " SQLColumnPrivileges retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  DisplayColAttributes(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

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
