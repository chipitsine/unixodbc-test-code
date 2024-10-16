/*!**********************************************************************

  module: o3ignore2.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Use SQLSet with option SQL_ADD. Set *octetLengthPtr of
                some columns to SQL_IGNORE (PTS 1104910).

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

#define PROGNAME "O3IGNORE2"

#define ODBCVER 0x0350

#include "o3util.c"

#define CHAR_LEN 200
#define NUM_VALUES 7

typedef struct
{
    UWORD   operation; /* SQL_ADD, SQL_UPDATE */
    UDWORD  intValue;
    SDWORD  intLen;
    char   *charValue1;
    SDWORD  charLen1;
    char   *charValue2;
    SDWORD  charLen2;
} TestCase;


TestCase  updateTests[] =
{
    { SQL_ADD, 21, sizeof(UDWORD), "add1", SQL_NTS,       "",        SQL_IGNORE },
    { SQL_ADD, 22, sizeof(UDWORD), "",        SQL_IGNORE, "add2", SQL_NTS },
    { SQL_ADD, 23, sizeof(UDWORD), "",        SQL_IGNORE, "add3", SQL_NTS },
    { SQL_ADD, 24, sizeof(UDWORD), "add4", SQL_NTS,       "add5", SQL_NTS },
    { SQL_ADD, 25, SQL_IGNORE,     "",        SQL_IGNORE, "",        SQL_IGNORE },
    { SQL_ADD, 26, sizeof(UDWORD), "add6", SQL_NTS,       "add7", SQL_NTS },
    { SQL_ADD, 24, SQL_IGNORE,     "add8", SQL_NTS,       "add9", SQL_NTS },
    { 0, 0, 0, NULL, 0, NULL, 0 }
}; /* setPosTests */


RETCODE InsertValues();
SQLRETURN RunTest( TestCase *tests );
RETCODE ReadValues();

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

    retcode = CreateTable( hdbc, PROGNAME, "( a int, b char(200), c varchar(200))" );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = InsertValues();
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ReadValues();
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = RunTest( updateTests );

    retcode = ReadValues();
exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */


