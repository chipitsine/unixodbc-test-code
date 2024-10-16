/*!***************************************************************************

  module:        o3c2sqltest.c

  -------------------------------------------------------------------------

  responsible:   Anton Arsenov

  special area:  Unicode conversion tests

  description:   Tests for converting to CHAR UNICODE, VARCHAR UNICODE, LONG VARCHAR UNICODE
				 from:
				 SQL_C_CHAR, SQL_C_BINARY, SQL_C_BIT
				 SQL_C_SSHORT, SQL_C_USHORT, SQL_C_SHORT,
				 SQL_C_SLONG, SQL_C_ULONG, SQL_S_LONG,
				 SQL_C_STINYINT, SQL_C_UTINYINT, SQL_C_TINYINT,
				 SQL_C_SBIGINT, SQL_C_UBIGINT,
				 SQL_C_NUMERIC, SQL_C_TYPE_DATE,
				 SQL_C_TYPE_TIME, SQL_C_TYPE_TIMESTAMP.

  [optional tags]:

  -------------------------------------------------------------------------


\if EMIT_LICENCE


    ========== licence begin  GPL
    Copyright (c) 2001-2004 SAP AG

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

\endif
*****************************************************************************/


/*===========================================================================*
 *  INCLUDES                                                                 *
 *===========================================================================*/
#include "tst.h"


/*===========================================================================*
 *  DEFINES                                                                  *
 *===========================================================================*/

/*===========================================================================*
 *  FUNCTIONS                                                                *
 *===========================================================================*/

int GetTypeInfo (SQLHDBC hdbc, int version);
int FetchInfo (SQLHSTMT hstmt);


#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFSIZE 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS && retcode != SQL_STILL_EXECUTING) {\
                           SQLError(SQL_NULL_HENV, SQL_NULL_HDBC, hstmtx,\
                              sqlstate, &esq_sql_code, error_txt,\
                                    sizeof(error_txt), &used_error_txt);\
                           SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
                           SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
                           PRINTF(buffer);\
                           SPRINTF(buffer," error_txt: %s"CRLF, error_txt);\
                           PRINTF(buffer);\
                  switch (retcode) {\
                  case(SQL_NO_DATA_FOUND) :\
                  case(SQL_STILL_EXECUTING) :\
                          break;\
                  case(SQL_ERROR) : \
                     if (!STRCMP(sqlstate,"22005") &&\
                        !STRCMP(sqlstate,"22003") &&\
                           !STRCMP(sqlstate,"22008") &&\
                           !STRCMP(sqlstate,"S0002") &&\
                              !STRCMP(sqlstate,"07006"))\
                              { return(16); }\
                     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
                       if (STRCMP(sqlstate,"01004"))\
                          if (STRCMP(sqlstate,"22001"))\
                        { return(16); }\
                     break;\
                  default   :\
                          return(16);\
                          break; } }


UCHAR buffer [BUFSIZE];


int main(int argc, argv_t argv) {
	SQLHENV henv   = SQL_NULL_HENV;
	SQLHDBC hdbc   = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SDWORD pcbValue;
        RETCODE retcode;
        UCHAR sqlstate  [ 10 ];
        SDWORD esq_sql_code;
        UCHAR error_txt  [ 101 ] ;
        SWORD len_error_txt = 100;
        SWORD used_error_txt;

        SQLCHAR result[500];
        SQLINTEGER len;


   ProcessCmdArguments(argc,argv);


/* Allocate environment, connection, and statement handle  */
   retcode = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
   SPRINTF(buffer, " SQLAllocHandle retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                      (SQLPOINTER)SQL_OV_ODBC2, 0);
   SPRINTF(buffer, " SQLSetEnvAttr (SQL_OV_ODBC2) retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLAllocHandle (SQL_HANDLE_DBC, henv, &hdbc);
   SPRINTF(buffer, " SQLSetEnvAttr retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

/* Connect to data source                                  */
  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);


  GetTypeInfo (hdbc, 2);

  SQLDisconnect (hdbc);



/* Allocate environment, connection, and statement handle  */
   retcode = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
   SPRINTF(buffer, " SQLAllocHandle retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                      (SQLPOINTER)SQL_OV_ODBC3, 0);
   SPRINTF(buffer, " SQLSetEnvAttr (SQL_OV_ODBC3) retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

   retcode = SQLAllocHandle (SQL_HANDLE_DBC, henv, &hdbc);
   SPRINTF(buffer, " SQLSetEnvAttr retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(NULL);

/* Connect to data source                                  */
  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);


  GetTypeInfo (hdbc, 3);

  SQLDisconnect (hdbc);

  return 0;
}



int GetTypeInfo (SQLHDBC hdbc, int version)
{
  SQLHSTMT hstmt;
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  retcode = SQLAllocHandle (SQL_HANDLE_STMT, hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocHandle retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetTypeInfo(hstmt, SQL_DATE);
  SPRINTF(buffer," SQLGetTypeInfo (SQL_DATE (%d)) retcode: %d"CRLF, SQL_DATE, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* Fetch the record */
  if (retcode == SQL_SUCCESS)
    FetchInfo (hstmt);

  if (version == 2)
    return 1;

  retcode = SQLAllocHandle (SQL_HANDLE_STMT, hdbc, &hstmt);
  SPRINTF(buffer," SQLAllocHandle retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLGetTypeInfo(hstmt, SQL_TYPE_DATE);
  SPRINTF(buffer," SQLGetTypeInfo (SQL_TYPE_DATE (%d)) retcode: %d"CRLF, SQL_TYPE_DATE, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* Fetch the record */
  if (retcode == SQL_SUCCESS)
    FetchInfo (hstmt);

  return 1;
}

int FetchInfo (SQLHSTMT hstmt)
{
  SQLSMALLINT i, n;     /* columns count */
  char column[128];
  SQLLEN length;
  RETCODE retcode;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  retcode = SQLNumResultCols (hstmt, &n);
  SPRINTF(buffer," SQLNumResultCols retcode: %d (%d)"CRLF, retcode, n);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFetch (hstmt);
  SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);


  for (i=0; i<n; i++) {
    retcode = SQLGetData (hstmt, i+1, SQL_C_CHAR, column, sizeof (column), &length);

    if (retcode != SQL_SUCCESS) {
      SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
      PRINTF(buffer);
      ERROR_CHECK(hstmt);
    }
    else {
      if (length == SQL_NULL_DATA)
        SPRINTF (buffer, "%d.\t<NULL>\n", i);
      else
        SPRINTF (buffer, "%d.\t%s\n", i, column);
      PRINTF(buffer);
    }

  }

 return 1;
}


