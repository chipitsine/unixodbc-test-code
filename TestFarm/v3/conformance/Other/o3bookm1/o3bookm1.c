/*!**********************************************************************

  module: o3bookm1.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Retrieve bookmarks with SQLGetData. Use SQL_INTEGER
                bookmarks (the old fashioned 2.5 way).

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

#define PROGNAME "o3bookm1"

#define ODBCVER 0x0350

#include "o3util.c"

#define CHAR_LEN 4000
#define NUM_VALUES 20

#define SQL_STORE_BOOKMARK 20001

TestTableStruct TestValues[] = {
    { SQL_SELECT_NEW, 1, "", SQL_CURSOR_STATIC, SQL_CONCUR_READ_ONLY },
    { SQL_FETCH_LAST,      0, "" },
    { SQL_STORE_BOOKMARK,  0, "" },
    { SQL_FETCH_FIRST,     0, "" },
    { SQL_FETCH_BOOKMARK,  0, "" },
    { SQL_FETCH_NEXT,      0, "" },
    { SQL_STORE_BOOKMARK,  0, "" },
    { SQL_FETCH_RELATIVE, -10, "" },
    { SQL_STORE_BOOKMARK,  0, "" },
    { SQL_FETCH_NEXT,      0, "" },
    { SQL_FETCH_NEXT,      0, "" },
    { SQL_FETCH_NEXT,      0, "" },
    { SQL_FETCH_NEXT,      0, "" },
    { SQL_FETCH_BOOKMARK,  0, "" },
    { SQL_FETCH_NEXT,      0, "" }
}; /* TestValues */

RETCODE InsertValues( HSTMT hstmt );
RETCODE TestBookmark( HSTMT *phstmt, SWORD optimizeCursor );