SQLRETURN InsertValues()
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    char    charValue1[CHAR_LEN+1];
    char    charValue2[CHAR_LEN+1];
    UDWORD  intValue  = 1;
    SQLLEN  intLen  = sizeof(UDWORD);
    SQLLEN  charLen = SQL_NTS;
    SDWORD  iVal = 0;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "InsertValues\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF("insert into "PROGNAME" values (?,?,?)\n" );
    retcode = SQLPrepare( hstmt,
                          "insert into "PROGNAME" values (?,?,?)",
                          SQL_NTS );
    SPRINTF( textBuffer,
             "InsertValues\t -- Prepare   \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 1,
                                SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,
                                10, 0, &intValue, sizeof(SDWORD), &intLen );
    SPRINTF( textBuffer,
             "InsertValues\t -- BindParameter\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 2,
                                SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
                                CHAR_LEN, 0, &charValue1, CHAR_LEN, &charLen );
    SPRINTF( textBuffer,
             "InsertValues\t -- BindParameter\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindParameter( hstmt, 3,
                                SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
                                CHAR_LEN, 0, &charValue2, CHAR_LEN, &charLen );
    SPRINTF( textBuffer,
             "InsertValues\t -- BindParameter\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    charValue1[0] = '\0';
    charValue2[0] = '\0';
    for (iVal = 1; iVal <= NUM_VALUES; iVal++) {
        intValue = iVal;

        retcode = SQLExecute( hstmt );
        SPRINTF( textBuffer,
                 "InsertValues\t -- Execute   \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
        STRCAT( charValue1, "1" );
        STRCAT( charValue2, "2" );
    }; /* for */

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* InsertValues */


SQLRETURN RunTest( TestCase *tests )
{
    RETCODE   retcode;
    HSTMT     hstmt = SQL_NULL_HSTMT;
    char      textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UWORD     rowStatus;
    SQLROWSETSIZE    rowsFetched = 0;
    char      charValue1[CHAR_LEN+1];
    char      charValue2[CHAR_LEN+1];
    UDWORD    intValue  = 1;
    SQLLEN    intLen  = sizeof(UDWORD);
    SQLLEN    charLen1 = 0;
    SQLLEN    charLen2 = 0;
    TestCase *actTest = NULL;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "StartTest\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CONCURRENCY,
                              (SQLPOINTER) SQL_CONCUR_ROWVER, 0 );
    SPRINTF( textBuffer,
             "StartTest\t -- SetStmtAttr(CONCURRENCY) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CURSOR_TYPE,
                              (SQLPOINTER) SQL_CURSOR_STATIC, 0 );
    SPRINTF( textBuffer,
             "StartTest\t -- SetStmtAttr(CURSOR_TYPE) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( "select * from "PROGNAME" order by a\n" );
    retcode = SQLExecDirect( hstmt, "select a,b,c from "PROGNAME" order by a", SQL_NTS );
    SPRINTF( textBuffer,
             "StartTest\t -- ExecDirect   \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt, SQL_RETRIEVE_DATA, (SQLPOINTER) SQL_TRUE, 0 );
    SPRINTF( textBuffer,
             "StartTest\t -- SetStmtAttr \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */


    retcode = SQLBindCol( hstmt, 1, SQL_C_ULONG, &intValue, 0, &intLen );
    SPRINTF( textBuffer,
             "UpdateValues\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindCol( hstmt, 2, SQL_C_CHAR, &charValue1, CHAR_LEN, &charLen1 );
    SPRINTF( textBuffer,
             "UpdateValues\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindCol( hstmt, 3, SQL_C_CHAR, &charValue2, CHAR_LEN, &charLen2 );
    SPRINTF( textBuffer,
             "UpdateValues\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    for (actTest = tests; actTest->charValue1 != NULL; actTest++) {
        retcode = SQLExtendedFetch( hstmt,
                                    SQL_FETCH_NEXT, 0, &rowsFetched, &rowStatus );
        SPRINTF( textBuffer,
                 "StartTest\t -- ExtendedFetch \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS && retcode != SQL_NO_DATA_FOUND) {
            return retcode;
        }; /* if */

        STRCPY( charValue1, actTest->charValue1 );
        STRCPY( charValue2, actTest->charValue2 );
        intValue = actTest->intValue;
        intLen   = actTest->intLen;
        charLen1 = actTest->charLen1;
        charLen2 = actTest->charLen2;
        retcode = SQLSetPos( hstmt, 1, actTest->operation, SQL_LOCK_NO_CHANGE );
        SPRINTF( textBuffer,
                 "UpdateValues\t -- SetPos       \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
    }; /* for */

    return retcode;
} /* RunTest */


SQLRETURN ReadValues()
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLROWSETSIZE  rowsFetched = 0;
    UDWORD  intValue  = 17;
    SQLLEN  intLen  = sizeof(UDWORD);
    char    charValue1[CHAR_LEN+1];
    char    charValue2[CHAR_LEN+1];
    SQLLEN  charLen1 = 0;
    SQLLEN  charLen2 = 0;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "ReadValues\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CONCURRENCY,
                              (SQLPOINTER) SQL_CONCUR_ROWVER, 0 );
    SPRINTF( textBuffer,
             "ReadValues\t -- SetStmtAttr(CONCURRENCY) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt,
                              SQL_ATTR_CURSOR_TYPE,
                              (SQLPOINTER) SQL_CURSOR_STATIC, 0 );
    SPRINTF( textBuffer,
             "ReadValues\t -- SetStmtAttr(CURSOR_TYPE) \t retcode: %s"CRLF,
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

    retcode = SQLExecDirect( hstmt, "select * from "PROGNAME" order by a", SQL_NTS );
    SPRINTF( textBuffer,
             "ReadValue\t -- ExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 2, SQL_C_CHAR, &charValue1, CHAR_LEN, &charLen1 );
    SPRINTF( textBuffer,
             "ReadValues\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindCol( hstmt, 3, SQL_C_CHAR, &charValue2, CHAR_LEN, &charLen2 );
    SPRINTF( textBuffer,
             "ReadValues\t -- BindCol\t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    while (retcode == SQL_SUCCESS) {
        retcode = SQLExtendedFetch( hstmt,
                                    SQL_FETCH_NEXT, 0, &rowsFetched, NULL );
        SPRINTF( textBuffer,
                 "ReadValues\t -- ExtendedFetch \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_NO_DATA_FOUND) {
            if (retcode != SQL_SUCCESS) {
                return retcode;
            }; /* if */
            SPRINTF( textBuffer, "intValue = %d; intLen = %d\n", intValue, intLen );
            PRINTF( textBuffer );
            SPRINTF( textBuffer, "charValue1 = %s; charLen1 = %d\n",
                     charValue1, charLen1 );
            PRINTF( textBuffer );
            SPRINTF( textBuffer, "charValue2 = %s; charLen2 = %d\n",
                     charValue2, charLen2 );
            PRINTF( textBuffer );
        }; /* if */
    }; /* while */

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* ReadValues */
