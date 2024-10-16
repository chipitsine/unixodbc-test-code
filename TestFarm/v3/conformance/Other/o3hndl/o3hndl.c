/*!**********************************************************************

  module: o3hndl.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Tests for SQLAllocHandle, SQLFreeHandle

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

#define PROGNAME "o3hndl"

#define ODBCVER 0x0350

#include "o3util.c"

void ExplicitFreeStmt();
void ExplicitFreeDesc();
void ImplicitFreeStmt();
void ImplicitFreeDesc();
void InvalidFree();

int main(int argc, argv_t argv) {

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    ExplicitFreeStmt();
    ExplicitFreeDesc();
    ImplicitFreeStmt();
    ImplicitFreeDesc();
    InvalidFree();

    EndTest( PROGNAME );

    return 0;
}; /* main */

void ExplicitFreeStmt()
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    HENV henv    = SQL_NULL_HENV;
    HDBC hdbc    = SQL_NULL_HDBC;
    HSTMT hstmt1 = SQL_NULL_HSTMT;
    HSTMT hstmt2 = SQL_NULL_HSTMT;
    RETCODE retcode;

    PRINTF( CRLF"ExplicitFreeStmt"CRLF );
    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */

    /* Allocate Stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt1 );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt1 );

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt2 );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt2 );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt2 );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- FreeHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt2 );
    }; /* if */

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt1 );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- FreeHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt1 );
    }; /* if */

    FullDisconnect( &henv, &hdbc );
}; /* ExplicitFreeStmt */


void ExplicitFreeDesc()
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    HENV henv   = SQL_NULL_HENV;
    HDBC hdbc   = SQL_NULL_HDBC;
    HSTMT hstmt = SQL_NULL_HSTMT;
    SQLHDESC hdesc1 = SQL_NULL_HDESC;
    SQLHDESC hdesc2 = SQL_NULL_HDESC;
    RETCODE retcode;

    PRINTF( CRLF"ExplicitFreeDesc"CRLF );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */

    /* Allocate Stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "ExplicitFreeDesc -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLAllocHandle( SQL_HANDLE_DESC, hdbc, &hdesc1 );
    SPRINTF( textBuffer,
             "ExplicitFreeDesc -- AllocHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc1 );

    retcode = SQLAllocHandle( SQL_HANDLE_DESC, hdbc, &hdesc2 );
    SPRINTF( textBuffer,
             "ExplicitFreeDesc -- AllocHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc2 );

    retcode = SQLFreeHandle( SQL_HANDLE_DESC, hdesc1 );
    SPRINTF( textBuffer,
             "ExplicitFreeDesc -- FreeHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_DESC, hdesc1 );
    }; /* if */

    retcode = SQLFreeHandle( SQL_HANDLE_DESC, hdesc2 );
    SPRINTF( textBuffer,
             "ExplicitFreeDesc -- FreeHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_DESC, hdesc2 );
    }; /* if */

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- FreeHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
    }; /* if */

    FullDisconnect( &henv, &hdbc );
}; /* ExpliciteFreeDesc */


void ImplicitFreeStmt()
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    HENV henv    = SQL_NULL_HENV;
    HDBC hdbc    = SQL_NULL_HDBC;
    HSTMT hstmt1 = SQL_NULL_HSTMT;
    HSTMT hstmt2 = SQL_NULL_HSTMT;
    RETCODE retcode;

    PRINTF( CRLF"ImplicitFreeStmt"CRLF );
    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */

    /* Allocate Stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt1 );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt1 );

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt2 );
    SPRINTF( textBuffer,
             "ExplicitFreeStmt -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt2 );

    FullDisconnect( &henv, &hdbc );
}; /* TestImplicitFreeStmt */

void ImplicitFreeDesc()
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    HENV henv   = SQL_NULL_HENV;
    HDBC hdbc   = SQL_NULL_HDBC;
    HSTMT hstmt = SQL_NULL_HSTMT;
    SQLHDESC hdesc1 = SQL_NULL_HDESC;
    SQLHDESC hdesc2 = SQL_NULL_HDESC;
    RETCODE retcode;

    PRINTF( CRLF"ImplicitFreeDesc"CRLF );
    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */

    /* Allocate Stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "ImplicitFreeDesc -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLAllocHandle( SQL_HANDLE_DESC, hdbc, &hdesc1 );
    SPRINTF( textBuffer,
             "ImplicitFreeDesc -- AllocHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc1 );

    retcode = SQLAllocHandle( SQL_HANDLE_DESC, hdbc, &hdesc2 );
    SPRINTF( textBuffer,
             "ImplicitFreeDesc -- AllocHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc2 );

    FullDisconnect( &henv, &hdbc );
}; /* ImplicitFreeDesc */

void InvalidFree()
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    HENV henv    = SQL_NULL_HENV;
    HDBC hdbc    = SQL_NULL_HDBC;
    HSTMT hstmt1 = SQL_NULL_HSTMT;
    HSTMT hstmt2 = SQL_NULL_HSTMT;
    SQLHDESC hdesc1 = SQL_NULL_HDESC;
    RETCODE retcode;

    PRINTF( CRLF"InvalidFree"CRLF );
    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */

    /* Allocate Stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt1 );

    retcode = SQLAllocHandle( SQL_HANDLE_DESC, hdbc, &hdesc1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- AllocHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc1 );

    PRINTF( "InvalidFree\t -- Unknown handletype"CRLF );
    retcode = SQLFreeHandle( 1254, hdesc1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- FreeHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_DESC, hdesc1 );
    }; /* if */

    PRINTF( "InvalidFree\t -- Wrong handletype"CRLF );
    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hdesc1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- FreeHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_DESC, hdesc1 );
    }; /* if */

    retcode = SQLFreeHandle( SQL_HANDLE_DESC, hdesc1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- FreeHandle(DESC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_DESC, hdesc1 );
    }; /* if */

    PRINTF( "InvalidFree\t -- Wrong handletype"CRLF );
    retcode = SQLFreeHandle( SQL_HANDLE_ENV, hstmt1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- FreeHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt1 );
    }; /* if */

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt1 );
    SPRINTF( textBuffer,
             "InvalidFree\t -- FreeHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt1 );
    }; /* if */

    FullDisconnect( &henv, &hdbc );
}; /* InvalidFree */