/* globals */
HDBC    hdbc  = SQL_NULL_HDBC;

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    char   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = CreateTable( hdbc, PROGNAME, "( a long ascii, b int )" );
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

    retcode = InsertValues( hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = TestBookmark( &hstmt, /* OPTIMIZE_CURSOR == */ SQL_FALSE );

exit:
    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */


RETCODE InsertValues( HSTMT hstmt )
{
    RETCODE retcode = SQL_SUCCESS;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    int     i;
    char    charValue[CHAR_LEN];
    SQLSMALLINT     intValue;
    SQLLEN  charIndi;

    SPRINTF( (char*) stmtStr, "insert into %s values (?,?)", PROGNAME );
    SPRINTF( textBuffer, "InsertValues\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "InsertValues\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 1,
                                SQL_PARAM_INPUT,
                                SQL_C_CHAR,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &charValue,
                                CHAR_LEN, &charIndi );
    SPRINTF( textBuffer,
             "InsertValues\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    retcode = SQLBindParameter( hstmt, 2,
                                SQL_PARAM_INPUT,
                                SQL_C_SHORT,
                                SQL_INTEGER,
                                0, 0,
                                &intValue,
                                0, NULL );
    SPRINTF( textBuffer,
             "InsertValues\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    SPRINTF( textBuffer, "InsertValues\t -- Insert %d Values"CRLF, NUM_VALUES );
    PRINTF( textBuffer );
    charValue[0] = '\0';
    STRCAT( charValue, "1234567890abcdefghijklmnopqrstuvwxyz" );
    charIndi = strlen( charValue );
    for (i=1; (i <= NUM_VALUES && i < CHAR_LEN); i++) {
        intValue = i;
        retcode = SQLExecute( hstmt );
        SPRINTF( textBuffer,
                 "InsertValues\t -- SQLExecute (No %d)\t\t retcode: %s"CRLF,
                 i, cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
    }; /* for */
    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* Insert */

RETCODE PrepareStmt( HSTMT hstmt );

RETCODE DoSelect( HSTMT  *phstmt,
                  UDWORD  rowset,
                  DWORD   fSqlCursorType,
                  DWORD   fSqlConcur,
                  SWORD   optimizeCursor );

/* globals */
char      charValue[CHAR_LEN];
UWORD     intValue;
SQLLEN    binIndi;
SQLLEN    intIndi;

RETCODE TestBookmark( HSTMT *phstmt, SWORD optimizeCursor )
{
    UDWORD    i;
    UDWORD    rowNumber;

    TestTableStruct p;
    RETCODE         retcode;
    char           textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UDWORD          rowset;
    SQLLEN          bookmark = 0;           /* 64 Bit! */
    SQLROWSETSIZE          crow = 0;

    for (i=0; i < (sizeof(TestValues) / sizeof( TestTableStruct)); i++ ) {
        p = TestValues[i];
        switch(p.fFetchType) {
        case SQL_SELECT_NEW: {
            rowset = p.irow;
            retcode = DoSelect( phstmt, rowset,
                                p.fSqlCursorType, p.fSqlConcur, optimizeCursor );
            break;
        }
        case SQL_STORE_BOOKMARK:
            retcode = SQLGetData( *phstmt, 0,
                                  SQL_C_BOOKMARK,
                                  &bookmark, sizeof(bookmark), NULL );
            SPRINTF( textBuffer,
                     "TestBookmark\t -- SQLGetData \t retcode: '%s'"CRLF,
                     cnvret(retcode));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, *phstmt );
            break;
        case SQL_FETCH_BOOKMARK:
            p.irow = bookmark;
            /* falltrough */
        default: {
            STRCPY( charValue, "xxx" );
            retcode = SQLExtendedFetch( *phstmt,
                                        p.fFetchType,
                                        p.irow,
                                        &crow, NULL );
            SPRINTF( textBuffer,
                     "TestBookmark\t -- SQLExtendedFetch '%s' (%ld) retcode: '%s'"CRLF,
                     cnvFetchType(p.fFetchType), p.irow, cnvret(retcode));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, *phstmt );
            if (retcode != SQL_NO_DATA_FOUND) {
                SPRINTF( textBuffer,
                         "%s \t %d \t \n", charValue, intValue );
                PRINTF( textBuffer );
                if (binIndi == SQL_NULL_DATA) {
                    PRINTF( "charVale is NULL"CRLF );
                }; /* if */
            }; /* if */
            break;
        }
        }; /* switch */
    }; /* for */

    return retcode;
} /* TestBookmark */


RETCODE PrepareStmt( HSTMT hstmt )
{
    RETCODE   retcode = SQL_SUCCESS;
    char      textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];

    /* Prepare select stmt */
    SPRINTF( (char*) stmtStr, "select a, b from %s order by b", PROGNAME );
    SPRINTF( textBuffer, "PrepareStmt\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "PrepareStmt\t -- SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    return retcode;
} /* PrepareStmt */


RETCODE DoSelect( HSTMT  *phstmt,
                  UDWORD  rowset,
                  DWORD   fSqlCursorType,
                  DWORD   fSqlConcur,
                  SWORD   optimizeCursor )
{
    char      textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    RETCODE    retcode;

    SPRINTF( textBuffer, " New Test with %d rows and '%s'"CRLF,
             rowset,
             cnvcurs(fSqlCursorType));
    PRINTF( textBuffer);

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, *phstmt );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLFreeHandle\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, phstmt );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( *phstmt, SQL_ATTR_CURSOR_TYPE,
                              (SQLPOINTER) (SQLLEN) fSqlCursorType, 0 );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLSetStmtAttr(CURSOR_TYPE)\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( *phstmt, SQL_ATTR_CONCURRENCY,
                              (SQLPOINTER) (SQLLEN) fSqlConcur, 0 );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLSetStmtAttr(CONCUR)\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( *phstmt, SQL_ATTR_ROW_ARRAY_SIZE,
                              (SQLPOINTER) (SQLLEN) rowset, 0 );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLSetStmtAttr(ROWSET_SIZE)\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* SQL_ATTR_OPTIMIZE_CURSOR */
    retcode = SQLSetStmtAttr( *phstmt, 3002, (SQLPOINTER) (SQLLEN) optimizeCursor, 0 );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLSetStmtAttr(OPTIMIZE)\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( *phstmt, SQL_ATTR_USE_BOOKMARKS,
                              (SQLPOINTER) SQL_UB_ON, 0 );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLSetStmtAttr(USE_BOOKMARKS)\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = PrepareStmt( *phstmt );

    retcode = SQLBindCol( *phstmt, 1,
                          SQL_C_CHAR, charValue, CHAR_LEN, &binIndi );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLBindCol(1) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( *phstmt, 2,
                          SQL_C_SHORT, &intValue, 0, &intIndi );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLBindCol(2) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLExecute( *phstmt );
    ErrorCheck( SQL_HANDLE_STMT, *phstmt );

    return retcode;
} /* DoSelect */
