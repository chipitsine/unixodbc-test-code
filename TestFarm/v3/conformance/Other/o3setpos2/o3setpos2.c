/*!**********************************************************************

  module: o3setpos2.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Use SQLSet with option SQL_UPDATE to send long data with
                SQL_DATA_AT_EXEC (PTS 1104694).

  -----------------------------------------------------------------------





    ========== licence begin  GPL
    Copyright (c) 1999-2004 SAP AG

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

#define PROGNAME "O3SETPOS2"

#define ODBCVER 0x0350

#include "o3util.c"

#define LONG_LEN 4000
#define NUM_VALUES 20


RETCODE InsertValue();
RETCODE ReadValue();

/* globals */
HDBC    hdbc  = SQL_NULL_HDBC;

int main( int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = CreateTable( hdbc, PROGNAME, "( a int, b long byte )" );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = InsertValue();
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ReadValue();
exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */


SQLRETURN InsertValue()
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLROWSETSIZE  rowsFetched = 0;
    UDWORD  intValue  = 17;
    char    longValue[LONG_LEN+1];
    SQLLEN  intLen  = sizeof(UDWORD);
    SQLLEN  longLen = LONG_LEN;
    UWORD   rowStatus;

    InitLongBuf( longValue, LONG_LEN );

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "InsertValue\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF("insert into "PROGNAME" values (17,NULL)\n" );
    retcode = SQLExecDirect( hstmt,
                             "insert into "PROGNAME" values (17,NULL)",
                             SQL_NTS );
    SPRINTF( textBuffer,
             "InsertValue\t -- ExecDirect\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF("insert into "PROGNAME" values (37,NULL)\n" );
    retcode = SQLExecDirect( hstmt,
                             "insert into "PROGNAME" values (37,NULL)",
                             SQL_NTS );
    SPRINTF( textBuffer,
             "InsertValue\t -- ExecDirect\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "InsertValue\t -- FreeStmt(SQL_CLOSE)\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_ROWVER, 0 );
    SPRINTF( textBuffer,
             "InsertValue\t -- SetStmtAttr(CONCURRENCY) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0 );
    SPRINTF( textBuffer,
             "InsertValue\t -- SetStmtAttr(CURSOR_TYPE) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( "select * from "PROGNAME" order by a\n" );
    retcode = SQLExecDirect( hstmt, "select * from "PROGNAME" where a=17",
                             SQL_NTS );
    SPRINTF( textBuffer,
             "InsertValue\t -- ExecDirect   \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt, SQL_RETRIEVE_DATA, (SQLPOINTER)SQL_TRUE, 0 );
    SPRINTF( textBuffer,
             "InsertValue\t -- SetStmtAttr \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLExtendedFetch( hstmt,
                                SQL_FETCH_NEXT, 0, &rowsFetched, &rowStatus );
    SPRINTF( textBuffer,
             "InsertValue\t -- ExtendedFetch \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS && retcode != SQL_NO_DATA_FOUND) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 1, SQL_C_ULONG, &intValue, 0, &intLen );
    SPRINTF( textBuffer,
             "InsertValue\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    longLen = SQL_DATA_AT_EXEC;
    retcode = SQLBindCol( hstmt,
                          2, SQL_C_BINARY, (SQLPOINTER) 2, LONG_LEN, &longLen );
    SPRINTF( textBuffer,
             "InsertValue\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetPos( hstmt, 1, SQL_UPDATE, SQL_LOCK_NO_CHANGE );
    SPRINTF( textBuffer,
             "InsertValue\t -- SetPos(SQL_ADD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS && retcode != SQL_NEED_DATA) {
        return retcode;
    }; /* if */
    if (retcode == SQL_NEED_DATA) {
        SQLLEN paramNo = 0;           /* must be as long as a pointer! */

        retcode = SQLParamData( hstmt, (SQLPOINTER*) &paramNo );
        SPRINTF( textBuffer,
                 "InsertValue\t -- ParamData   \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS && retcode != SQL_NEED_DATA) {
            return retcode;
        }; /* if */
        if (paramNo != 2) {
            PRINTF( "Wrong paramNo !!!\n" );
            return SQL_ERROR;
        };
        retcode = SQLPutData( hstmt, longValue, LONG_LEN );
        SPRINTF( textBuffer,
                 "InsertValue\t -- PuData \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS && retcode != SQL_NEED_DATA) {
            return retcode;
        }; /* if */
        retcode = SQLParamData( hstmt, (SQLPOINTER*) &paramNo );
        SPRINTF( textBuffer,
                 "InsertValue\t -- ParamData   \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
    };

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* InsertValue */


SQLRETURN ReadValue()
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLROWSETSIZE  rowsFetched = 0;
    UDWORD  intValue  = 17;
    char    longValue[LONG_LEN+1];
    SQLLEN  intLen  = sizeof(UDWORD);
    SQLLEN  longLen = sizeof(longValue);

    longValue[0] = '\0';

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "ReadValue\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_ROWVER, 0 );
    SPRINTF( textBuffer,
             "ReadValue\t -- SetStmtAttr(CONCURRENCY) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0 );
    SPRINTF( textBuffer,
             "ReadValue\t -- SetStmtAttr(CURSOR_TYPE) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */


    retcode = SQLBindCol( hstmt, 1, SQL_C_ULONG, &intValue, 0, &intLen );
    SPRINTF( textBuffer,
             "ReadValue\t -- BindCol \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt,
                          2, SQL_C_BINARY, &longValue, LONG_LEN, &longLen );
    SPRINTF( textBuffer,
             "ReadValue\t -- BindCol \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLExecDirect( hstmt, "select * from "PROGNAME, SQL_NTS );
    SPRINTF( textBuffer,
             "ReadValue\t -- ExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLExtendedFetch( hstmt,
                                SQL_FETCH_NEXT, 0, &rowsFetched, NULL );
    SPRINTF( textBuffer,
             "ReadValue\t -- ExtendedFetch \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    if (longLen > 0)
      longValue[longLen] = '\0';
    ShowLongChar( longValue );
    SPRINTF( textBuffer, "intValue = %d; intLen = %d; longLen = %d\n",
             intValue, intLen, longLen );
    PRINTF( textBuffer );

    /* fetch second row */
    longValue[0] = '\0';
    retcode = SQLExtendedFetch( hstmt,
                                SQL_FETCH_NEXT, 0, &rowsFetched, NULL );
    SPRINTF( textBuffer,
             "ReadValue\t -- ExtendedFetch \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    if (longLen > 0)
      longValue[longLen] = '\0';
    ShowLongChar( longValue );
    SPRINTF( textBuffer, "intValue = %d; intLen = %d; longLen = %d\n",
             intValue, intLen, longLen );
    PRINTF( textBuffer );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* ReadValue */
