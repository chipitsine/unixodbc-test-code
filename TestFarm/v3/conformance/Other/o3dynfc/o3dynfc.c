/*!**********************************************************************

  module: o3diag.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Getting dynamic function code from diag fields

  -----------------------------------------------------------------------





    ========== licence begin  GPL
    Copyright (c) 1998-2004 SAP AG

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

#define PROGNAME "O3DYNFUNC"

#define ODBCVER 0x0350

#include "o3util.c"

RETCODE InsertStmt( HSTMT hstmt );

RETCODE SelectStmt( HSTMT hstmt );

RETCODE CloseStmt( HSTMT hstmt );

void ShowDynFunc( HSTMT hstmt );

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HDBC    hdbc  = SQL_NULL_HDBC;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = CreateTable( hdbc, PROGNAME, "( a long byte, b int )" );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    /* INSERT */
    retcode = InsertStmt( hstmt );
    if (retcode != SQL_SUCCESS)
        goto exit;

    ShowDynFunc( hstmt );

    retcode = CloseStmt( hstmt );
    if (retcode != SQL_SUCCESS)
        goto exit;

    /* SELECT */
    retcode = SelectStmt( hstmt );
    if (retcode != SQL_SUCCESS)
        goto exit;

    ShowDynFunc( hstmt );

    retcode = CloseStmt( hstmt );
    if (retcode != SQL_SUCCESS)
        goto exit;

exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );

    return 0;
} /* main */


RETCODE CloseStmt( HSTMT hstmt )
{
    RETCODE retcode;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "CloseStmt\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* CloseStmt */


RETCODE InsertStmt( HSTMT hstmt )
{
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    RETCODE retcode;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    SPRINTF( stmtStr, "insert into %s values (?,?)", PROGNAME );
    SPRINTF( textBuffer, "InsertStmt\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "InsertStmt\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* InsertStmt */

RETCODE SelectStmt( HSTMT hstmt )
{
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    RETCODE retcode;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    SPRINTF( stmtStr, "select * from %s", PROGNAME );
    SPRINTF( textBuffer, "SelectStmt\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "SelectStmt\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* SelectStmt */


void ShowDynFunc( HSTMT hstmt )
{
    RETCODE retcode;
    char dynFunc[100];
    SDWORD dynFuncCode = 0;
    SQLSMALLINT len;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLGetDiagField( SQL_HANDLE_STMT, hstmt, 0,
                               SQL_DIAG_DYNAMIC_FUNCTION,
                               dynFunc, sizeof( dynFunc ), &len );
    SPRINTF( textBuffer,
             "ShowDynFunc\t -- GetDiagField \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLGetDiagField( SQL_HANDLE_STMT, hstmt, 0,
                               SQL_DIAG_DYNAMIC_FUNCTION_CODE,
                               &dynFuncCode, 0, NULL );
    SPRINTF( textBuffer,
             "ShowDynFunc\t -- GetDiagField \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    SPRINTF( textBuffer, " DynFuncCode = %d  DynFunc = %s"CRLF,
             dynFuncCode, dynFunc );
    PRINTF( textBuffer );

} /* ShowDynFunc */
