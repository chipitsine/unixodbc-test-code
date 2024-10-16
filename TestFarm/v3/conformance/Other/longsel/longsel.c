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

#define PROGNAME "LONGSEL"

#define TABLE PROGNAME
#define PROC PROGNAME
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
			   SPRINTF(buffer," sqlstate: %s\n", sqlstate);\
                           PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld\n", esq_sql_code);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
		          if (esq_sql_code != -6000) \
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define MAXCOLS 26

#define C_LEN 0
SWORD sC[MAXCOLS];
SQLLEN cbC[MAXCOLS];
HENV henv   = SQL_NULL_HENV;
HDBC hdbc   = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;
UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ 101 ] ;
SWORD len_error_txt = 100;
SWORD used_error_txt;

int fetchout (HSTMT hstmt);

int main(int argc, argv_t argv)
{

  UCHAR stmt [ 1024 ];
  UCHAR colstr [ 50 ];

  RETCODE retcode;
  SWORD i, val[MAXCOLS];

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer," %s running.\n", PROGNAME);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer," SQLAllocEnv retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc,
		       DBNODE,
		       (SWORD) SQL_NTS,
		       ODBCUSER,
		       (SWORD) SQL_NTS,
		       ODBCPASSW,
		       (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  STRCPY(stmt, "DROP TABLE "TABLE);
  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d\n", retcode);

  STRCPY(stmt, "create table "TABLE" (");
  for (i=0; i < MAXCOLS; i++) {
     if (i==0) {
	SPRINTF( colstr, "c%03d int", i);
	STRCAT( stmt, colstr);
     }
     else {
	SPRINTF( colstr, ", c%03d int ", i);
	STRCAT( stmt, colstr);
     }
  }
  STRCAT( stmt, ")");
  PRINTF(stmt);
  PRINTF("\n");

  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d\n", retcode);

  STRCPY(stmt, "insert longsel values (");
  for (i=0; i < MAXCOLS; i++) {
     if (i == 0) {
	SPRINTF( colstr, "%3d ", i);
	STRCAT( stmt, colstr);
     }
     else {
	SPRINTF( colstr, ", %3d ", i);
	STRCAT( stmt, colstr);
     }
  }
  STRCAT( stmt, ")");
  PRINTF(stmt);
  PRINTF("\n");

  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d\n", retcode);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "insert longsel values (");
  for (i=0; i < MAXCOLS; i++) {
     if (i == 0)
	STRCAT( stmt, "? ");
     else
	STRCAT( stmt, ", ?");
     val[i] = (SWORD)MAXCOLS-i;
     retcode = SQLSetParam (hstmt, (SWORD)(i+1), SQL_C_SHORT, SQL_INTEGER, 0, 0, &val[i], NULL);
     ERROR_CHECK(hstmt);
  }
  STRCAT( stmt, ")");
  PRINTF(stmt);
  PRINTF("\n");

  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d\n", retcode);
  ERROR_CHECK(NULL);
  retcode = SQLFreeStmt(hstmt, SQL_DROP);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  retcode = SQLExecDirect (hstmt,
	                   "select * from longsel",
			   SQL_NTS);
  SPRINTF(buffer," SQLExecDirect retcode: %d\n", retcode);

  fetchout(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer," SQLTransact retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer," %s ending\n", PROGNAME);
  PRINTF(buffer);

  return(0);
}

int fetchout (HSTMT hstmt)
{
  RETCODE retcode;
  SWORD i;
    for (i=0; i < MAXCOLS; i++) {
    retcode = SQLBindCol (hstmt, (UWORD)(i+1), SQL_C_SHORT, &sC[i],
       C_LEN, &cbC[i]);
  }
  while (retcode == SQL_SUCCESS) {
    retcode = SQLFetch (hstmt);
    ERROR_CHECK(hstmt);
    if (retcode == SQL_SUCCESS ){
      for (i=0; i < MAXCOLS; i++) {
         SPRINTF(buffer,"C%3d = %d\n", i, (int) sC[i]);
         PRINTF(buffer);
      }
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %d\n", retcode);
      PRINTF(buffer);
    }
  }
  return 1;
}
