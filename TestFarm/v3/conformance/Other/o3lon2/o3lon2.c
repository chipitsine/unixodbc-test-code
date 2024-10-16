/*!**********************************************************************

  module: o3lon2.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Read long data with SQLGetData. PTS 1102534
               - two long columns
               - first read read only the first 1024 bytes of both longs
               - fetch again
               - then read the rest of the first long in pieces of 32k
               - then read the rest of the second long at once

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

#define PROGNAME "O3LON2"

#define ODBCVER 0x0350

#include "o3util.c"

#define CHAR_LEN 100000

char longBuf[CHAR_LEN];

/* Insert long values */
RETCODE InsertLong( HSTMT hstmt, UDWORD longLen1, UDWORD longLen2 );

/* show table contents */
RETCODE ShowTable( HDBC hdbc );

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HDBC    hdbc  = SQL_NULL_HDBC;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    InitLongBuf( longBuf, CHAR_LEN );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = CreateTable( hdbc, PROGNAME,
                           "( b long byte, c long byte )" );
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

    retcode = InsertLong( hstmt, 80000, 80000 );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ShowTable( hdbc );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
}; /* main */


RETCODE InsertLong( HSTMT hstmt, UDWORD longLen1, UDWORD longLen2 )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    int     i = 0;
    char    binValue1[CHAR_LEN];
    char    binValue2[CHAR_LEN];
    int     intValue;
    SQLLEN  binIndi1;
    SQLLEN  binIndi2;

    SPRINTF( stmtStr, "insert into %s values (?,?)", PROGNAME );
    SPRINTF( textBuffer, "InsertLong\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 1,
                                SQL_PARAM_INPUT,
                                SQL_C_BINARY,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &binValue1,
                                CHAR_LEN, &binIndi1 );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindParameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 2,
                                SQL_PARAM_INPUT,
                                SQL_C_BINARY,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &binValue2,
                                CHAR_LEN, &binIndi2 );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindParameter(3) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    memcpy( binValue1, longBuf, longLen1 );
    memcpy( binValue2, longBuf+10000, longLen2 );
    SPRINTF( textBuffer, "InsertLong\t -- Insert Values"CRLF );
    PRINTF( textBuffer );
    binIndi1 = longLen1;
    binIndi2 = longLen2;
    retcode = SQLExecute( hstmt );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLExecute (No %d)\t\t retcode: %s"CRLF,
             i, cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* InsertLong */


RETCODE ShowTable( HDBC hdbc )
{
    RETCODE   retcode = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];
    char      binValue[CHAR_LEN];
    char      charValue[CHAR_LEN];
    UWORD     intValue;
    SQLLEN    binIndi;
    SQLLEN    charIndi;
    SQLLEN    intIndi;
    HSTMT     hstmt;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select b, c from %s", PROGNAME );
    SPRINTF( textBuffer, "ShowTable\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    /* set SQL_ATTR_OPTIMIZE_CURSOR */
    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CURSOR_TYPE,
                              (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0 );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLSetStmtAttr \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    retcode = SQLSetStmtAttr( hstmt, SQL_ATTR_CONCURRENCY,
                              (SQLPOINTER) SQL_CONCUR_READ_ONLY, 0 );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLSetStmtAttr \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    retcode = SQLSetStmtAttr( hstmt, 3002, (SQLPOINTER) 1, SQL_IS_INTEGER );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLSetStmtAttr \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        return retcode;
    }; /* if */

    retcode = SQLExecDirect( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* show content of table */
    PRINTF( "ShowTable\t -- Content of table"CRLF );
    retcode = SQLFetchScroll( hstmt, SQL_FETCH_ABSOLUTE, 1 );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLFetch \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        return retcode;
    }; /* if */

    /* get the first 1024 bytes from the long columns */
    printf( "get the first 1024 bytes from the long columns\n" );
    retcode = SQLGetData( hstmt, 1, SQL_C_BINARY, charValue, 1024, &charIndi );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    printf( "LONGCHAR: " );
    if (charIndi != SQL_NULL_DATA) {
        if (charIndi != SQL_NO_TOTAL  &&  charIndi < 1024) {
            charValue[charIndi] = '\0';
        } else {
            charValue[1024] = '\0';
        }; /* else */
        ShowLongChar( charValue );
    } else {
        printf("NULL\n");
    }; /* else */
    printf( "charIndi = %d\n", charIndi );

    retcode = SQLGetData( hstmt, 2, SQL_C_BINARY, binValue, 1024, &binIndi );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    printf( "LONGBINARY: " );
    if (binIndi != SQL_NULL_DATA) {
        if (binIndi != SQL_NO_TOTAL  &&  binIndi < 1024) {
            binValue[binIndi] = '\0';
        } else {
            binValue[1024] = '\0';
        }; /* else */
        ShowLongChar( binValue );
    } else {
        printf("NULL\n");
    }; /* else */
    printf( "binIndi = %d\n", binIndi );

    /* fetch again */
    retcode = SQLFetchScroll( hstmt, SQL_FETCH_ABSOLUTE, 1 );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLFetch \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        return retcode;
    }; /* if */

    /* get whole data from the long columns */
    do {
        printf( "get more data from the long columns\n" );
        retcode = SQLGetData( hstmt, 1, SQL_C_BINARY, charValue, 32780,
                              &charIndi );
        SPRINTF( textBuffer,
                 "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        printf( "LONGCHAR: " );
        if (charIndi != SQL_NULL_DATA) {
            if (charIndi != SQL_NO_TOTAL  &&  binIndi < 32780) {
                charValue[binIndi] = '\0';
            } else {
                charValue[32780] = '\0';
            }; /* else */
            ShowLongChar( charValue );
        } else {
            printf("NULL\n");
        }; /* else */
        printf( "charIndi = %d\n", charIndi );
    } while (retcode == SQL_SUCCESS_WITH_INFO);

    retcode = SQLGetData( hstmt, 2, SQL_C_BINARY,binValue, CHAR_LEN, &binIndi );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    printf( "LONGBINARY: " );
    if (binIndi != SQL_NULL_DATA) {
        binValue[binIndi] = '\0';
        ShowLongChar( binValue );
    } else {
        printf("NULL\n");
    }; /* else */
    printf( "binIndi = %d\n", binIndi );

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLFreeStmt(CLOSE)\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* ShowTable */
