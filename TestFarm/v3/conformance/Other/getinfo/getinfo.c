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
#define PROGNAME "getinfo"
#include "tst.h"

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
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt2;
  UCHAR stmt [ 50 ]/* =
	     " insert into file91 values ('x', 99) "*/;
   UCHAR stmt2 [ 50 ]/* =
	     " select * from file91 "*/;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
  SWORD len_error_txt = ERROR_TEXT_LEN;
  SWORD used_error_txt;
  SDWORD size_1 = 0;
  SDWORD size_2 = 0;

  UCHAR name [ 21 ]/*= "nothing yet"*/;
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

  ProcessCmdArguments(argc,argv);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
     ODBCUSER, (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLGetInfo( hdbc, SQL_DRIVER_VER,
     (PTR) driverver, sizeof(driverver), NULL);

  SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  driverver[8] = '\0';
  SPRINTF(buffer,"SQL_DRIVER_VER = %s"CRLF, driverver);
  PRINTF(buffer);

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

  SPRINTF(buffer,""CRLF);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);
  return(0);
}
