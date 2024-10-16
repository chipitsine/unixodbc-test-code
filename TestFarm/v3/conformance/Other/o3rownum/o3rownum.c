/*!**********************************************************************

  module: o3rownum.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: testing stmt attribut SQL_ROW_NUMBER with and without
               the stmt attribut SQL_ATTR_OPTIMIZE_CURSOR
               PTS 1102762

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

#define PROGNAME "O3ROWNUM"

#define ODBCVER 0x0350

#include "o3util.c"

#define CHAR_LEN 4000
#define NUM_VALUES 10

TestTableStruct TestValues[] = {
{ SQL_SELECT_NEW, 1, "", SQL_CURSOR_KEYSET_DRIVEN, SQL_CONCUR_READ_ONLY },
{ SQL_FETCH_LAST, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_RELATIVE, 2, "" },
{ SQL_SELECT_NEW, 1, "", SQL_CURSOR_KEYSET_DRIVEN, SQL_CONCUR_READ_ONLY },
{ SQL_FETCH_LAST, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_RELATIVE, -2, "" },
{ SQL_SELECT_NEW, 1, "", SQL_CURSOR_KEYSET_DRIVEN, SQL_CONCUR_READ_ONLY },
{ SQL_FETCH_ABSOLUTE, 2, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_PREV, 0, "" },
{ SQL_FETCH_RELATIVE, 2, "" },
{ SQL_SELECT_NEW, 1, "", SQL_CURSOR_KEYSET_DRIVEN, SQL_CONCUR_READ_ONLY },
{ SQL_FETCH_ABSOLUTE, 8, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_RELATIVE, -2, "" },
{ SQL_SELECT_NEW, 1, "", SQL_CURSOR_KEYSET_DRIVEN, SQL_CONCUR_READ_ONLY },
{ SQL_FETCH_RELATIVE, 0, "" },
{ SQL_FETCH_FIRST, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_RELATIVE, -10, "" },
{ SQL_FETCH_LAST, 0, "" },
{ SQL_FETCH_NEXT, 0, "" },
{ SQL_FETCH_RELATIVE, -1, "" },
{ SQL_FETCH_RELATIVE, -4, "" },
};

RETCODE InsertValues( HSTMT hstmt );

RETCODE TestFetchScroll( HSTMT *phstmt, SWORD optimizeCursor );

HDBC    hdbc  = SQL_NULL_HDBC;

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = CreateTable( hdbc, PROGNAME, "( a varchar(1000), b int )" );
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

    retcode = TestFetchScroll( &hstmt, /* OPTIMIZE_CURSOR == */ SQL_FALSE );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    };

    retcode = TestFetchScroll( &hstmt, /* OPTIMIZE_CURSOR == */ SQL_TRUE );
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

exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */


RETCODE InsertValues( HSTMT hstmt )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    int     i;
    char    charValue[CHAR_LEN];
    SQLSMALLINT     intValue;
    SQLLEN  charIndi;

    SPRINTF( stmtStr, "insert into %s values (?,?)", PROGNAME );
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
                                SQL_VARCHAR,
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
                 "InsertLong\t -- SQLExecute (No %d)\t\t retcode: %s"CRLF,
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

RETCODE DoSelect( HSTMT   *phstmt,
                  UDWORD  rowset,
                  DWORD   fSqlCursorType,
                  DWORD   fSqlConcur,
                  SWORD   optimizeCursor );

/* globals */
char      charValue[CHAR_LEN];
UWORD     intValue;
SQLLEN    binIndi;
SQLLEN    intIndi;

RETCODE TestFetchScroll( HSTMT *phstmt, SWORD optimizeCursor )
{
    UDWORD    crow;
    UDWORD    i;
    UDWORD    rowNumber;

    TestTableStruct p;
    RETCODE         retcode;
    UCHAR           textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UDWORD          rowset;

    for (i=0; i < (sizeof(TestValues) / sizeof( TestTableStruct)); i++ ) {
        p = TestValues[i];
        switch(p.fFetchType) {
        case SQL_SELECT_NEW: {
            rowset = p.irow;
            retcode = DoSelect( phstmt, rowset,
                                p.fSqlCursorType, p.fSqlConcur, optimizeCursor );
            break;
        }
        /*
        case SQL_UPDATE_ROW: {}
        case SQL_INSERT_ROW: {}
        case SQL_DELETE_ROW: {
            DoSetPos(p, &row);
            PrintArrSet(rowset);
            break;
        }
        */
        default: {
            STRCPY( charValue, "xxx" );
            retcode = SQLFetchScroll( *phstmt,
                                      p.fFetchType,
                                      p.irow );
            SPRINTF( textBuffer,
                     "TestFetchScroll\t -- SQLFetchScroll '%s' (%ld) retcode: '%s'"CRLF,
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
                rowNumber = 1000000;
                retcode = SQLGetStmtAttr( *phstmt, SQL_ROW_NUMBER,
                                          &rowNumber, 0, NULL );
                SPRINTF( textBuffer,
                         "TestFetchScroll\t -- SQLGetStmtAttr(ROW_NUM) retcode: '%s'"CRLF,
                         cnvret(retcode));
                PRINTF( textBuffer );
                ErrorCheck( SQL_HANDLE_STMT, *phstmt );
                SPRINTF( textBuffer,
                         "rownumber: %4d"CRLF, rowNumber );
                PRINTF( textBuffer );
            }; /* if */
            break;
        }
        }; /* switch */
    }; /* for */

    return retcode;
} /* TestFetchScroll */


RETCODE PrepareStmt( HSTMT hstmt )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select a, b from %s order by b", PROGNAME );
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
    UCHAR      textBuffer[O3UTIL_TEXT_BUFFER_LEN];
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
    /* ErrorCheck( SQL_HANDLE_STMT, *phstmt ); must not use phstmt!! */
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
    retcode = SQLSetStmtAttr( *phstmt, 3002, (SQLPOINTER)optimizeCursor, 0 );
    SPRINTF( textBuffer,
             "DoSelect\t -- SQLSetStmtAttr(OPTIMIZE)\t retcode: %s"CRLF,
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
