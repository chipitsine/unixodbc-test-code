/*!**********************************************************************

  module: o3lparam.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Insert LONG value of length 0 and buffer=NULL
               with SQLPutData (PTS 1102741)

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

#define PROGNAME "O3LPARAM"

#define ODBCVER 0x0350

#include "o3util.c"

#define NUM_VALUES 1
#define CHAR_LEN 15000

char longBuf[CHAR_LEN];

/* Insert long values */
RETCODE InsertLong( HSTMT hstmt, UDWORD longLen1, UDWORD longLen2 );

/* show table contents */
RETCODE ShowTable( HSTMT hstmt );

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
                           "( a long byte, b int, c long byte )" );
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

    retcode = InsertLong( hstmt, 0, 10000 );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ShowTable( hstmt );
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
    SQLLEN  dataAtExec = SQL_DATA_AT_EXEC;
    SQLLEN  nextParam;         /* must be as long as a pointer! */

    SPRINTF( stmtStr, "insert into %s values (?,?,?)", PROGNAME );
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

    retcode = SQLBindParameter( hstmt, 1, SQL_PARAM_INPUT,
                                SQL_C_BINARY, SQL_LONGVARBINARY,
                                CHAR_LEN, 0,
                                (SQLPOINTER) 1,
                                0, &dataAtExec );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 2, SQL_PARAM_INPUT,
                                SQL_C_SLONG, SQL_INTEGER,
                                0, 0,
                                (SQLPOINTER) 2,
                                0, &dataAtExec );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 3, SQL_PARAM_INPUT,
                                SQL_C_BINARY, SQL_LONGVARBINARY,
                                0, 0,
                                (SQLPOINTER) 3,
                                0, &dataAtExec );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindParameter(3) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    if (longLen1 != SQL_NULL_DATA) {
        memcpy( binValue1, longBuf, longLen1 );
    };
    if (longLen2 != SQL_NULL_DATA) {
        memcpy( binValue2, longBuf, longLen2 );
    };

    SPRINTF( textBuffer, "InsertLong\t -- Insert Values"CRLF );
    PRINTF( textBuffer );
    intValue = longLen1;
    retcode = SQLExecute( hstmt );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLExecute (No %d)\t\t retcode: %s"CRLF,
             i, cnvret( retcode ));
    PRINTF( textBuffer );
    if (retcode != SQL_NEED_DATA) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
    };
    if (retcode == SQL_NEED_DATA) {
        retcode = SQLParamData( hstmt, (SQLPOINTER*) &nextParam );
        SPRINTF( textBuffer,
                 "InsertLong\t -- SQLParamData \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        while (retcode == SQL_NEED_DATA) {
            switch (nextParam) {
            case 1 :
                {
                    retcode = SQLPutData( hstmt, binValue1, longLen1 );
                    SPRINTF( textBuffer,
                      "InsertLong\t -- SQLPutData (No %d)\t\t retcode: %s"CRLF,
                             nextParam, cnvret( retcode ));
                    PRINTF( textBuffer );
                    ErrorCheck( SQL_HANDLE_STMT, hstmt );
                    break;
                }
            case 2 :
                {
                    retcode = SQLPutData( hstmt, &intValue, 0 );
                    SPRINTF( textBuffer,
                      "InsertLong\t -- SQLPutData (No %d)\t\t retcode: %s"CRLF,
                             nextParam, cnvret( retcode ));
                    PRINTF( textBuffer );
                    ErrorCheck( SQL_HANDLE_STMT, hstmt );
                    break;
                }
            case 3 :
                {
                    retcode = SQLPutData( hstmt, binValue2, longLen2 );
                    SPRINTF( textBuffer,
                      "InsertLong\t -- SQLPutData (No %d)\t\t retcode: %s"CRLF,
                             nextParam, cnvret( retcode ));
                    PRINTF( textBuffer );
                    ErrorCheck( SQL_HANDLE_STMT, hstmt );
                    break;
                }
            default:
                SPRINTF( textBuffer, "unexpected param no. %d"CRLF, nextParam );
                PRINTF( textBuffer );
                break;
            }; /* switch */
            retcode = SQLParamData( hstmt, (SQLPOINTER*) &nextParam );
            SPRINTF( textBuffer,
                     "InsertLong\t -- SQLParamData \t\t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
        }; /* while */
    }; /* if */
    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* InsertLong */


RETCODE ShowTable( HSTMT hstmt )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];
    char      binValue1[CHAR_LEN];
    char      binValue2[CHAR_LEN];
    UWORD     intValue;
    SQLLEN    binIndi1;
    SQLLEN    binIndi2;
    SQLLEN    intIndi;

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select a, b, c from %s", PROGNAME );
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
            retcode = SQLGetData( hstmt, 1, SQL_C_BINARY, binValue1, CHAR_LEN,
                                  &binIndi1 );
            SPRINTF( textBuffer,
                     "ShowTable\t -- SQLGetData(1) \t\t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            if (retcode != SQL_NO_DATA_FOUND) {
                ErrorCheck( SQL_HANDLE_STMT, hstmt );
            }; /* if */
            if (binIndi1 != SQL_NULL_DATA) {
                if (binIndi1>=0) {
                    binValue1[binIndi1] = '\0';
                }; /* if */
                ShowLongChar( binValue1 );
            } else {
                printf("NULL\n");
            }; /* else */

            if (retcode == SQL_NO_DATA_FOUND) {
                retcode = SQL_SUCCESS; /* we dont want to stop now */
            }; /* if */
            retcode = SQLGetData( hstmt, 3, SQL_C_BINARY, binValue2, CHAR_LEN,
                                  &binIndi2 );
            SPRINTF( textBuffer,
                     "ShowTable\t -- SQLGetData(3) \t\t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            if (retcode != SQL_NO_DATA_FOUND) {
                ErrorCheck( SQL_HANDLE_STMT, hstmt );
            }; /* if */
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                if (binIndi2 != SQL_NULL_DATA) {
                    if (binIndi2>=0) {
                        binValue2[binIndi2] = '\0';
                    }; /* if */
                    ShowLongChar( binValue2 );
                } else {
                    printf("NULL\n");
                }; /* else */
                SPRINTF( textBuffer,
                   "Fetch: int=%d indi1=%d strlen1=%d indi2=%d strlen2=%d "CRLF,
                         intValue,
                         binIndi1, STRLEN(binValue1),
                         binIndi2, STRLEN(binValue2) );
                PRINTF( textBuffer );
            }; /* if */

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
}; /* ShowTable */
