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
#define PROGNAME "connect"
#include "tst.h"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"
#endif

#define USER1  "\"ODBC\""
#define PASSW1  "odbc"

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
		  SPRINTF(buffer, " esq_sql_code: %d"CRLF, esq_sql_code);\
		  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  case (SQL_ERROR):\
		     if (esq_sql_code != -4008 &&\
		        esq_sql_code != -711 &&\
		        esq_sql_code != -715)\
			return(16);\
			   else\
			      break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

typedef struct {
   UCHAR FAR *szUID;
   UCHAR FAR *szAuthStr;
} TestParmStruct;

static TestParmStruct TestParm[] = {
{"ODBC", "ODBC"},
{"\"ODBC\"", "\"ODBC\""},
{"odbc", "odbc"},
{"\"odbc\"", "\"odbc\""},
{"      ", "       "},
{"\"     \"", "\"      \""},
{"", ""},
{NULL, NULL}
};



int main(int argc, argv_t  argv)
{
  HENV henv;
  HDBC hdbc = SQL_NULL_HANDLE;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
  SWORD len_error_txt = ERROR_TEXT_LEN;
  SWORD used_error_txt;

  RETCODE retcode;

  TestParmStruct *p;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, " %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " 1. SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  for (p = TestParm; p->szUID != NULL && p->szAuthStr != NULL; p++) {
     SPRINTF(buffer, " szUID='%s' szAuthStr='%s'"CRLF, p->szUID, p->szAuthStr);
     PRINTF(buffer);
     retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
	p->szUID, (SWORD) SQL_NTS, p->szAuthStr, (SWORD) SQL_NTS );
     SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(NULL);

     if (retcode == SQL_SUCCESS) {
	retcode = SQLDisconnect(hdbc);
	SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
	PRINTF(buffer);
	ERROR_CHECK(NULL);
     }
  }

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
