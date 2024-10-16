/*!**********************************************************************

  module: o3nulldefault.c

  -----------------------------------------------------------------------

  responsible:  TTK (D024584)

  special area: ODBC-Tests

  description:  PTS 1118671: binding NULL values by columns with default values failed
                PTS xxx    : using SQL_DEFAULT_PARAM in SQLBindParameter

  -----------------------------------------------------------------------





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



************************************************************************/

#define PROGNAME "o3nulldefault"
#define TABLE PROGNAME

#define ODBCVER 0x0350

#include "o3util.c"
#include "o3cnvtab.h"


int main(int argc, argv_t argv) {
    RETCODE    retcode = SQL_SUCCESS;
    SQLHENV    henv  = SQL_NULL_HENV;
    SQLHDBC    hdbc  = SQL_NULL_HDBC;
    SQLHSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR      textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLINTEGER lValue = 0;
    SQLLEN     len, lIndicator = 0;
    SQLINTEGER data;
    int        testOK;

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */


    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    SPRINTF( textBuffer,
             "SQLAllocHandle\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecDirect(hstmt, (SQLCHAR *)"create table "TABLE"(i integer default 10)", SQL_NTS);
    SPRINTF( textBuffer,
             "SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLPrepare(hstmt, (SQLCHAR *)"insert into "TABLE" values (?)", SQL_NTS);
    SPRINTF( textBuffer,
             "SQLPrepare\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 10, 0, &lValue, 0, &lIndicator);
    SPRINTF( textBuffer,
             "SQLBindParameter\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    /* add default value */
    lIndicator = SQL_DEFAULT_PARAM;
    retcode = SQLExecute(hstmt);
    SPRINTF( textBuffer,
             "SQLExecute\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    /* add NULL value */
    lIndicator = SQL_NULL_DATA;
    retcode = SQLExecute(hstmt);
    SPRINTF( textBuffer,
             "SQLExecute\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    /* add normal value */
    lIndicator = sizeof(lValue);
    lValue = 4165;
    retcode = SQLExecute(hstmt);
    SPRINTF( textBuffer,
             "SQLExecute\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
    SPRINTF( textBuffer,
             "SQLExecute\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );


    retcode = SQLPrepare(hstmt, (SQLCHAR *)"select * from "TABLE"", SQL_NTS);
    SPRINTF( textBuffer,
             "SQLPrepare\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindCol (hstmt, 1, SQL_C_LONG, &data, 0, &len);
    SPRINTF( textBuffer,
             "SQLBindCol\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecute(hstmt);
    SPRINTF( textBuffer,
             "SQLExecute\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    /* read first value */
    data = len = 44;
    retcode = SQLFetch(hstmt);
    SPRINTF( textBuffer,
             "SQLFetch\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "\tfetched\t: %ld(%ld)"CRLF,
             data, len);
    PRINTF( textBuffer );
    testOK = data == 10 && len == 4;

    /* read second value */
    data = len = 44;
    retcode = SQLFetch(hstmt);
    SPRINTF( textBuffer,
             "SQLFetch\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "\tfetched\t: %ld(%ld)"CRLF,
             data, len);
    PRINTF( textBuffer );
    testOK |= data == 44 && len == -1;

    /* read second value */
    data = len = 44;
    retcode = SQLFetch(hstmt);
    SPRINTF( textBuffer,
             "SQLFetch\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "\tfetched\t: %ld(%ld)"CRLF,
             data, len);
    PRINTF( textBuffer );
    testOK |= data == 4165 && len == 4;

    retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
    SPRINTF( textBuffer,
             "SQLExecute\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecDirect(hstmt, (SQLCHAR *)"drop table "TABLE"", SQL_NTS);
    SPRINTF( textBuffer,
             "SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    FullDisconnect( &henv, &hdbc );

    if (testOK)
      SPRINTF (textBuffer, "Test ended: OK\n");
    else
      SPRINTF (textBuffer, "Test failed\n");
    PRINTF( textBuffer );

    EndTest( PROGNAME );
 exit:
    return 0;
}; /* main */

