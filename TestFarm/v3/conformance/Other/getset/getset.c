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

#define PROGNAME "getset"
#define TABLE PROGNAME

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
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (STRCMP(sqlstate,"HYC00"))\
		     if (STRCMP(sqlstate,"S1009"))\
                       if (STRCMP(sqlstate,"S1C00"))\
                         if (STRCMP(sqlstate,"S1092"))\
                         if (STRCMP(sqlstate,"IM009"))\
			{ return(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		     if (STRCMP(sqlstate,"01S02"))\
			 { return(16); }\
		     break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
char buffer [1024];

#define LAST_OPT 9999
#define OPTION_OUT_OF_RANGE 9998

void dspprm(  UWORD, SWORD FAR, UDWORD FAR, SWORD FAR, SWORD FAR);

#define RET_OK "0"

typedef struct tagOPTSET {
       UWORD fOption;
       UDWORD vParam;
       UCHAR sState  [ 10 ];
       } OPTSET;

OPTSET sConOpt[] = { { SQL_ACCESS_MODE, SQL_MODE_READ_WRITE, RET_OK },
                     { SQL_AUTOCOMMIT, 0, RET_OK },
                     { SQL_AUTOCOMMIT, 3, "S1009" },
                     { SQL_LOGIN_TIMEOUT, 60, RET_OK },
                     /* following only relevant for driver manager */
                     /* { SQL_OPT_TRACE, 1, "0S102" }, */
                     /* { SQL_OPT_TRACEFILE, (UDWORD)NULL, "S1C00" }, */
                     /* { SQL_TRANSLATE_DLL, (UDWORD)NULL, "IM900" }, */
                     { SQL_TRANSLATE_OPTION, 9999, RET_OK },
                     { SQL_TXN_ISOLATION, SQL_TXN_READ_UNCOMMITTED, RET_OK },
                     { OPTION_OUT_OF_RANGE, 0, "S1092" },
		     { LAST_OPT, 0, RET_OK }
		   };

OPTSET sStmtOpt[] = { { SQL_QUERY_TIMEOUT, 60, RET_OK },
		      { SQL_MAX_LENGTH, 4001, RET_OK },
		      { SQL_MAX_ROWS, 1000, RET_OK },
		      { SQL_NOSCAN, TRUE, RET_OK },
		      { SQL_NOSCAN, 99, "S1009" },
/* 		      { SQL_ASYNC_ENABLE, 1, "S1C00" }, does not run on all Unix systems */
/* 		      { SQL_ASYNC_ENABLE, 2, "S1C00" }, */
		      { SQL_BIND_TYPE, SQL_BIND_BY_COLUMN, RET_OK },
                      { OPTION_OUT_OF_RANGE, 0, "S1092" },
		      { LAST_OPT, 0, RET_OK }
		    };


int main(int argc, argv_t argv)
{

  SQLHENV henv   = SQL_NULL_HENV;
  SQLHDBC hdbc   = SQL_NULL_HDBC;
  SQLHSTMT hstmt = SQL_NULL_HSTMT;
  SQLHSTMT hstmt2 = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  UDWORD vParam[20];  /* SQL_OPT_TRACEFILE, SQL_TRANSLATE_DLL return a string */
  PTR    pvParam;
  int i;
  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer," %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc,
		       (UCHAR*)DBNODE,
		       (SWORD) SQL_NTS,
		       (UCHAR*)ODBCUSER,
		       (SWORD) SQL_NTS,
		       (UCHAR*)ODBCPASSW,
		       (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  for( i=0; ( sConOpt[i].fOption != LAST_OPT); i++) {
     retcode = SQLSetConnectOption (hdbc, sConOpt[i].fOption, sConOpt[i].vParam);
     SPRINTF(buffer," SQLSetConnectOption fOption = %d, vParam = %d retcode: %d state = %s"CRLF,
                     sConOpt[i].fOption, sConOpt[i].vParam, retcode, sConOpt[i].sState);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

  for( i=0; ( sConOpt[i].fOption != LAST_OPT); i++) {
     retcode = SQLGetConnectOption (hdbc, sConOpt[i].fOption, vParam);
     SPRINTF(buffer," SQLGetConnectOption fOption = %d, vParam = %d retcode: %d state = %s"CRLF,
                     sConOpt[i].fOption, vParam[0], retcode, sConOpt[i].sState);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  for( i=0; ( sStmtOpt[i].fOption != LAST_OPT); i++) {
     retcode = SQLSetStmtOption (hstmt, sStmtOpt[i].fOption, sStmtOpt[i].vParam);
     SPRINTF(buffer," SQLSetStmtOption fOption = %d vParam = %d retcode: %d state = %s"CRLF,
	sStmtOpt[i].fOption, sStmtOpt[i].vParam, retcode, sStmtOpt[i].sState);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

  for( i=0; ( sStmtOpt[i].fOption != LAST_OPT); i++) {
     retcode = SQLGetStmtOption (hstmt, sStmtOpt[i].fOption, vParam);
     SPRINTF(buffer," SQLGetStmtOption fOption = %d vParam = %d retcode: %d state = %s"CRLF,
	sStmtOpt[i].fOption, vParam[0], retcode, sStmtOpt[i].sState);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hdbc);

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void dspprm(UWORD ipar, SWORD FAR fSqlType,
UDWORD FAR cbColDef, SWORD FAR ibScale, SWORD FAR fNullable)
{
  SPRINTF(buffer, "par=%d, SqlType=%d, ColDef=%d, Nullable=%d"CRLF,
   ipar, fSqlType, cbColDef, ibScale, fNullable);
  PRINTF(buffer);
}

