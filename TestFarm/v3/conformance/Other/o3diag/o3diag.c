/*!**********************************************************************

  module: o3diag.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Tests for SQLGetDiagRec, SQLGetDiagField

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

#define PROGNAME "o3diag"

#define ODBCVER 0x0350

#include "o3util.c"

void GetDiagInfoStmt();

int main(int argc, argv_t argv) {

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    GetDiagInfoStmt();

    EndTest( PROGNAME );

    return 0;
}; /* main */

void ShowDiagFields( SWORD handleType, SQLHANDLE handle, SWORD recNumber );

void GetDiagInfoStmt()
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    HENV henv    = SQL_NULL_HENV;
    HDBC hdbc    = SQL_NULL_HDBC;
    HSTMT hstmt1 = SQL_NULL_HSTMT;
    HSTMT hstmt2 = SQL_NULL_HSTMT;
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    RETCODE retcode;

    PRINTF( CRLF"GetDiagInfoStmt"CRLF );
    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */

    /* Allocate Stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt1 );
    SPRINTF( textBuffer,
             "GetDiagInfoStmt\t -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt1 );

    PRINTF( "GetDiagInfoStmt\t -- Error from datasource"CRLF );
    SPRINTF( stmtStr, "SYNTACTICAL INCORRECT STMT" );
    SPRINTF( textBuffer, "GetDiagInfoStmt\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );
    retcode = SQLExecDirect( hstmt1, stmtStr, STRLEN(stmtStr) );
    SPRINTF( textBuffer,
             "GetDiagInfoStmt\t -- ExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt1 );
    ShowDiagFields( SQL_HANDLE_STMT, hstmt1, 1 );

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt2 );
    SPRINTF( textBuffer,
             "GetDiagInfoStmt\t -- AllocHandle(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt2 );

    PRINTF( "GetDiagInfoStmt\t -- Error from driver"CRLF );
    retcode = SQLSetStmtAttr( hstmt2, /* invalid */ 29999, NULL, 0 );
    SPRINTF( textBuffer,
             "GetDiagInfoStmt\t -- SetStmtAttr(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt2 );
    ShowDiagFields( SQL_HANDLE_STMT, hstmt2, 1 );

    /* checking messages from DM not sensible under Linux/UNIX
    PRINTF( "GetDiagInfoStmt\t -- Message from DM"CRLF );
    retcode = SQLSetStmtAttr( hstmt2,
                              SQL_ATTR_ASYNC_ENABLE, (SQLPOINTER) 123, 0 );
    SPRINTF( textBuffer,
             "GetDiagInfoStmt\t -- SetStmtAttr(STMT) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt2 );
    ShowDiagFields( SQL_HANDLE_STMT, hstmt2, 1 );
    */
    FullDisconnect( &henv, &hdbc );
}; /* GetDiagInfoStmt */

typedef struct {
    SWORD  infoId;
    UCHAR  infoName[50];
    SWORD  dataType;
    SWORD  recNo;
} DiagInfoTab;

static DiagInfoTab diagInfoValues[] =
{
    { SQL_DIAG_CURSOR_ROW_COUNT, "CURSOR_ROW_COUNT\t", SQL_INTEGER, 0 },
    { SQL_DIAG_DYNAMIC_FUNCTION, "DYNAMIC_FUNCTION\t", SQL_CHAR, 0 },
    { SQL_DIAG_DYNAMIC_FUNCTION_CODE,"DYNAMIC_FUNCTION_CODE\t",SQL_INTEGER, 0},
    { SQL_DIAG_NUMBER, "NUMBER\t\t\t", SQL_INTEGER, 0},
    /* following is implemented in the driver manager */
    /* { SQL_DIAG_RETURNCODE, "RETURNCODE\t\t", SQL_INTEGER, 0 }, */
    { SQL_DIAG_ROW_COUNT, "ROW_COUNT\t\t", SQL_INTEGER, 0 },
    { SQL_DIAG_CLASS_ORIGIN, "CLASS_ORIGIN\t\t", SQL_CHAR, 1 },
    { SQL_DIAG_COLUMN_NUMBER, "COLUMN_NUMBER\t\t", SQL_INTEGER, 1 },
    { SQL_DIAG_CONNECTION_NAME, "CONNECTION_NAME\t\t", SQL_CHAR, 1 },
    { SQL_DIAG_MESSAGE_TEXT, "MESSAGE_TEXT\t\t", SQL_CHAR, 1 },
    { SQL_DIAG_NATIVE, "NATIVE\t\t\t", SQL_INTEGER, 1 },
    { SQL_DIAG_ROW_NUMBER, "ROW_NUMBER\t\t", SQL_INTEGER, 1 },
    { SQL_DIAG_SERVER_NAME, "SERVER_NAME\t\t", SQL_CHAR, 1 },
    { SQL_DIAG_SQLSTATE, "SQLSTATE\t\t", SQL_CHAR, 1 },
    { SQL_DIAG_SUBCLASS_ORIGIN, "SUB_CLASS_ORIGIN\t", SQL_CHAR, 1 }
}; /* diagInfoValues */

static UWORD numEntries = sizeof(diagInfoValues) / sizeof(DiagInfoTab);

void ShowDiagFields( SWORD handleType, SQLHANDLE handle, SWORD recNumber )
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    RETCODE retcode;
    SDWORD intValue = 0;
    SWORD  length   = 0;
    UCHAR  charValue[300];
    SWORD  i;

    for ( i=0; i<numEntries; i++ ) {
        if (diagInfoValues[i].dataType == SQL_CHAR) {
            retcode = SQLGetDiagField( handleType, handle,
                                       diagInfoValues[i].recNo,
                                       diagInfoValues[i].infoId,
                                       &charValue,
                                       sizeof(charValue),
                                       &length );
        } else {
            retcode = SQLGetDiagField( handleType, handle,
                                       diagInfoValues[i].recNo,
                                       diagInfoValues[i].infoId,
                                       &intValue,
                                       sizeof(intValue),
                                       &length );
        }; /* else */
        if (retcode != SQL_SUCCESS) {
            SPRINTF( textBuffer,
                     "ShowDiagFields\t -- GetDiagField(%s)\t retcode: %s"CRLF,
                     diagInfoValues[i].infoName,
                     cnvret(retcode));
            PRINTF( textBuffer );
        } else {
            PRINTF( diagInfoValues[i].infoName );
            PRINTF( " = " );
            if (diagInfoValues[i].dataType == SQL_C_CHAR) {
                SPRINTF( textBuffer, "%s\t (length %d)"CRLF,
                         charValue,
                         length );
                PRINTF( textBuffer );
            } else {
                SPRINTF( textBuffer, "%d"CRLF, intValue );
                PRINTF( textBuffer );
            }; /* else */
        }; /* else */
    }; /* for */
}; /* ShowDiagFields */
