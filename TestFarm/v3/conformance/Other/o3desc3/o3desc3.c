/*!**********************************************************************

  module: o3desc3.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Get descriptor fields.

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

#define PROGNAME "O3DESC3"

#define ODBCVER 0x0350

#define CHAR_LEN   100000

#include "o3util.c"
#include "o3utdesc.c"

RETCODE Insert( HSTMT hstmt );

RETCODE Fetch( HSTMT hstmt );

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HDBC    hdbc  = SQL_NULL_HDBC;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
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

    ShowStmtDesc( hstmt );

    retcode = CreateTable( hdbc, PROGNAME, "( a long byte, b long varchar, c int, d char(10), e varchar(200), f date, g time, h timestamp )" );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    Insert( hstmt );

    Fetch( hstmt );
exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
}; /* MAIN */


RETCODE Insert( HSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];

    char    longbinValue[CHAR_LEN];
    char    longcharValue[CHAR_LEN];
    SQLSMALLINT     intValue;
    char    charValue[CHAR_LEN];
    char    varcharValue[CHAR_LEN];
    SQL_DATE_STRUCT dateValue;
    SQL_TIME_STRUCT timeValue;
    SQL_TIMESTAMP_STRUCT timestampValue;
    SQLLEN  longbinIndi;
    SQLLEN  longcharIndi;
    SQLLEN  charIndi;
    SQLLEN  varcharIndi;

    SPRINTF( stmtStr, "insert into %s values (?,?,?,?,?,?,?,?)", PROGNAME );
    SPRINTF( textBuffer, "insert\t\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( CRLF"Stmt is prepared:"CRLF );
    ShowStmtDesc( hstmt );

    /* Bind parameters */
    retcode = SQLBindParameter( hstmt, 1,
                                SQL_PARAM_INPUT,
                                SQL_C_BINARY,
                                SQL_LONGVARBINARY,
                                CHAR_LEN, 0,
                                &longbinValue,
                                CHAR_LEN, &longbinIndi );
    longbinIndi = CHAR_LEN;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 2,
                                SQL_PARAM_INPUT,
                                SQL_C_CHAR,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &longcharValue,
                                CHAR_LEN, &longcharIndi );
    longcharIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 3,
                                SQL_PARAM_INPUT,
                                SQL_C_SHORT,
                                SQL_INTEGER,
                                0, 0,
                                &intValue,
                                0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(3) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 4,
                                SQL_PARAM_INPUT,
                                SQL_C_CHAR,
                                SQL_CHAR,
                                CHAR_LEN, 0,
                                &charValue,
                                CHAR_LEN, &charIndi );
    charIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(4) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 5,
                                SQL_PARAM_INPUT,
                                SQL_C_CHAR,
                                SQL_VARCHAR,
                                CHAR_LEN, 0,
                                &varcharValue,
                                CHAR_LEN, &varcharIndi );
    varcharIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(5) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 6,
                                SQL_PARAM_INPUT,
                                SQL_C_DATE,
                                SQL_DATE,
                                0, 0,
                                &dateValue,
                                0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(6) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 7,
                                SQL_PARAM_INPUT,
                                SQL_C_TIME,
                                SQL_TIME,
                                0, 0,
                                &timeValue,
                                0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(7) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 8,
                                SQL_PARAM_INPUT,
                                SQL_C_TIMESTAMP,
                                SQL_TIMESTAMP,
                                0, 0,
                                &timestampValue,
                                0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(8) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( "Parameters are bound:"CRLF );
    ShowStmtDesc( hstmt );

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* Insert */


RETCODE Fetch( HSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];

    char    longbinValue[CHAR_LEN];
    char    longcharValue[CHAR_LEN];
    int     intValue;
    char    charValue[CHAR_LEN];
    char    varcharValue[CHAR_LEN];
    SQL_DATE_STRUCT dateValue;
    SQL_TIME_STRUCT timeValue;
    SQL_TIMESTAMP_STRUCT timestampValue;
    SQLLEN  longbinIndi;
    SQLLEN  longcharIndi;
    SQLLEN  charIndi;
    SQLLEN  varcharIndi;

    SPRINTF( stmtStr, "select * from %s", PROGNAME );
    SPRINTF( textBuffer, "Fetch\t\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "Fetch\t\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( CRLF"Stmt is prepared:"CRLF );
    ShowStmtDesc( hstmt );

    /* Bind parameters */
    retcode = SQLBindCol( hstmt, 1,
                          SQL_C_BINARY,
                          &longbinValue,
                          CHAR_LEN, &longbinIndi );
    longbinIndi = CHAR_LEN;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 2,
                          SQL_C_CHAR,
                          &longcharValue,
                          CHAR_LEN, &longcharIndi );
    longcharIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 3,
                          SQL_C_SHORT,
                          &intValue,
                          0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(3) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 4,
                          SQL_C_CHAR,
                          &charValue,
                          CHAR_LEN, &charIndi );
    charIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(4) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 5,
                          SQL_C_CHAR,
                          &varcharValue,
                          CHAR_LEN, &varcharIndi );
    varcharIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(5) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 6,
                          SQL_C_DATE,
                          &dateValue,
                          0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(6) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 7,
                          SQL_C_TIME,
                          &timeValue,
                          0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(7) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 8,
                          SQL_C_TIMESTAMP,
                          &timestampValue,
                          0, NULL );
    SPRINTF( textBuffer,
             "Insert\t\t -- SQLBindParameter(8) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( "Columns are bound:"CRLF );
    ShowStmtDesc( hstmt );

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "Fetch\t\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* Fetch */


