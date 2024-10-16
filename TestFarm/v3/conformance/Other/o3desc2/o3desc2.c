/*!**********************************************************************

  module: o3desc2.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Use a decriptor to copy the contents of one table
                into another like o3desc1. Here the table contains
                long values.

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

#define PROGNAME "O3DESC2"

#define ODBCVER 0x0350

#include "o3util.c"

#define NUM_VALUES 50
#define CHAR_LEN   100000

/* Insert Values into table 1 */
RETCODE InsertInto1( HSTMT hstmt );

/* Copy Values from table 1 into table 2 */
RETCODE CopyFrom1To2( HDBC hdbc, HSTMT hstmt );

/* Display values from table 1 */
RETCODE ReadFrom1( HSTMT hstmt );

/* Display values from table 2 */
RETCODE ReadFrom2( HSTMT hstmt );


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

    retcode = CreateTable( hdbc, PROGNAME"1", "( a long byte, b long varchar, c int )" );
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

    retcode = InsertInto1( hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ReadFrom1( hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = CreateTable( hdbc, PROGNAME"2", "( a long byte, b long varchar, c int )" );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = CopyFrom1To2( hdbc, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ReadFrom2( hstmt );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLFreeHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );

exit:
    retcode = DropTable( hdbc, PROGNAME"1" );
    retcode = DropTable( hdbc, PROGNAME"2" );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
}; /* main */


RETCODE InsertInto1( HSTMT hstmt )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    int     i;
    char    charValue[CHAR_LEN];
    char    binValue[CHAR_LEN];
    SQLSMALLINT     intValue;
    SQLLEN  charIndi;
    SQLLEN  binIndi;

    SPRINTF( stmtStr, "insert into %s values (?,?,?)", PROGNAME"1" );
    SPRINTF( textBuffer, "InsertInto1\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    memset (charValue, 0, CHAR_LEN);
    memset (binValue, 0, CHAR_LEN);

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "InsertInto1\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 1,
                                SQL_PARAM_INPUT,
                                SQL_C_BINARY,
                                SQL_LONGVARBINARY,
                                CHAR_LEN, 0,
                                binValue,
                                CHAR_LEN, &binIndi );
    binIndi = CHAR_LEN;
    SPRINTF( textBuffer,
             "InsertInto1\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
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
                                charValue,
                                CHAR_LEN, &charIndi );
    charIndi = SQL_NTS;
    SPRINTF( textBuffer,
             "InsertInto1\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
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
             "InsertInto1\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    STRCPY( charValue, "" );
    STRCPY( binValue, "" );
    SPRINTF( textBuffer, "InsertInto1\t -- Insert %d Values"CRLF, NUM_VALUES );
    PRINTF( textBuffer );
    for (i=0; (i < NUM_VALUES && i < CHAR_LEN); i++) {
        STRCAT( binValue, "1234567890abcdefghijklmnopqrstuvwxyz" );
        STRCAT( binValue, "1234567890abcdefghijklmnopqrstuvwxyz" );
        STRCAT( charValue, binValue );
        intValue = i;
        retcode = SQLExecute( hstmt );
        SPRINTF( textBuffer,
                 "InsertInto1\t -- SQLExecute (No %d)\t\t retcode: %s"CRLF,
                 i, cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
    }; /* for */
    /* Insert NULL Values */
    binIndi = SQL_NULL_DATA;
    retcode = SQLExecute( hstmt );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    charIndi = SQL_NULL_DATA;
    retcode = SQLExecute( hstmt );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* InsertInto1 */


RETCODE CopyFrom1To2( HDBC hdbc, HSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    HSTMT     insertHstmt = SQL_NULL_HSTMT;
    SQLHDESC  hdesc       = SQL_NULL_HDESC;
    SQLHDESC  newHdesc    = SQL_NULL_HDESC;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];
    int       i;
    char      charValue[CHAR_LEN];
    char      binValue[CHAR_LEN];
    SQLSMALLINT     intValue;
    SQLLEN    charIndi;
    SQLLEN    binIndi;
    SQLLEN    intIndi;

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select a, b, c from %s order by c", PROGNAME"1" );
    SPRINTF( textBuffer, "CopyFrom1To2\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLExecDirect( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 1,
                          SQL_C_BINARY, binValue, CHAR_LEN, &binIndi );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLBindCol(1) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 2,
                          SQL_C_CHAR, charValue, CHAR_LEN, &charIndi );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLBindCol(1) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindCol( hstmt, 3,
                          SQL_C_SHORT, &intValue, 0, &intIndi );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLBindCol(2) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* Get application Row Descriptor from select stmt */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_APP_ROW_DESC, &hdesc, 0, NULL );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- GetStmtAttr(ARD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    ShowDesc( hdesc );

    /* alloc insert stmt */
    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &insertHstmt );
    SPRINTF( textBuffer,
             "CopyFrom1To2 \t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, insertHstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* Use ARD from select as APD for insert */
    retcode = SQLAllocHandle( SQL_HANDLE_DESC, hdbc, &newHdesc );
    SPRINTF( textBuffer,
             "CopyFrom1To2 \t -- SQLAllocHandle(DESC) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DBC, hdbc );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLCopyDesc( hdesc, newHdesc );
    SPRINTF( textBuffer,
             "CopyFrom1To2 \t -- CopyDesc \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    ShowDesc( newHdesc );

    retcode = SQLSetStmtAttr( insertHstmt, SQL_ATTR_APP_PARAM_DESC,
                              newHdesc, sizeof(newHdesc) );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SetStmtAttr(APD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, insertHstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* Prepare insert stmt */
    SPRINTF( stmtStr, "insert into %s values (?,?,?)", PROGNAME"2" );
    SPRINTF( textBuffer, "CopyFrom1To2\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( insertHstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, insertHstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* Copy rows from table 1 into table 2 */
    PRINTF( "CopyFrom1To2\t -- Copy tab1 to tab2"CRLF );
    do {
        retcode = SQLFetch( hstmt );
        SPRINTF( textBuffer,
                 "CopyFrom1To2\t -- SQLFetch \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "Fetch Row: %d %d %d"CRLF,
                     intValue, binIndi, charIndi );
            PRINTF( textBuffer );
            retcode = SQLExecute( insertHstmt );
            SPRINTF( textBuffer,
                     "CopyFrom1To2\t -- SQLExecute \t\t\t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, insertHstmt );
        } else {
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
        }; /* else */
    } while (retcode == SQL_SUCCESS);
    retcode = SQLFreeHandle( SQL_HANDLE_STMT, insertHstmt );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLFreeHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "CopyFrom1To2\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* CopyFrom1To2 */


RETCODE ReadFromTable( HSTMT hstmt, char *tableName );


RETCODE ReadFrom1( HSTMT hstmt )
{
    char tableName[100];
    SPRINTF( tableName, PROGNAME"1" );
    return ReadFromTable( hstmt, tableName );
}; /* ReadFrom1 */


RETCODE ReadFrom2( HSTMT hstmt )
{
    char tableName[100];
    SPRINTF( tableName, PROGNAME"2" );
    return ReadFromTable( hstmt, tableName );
}; /* ReadFrom2 */


RETCODE ReadFromTable( HSTMT hstmt, char *tableName )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];
    char      charValue[CHAR_LEN];
    char      binValue[CHAR_LEN];
    SQLSMALLINT     intValue;
    SQLLEN    charIndi;
    SQLLEN    binIndi;
    SQLLEN    intIndi;

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select a, b, c from %s order by c", tableName );
    SPRINTF( textBuffer, "ReadFromTable\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLExecDirect( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "ReadFromTable\t -- SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 1,
                          SQL_C_BINARY, binValue, CHAR_LEN, &binIndi );
    SPRINTF( textBuffer,
             "ReadFromTable\t -- SQLBindCol(1) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 2,
                          SQL_C_CHAR, charValue, CHAR_LEN, &charIndi );
    SPRINTF( textBuffer,
             "ReadFromTable\t -- SQLBindCol(1) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 3,
                          SQL_C_SHORT, &intValue, 0, NULL );
    SPRINTF( textBuffer,
             "ReadFromTable\t -- SQLBindCol(2) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* show content of table */
    PRINTF( "ReadFromTable\t -- Content of tab2"CRLF );
    do {
        retcode = SQLFetch( hstmt );
        SPRINTF( textBuffer,
                 "ReadFromTable\t -- SQLFetch \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ShowLongChar( binValue );
        ShowLongChar( charValue );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "Fetch Row: %d %d strlen=%d %d"CRLF,
                     intValue, binIndi, STRLEN(binValue), charIndi );
            PRINTF( textBuffer );
        } else {
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
        }; /* else */
    } while (retcode == SQL_SUCCESS);

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "ReadFromTable\t -- SQLFreeStmt(CLOSE)\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* ReadFromTable */
