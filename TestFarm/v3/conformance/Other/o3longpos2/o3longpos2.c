/*!**********************************************************************

  module: o3longpos2.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Positional reading of long data

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

#define PROGNAME "O3LONGPOS2"

#define ODBCVER 0x0350

#include "o3util.c"

#define NUM_VALUES 1
#define CHAR_LEN 150000

char longBuf[CHAR_LEN];

/* Insert long values */
RETCODE InsertLong( HSTMT hstmt, UDWORD longLen );

/* show table contents */
RETCODE ShowTable( HSTMT hstmt, UDWORD longPos );

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
    retcode = InsertLong( hstmt, 100000 );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ShowTable( hstmt, 97000 );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */


RETCODE InsertLong( HSTMT hstmt, UDWORD longLen )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    int     i;
    char    binValue[CHAR_LEN];
    int     intValue;
    SQLLEN  binIndi;

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
                                SQL_C_CHAR,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &binValue,
                                CHAR_LEN, &binIndi );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
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
             "InsertLong\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    memcpy( binValue, longBuf, longLen );
    binValue[longLen] = '\0';
    SPRINTF( textBuffer, "InsertLong\t -- Insert %d Values"CRLF, NUM_VALUES );
    PRINTF( textBuffer );
    for (i=0; (i < NUM_VALUES && i < CHAR_LEN); i++) {
        binIndi = strlen( binValue );
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
    /* Insert NULL Value */
#if 0
    intValue++;
    binIndi = SQL_NULL_DATA;
    retcode = SQLExecute( hstmt );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
#endif
    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* InsertLong */

RETCODE ShowTable( HSTMT hstmt, UDWORD longPos )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];
    char      binValue[CHAR_LEN];
    UWORD     intValue;
    SQLLEN    binIndi;
    SQLLEN    intIndi;

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select a, b from %s order by b", PROGNAME );
    SPRINTF( textBuffer, "ShowTable\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLExecDirect( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 2,
                          SQL_C_SHORT, &intValue, 0, &intIndi );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLBindCol(2) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* show content of table */
    PRINTF( "ShowTable\t -- Content of table"CRLF );
    do {
        retcode = SQLFetch( hstmt );
        SPRINTF( textBuffer,
                 "ShowTable\t -- SQLFetch \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLSetStmtAttr( hstmt, 3003 /* SQL_ATTR_READ_LONG_POS */,
                                      (SQLPOINTER) (SQLLEN) longPos, 0 );
            SPRINTF( textBuffer,
                     "ShowTable\t -- SetStmtAttr(READ_LONG_POS=%d)\t retcode: %s"CRLF,
                     longPos, cnvret( retcode ));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
            do {
                retcode = SQLGetData( hstmt, 1,
                                      SQL_C_CHAR, binValue, 2000,
                                      &binIndi );
                SPRINTF( textBuffer,
                         "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
                         cnvret( retcode ));
                PRINTF( textBuffer );
                if (retcode != SQL_NO_DATA_FOUND) {
                    ErrorCheck( SQL_HANDLE_STMT, hstmt );
                }; /* if */
                if (retcode == SQL_SUCCESS
                    || retcode == SQL_SUCCESS_WITH_INFO) {
                    if (binIndi != SQL_NULL_DATA)
                        ShowLongChar( binValue );
                    else
                        printf("NULL\n");

                    SPRINTF( textBuffer, "Fetch Row: %d %d strlen=%d "CRLF,
                             intValue, binIndi, STRLEN(binValue) );
                    PRINTF( textBuffer );
                }; /* if */
            } while (retcode == SQL_SUCCESS_WITH_INFO);

            if (retcode == SQL_NO_DATA_FOUND) {
                retcode = SQL_SUCCESS; /* we dont want to stop now */
            }; /* if */
        } else {
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
        }; /* else */
    } while (retcode == SQL_SUCCESS);

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLFreeStmt(CLOSE)\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
} /* ShowTable */
