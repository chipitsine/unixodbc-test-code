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

#define PROGNAME "conmix"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define MAX_CONNECTIONS 16
#define USER1  ODBCUSER
#define PASSW1 ODBCPASSW

#define USER2   USER1
#define PASSW2  PASSW1

#define ERROR_CHECK(henvx, hdbcx, hstmtx) if (retcode != SQL_SUCCESS) {\
			   SQLError(henvx,\
				    hdbcx,\
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

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc[MAX_CONNECTIONS];
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
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
  SWORD i;

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
  ERROR_CHECK(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);

  i = 2;
  retcode = SQLAllocConnect(henv, &hdbc[i]);
    retcode = SQLConnect(hdbc[i], DBNODE, (SWORD) SQL_NTS, USER1, (SWORD) SQL_NTS,
		       PASSW1, (SWORD) SQL_NTS );
  SPRINTF(buffer, " %d. SQLConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 0;
  retcode = SQLAllocConnect(henv, &hdbc[i]);
  retcode = SQLConnect(hdbc[i], DBNODE, (SWORD) SQL_NTS, USER1, (SWORD) SQL_NTS,
		       PASSW1, (SWORD) SQL_NTS );
  SPRINTF(buffer, " %d. SQLConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 1;
  retcode = SQLAllocConnect(henv, &hdbc[i]);
  retcode = SQLConnect(hdbc[i], DBNODE, (SWORD) SQL_NTS, USER1, (SWORD) SQL_NTS,
		       PASSW1, (SWORD) SQL_NTS );
  SPRINTF(buffer, " %d. SQLConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 2;
  retcode = SQLDisconnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLDisconnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 3;
  retcode = SQLAllocConnect(henv, &hdbc[i]);
  retcode = SQLConnect(hdbc[i], DBNODE, (SWORD) SQL_NTS, USER1, (SWORD) SQL_NTS,
		       PASSW1, (SWORD) SQL_NTS );
  SPRINTF(buffer, " %d. SQLConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 0;
  retcode = SQLDisconnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLDisconnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 1;
  retcode = SQLDisconnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLDisconnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 3;
  retcode = SQLDisconnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLDisconnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  i = 2;
  retcode = SQLFreeConnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLFreeConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);
  i = 1;
  retcode = SQLFreeConnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLFreeConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);
  i = 0;
  retcode = SQLFreeConnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLFreeConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);
  i = 3;
  retcode = SQLFreeConnect(hdbc[i]);
  SPRINTF(buffer, " %d. SQLFreeConnect retcode: %d"CRLF, i, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, hdbc[i], SQL_NULL_HSTMT);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);

  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
