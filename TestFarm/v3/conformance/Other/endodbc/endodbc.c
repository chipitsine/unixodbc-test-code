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
#define PROGNAME "endodbc"
#include "tst.h"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define SYSODBCUSER  "SYSODBC"
#define SYSODBCPASSW "SYSODBC"

#define CMD_NO_RESULT (1)
#define CMD_RESULT (2)

#define ERROR_CHECK(hstmtx )      if (retcode != SQL_SUCCESS) {\
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
				SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
		                PRINTF(buffer);\
                switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  return(16);\
			  break; } }

  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt2;
  UCHAR FAR * stmt_ptr;
  UCHAR stmt [ 3000 ];
  UCHAR stmt2 [ 50 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  SDWORD size_1 = 0;
  SDWORD size_2 = 0;
  UCHAR driverver [ 100 ];

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

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;

  UCHAR stuff [ 80 ];

UCHAR buffer [1024];

#define F1_LEN 11
UCHAR szF1[F1_LEN];
SWORD sC1;
SDWORD cbF1, cbC1;

RETCODE command (HDBC hdbc, UCHAR *stmt, SWORD cmd_mode, SDWORD stmt_len);

int main(int argc, argv_t argv)
{
  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, " %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;
  stmt_ptr = (UCHAR FAR*) stmt;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc,
		       DBNODE,
		       (SWORD) SQL_NTS,
		       USERNAME,
		       (SWORD) SQL_NTS,
		       PASSWORD,
		       (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLGetInfo( hdbc, SQL_DRIVER_VER, (PTR) driverver, sizeof(driverver), NULL);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "drop user ");
  STRCAT(stmt, ODBCUSER);
  retcode = command( hdbc, stmt, CMD_NO_RESULT, SQL_NTS);

  STRCPY(stmt, "drop user ");
  STRCAT(stmt, SYSODBCUSER);
  retcode = command( hdbc, stmt, CMD_NO_RESULT, SQL_NTS);

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

  SPRINTF(buffer, "%s ending."CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

RETCODE command (HDBC hdbc, UCHAR *stmt, SWORD cmd_mode, SDWORD stmt_len)
{
  phstmt = (HSTMT FAR *) &hstmt;

  retcode = SQLAllocStmt (hdbc, phstmt);
  SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  if (retcode == SQL_SUCCESS) {
     retcode = SQLExecDirect (hstmt, stmt, stmt_len);
     SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     SPRINTF(buffer,"%s"CRLF, stmt);
     PRINTF(buffer);
     switch (cmd_mode) {
	 case (CMD_NO_RESULT) :
		 break;
         case (CMD_RESULT) :
           while (retcode == SQL_SUCCESS) {
             retcode = SQLFetch (hstmt);
             SPRINTF(buffer," SQLFetch retcode: '%d'"CRLF, retcode);
             PRINTF(buffer);
        }
	   break;
	 default :
           break;
     }
  }
/*  if (retcode == SQL_SUCCESS) */ {
     retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
     SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, retcode);
     PRINTF(buffer);
  }
  return (retcode);
}
