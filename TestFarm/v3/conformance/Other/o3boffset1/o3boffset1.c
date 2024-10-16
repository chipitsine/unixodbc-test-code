/*!**********************************************************************

  module: o3boffset.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Use desc-field SQL_DESC_BIND_OFFSET_PTR to fetch data
                into array.

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

#define PROGNAME "O3BOFFSET"

#define ODBCVER 0x0350

#include "o3util.c"

#define CHAR_LEN 4000
#define NUM_VALUES 10

RETCODE InsertValues( HSTMT hstmt );
RETCODE FetchValues( HSTMT hstmt );

HDBC    hdbc  = SQL_NULL_HDBC;


struct st_rowbinding
{
    UCHAR    charVal[CHAR_LEN];
    SQLLEN   charIndi;
    SWORD    intVal;
    SQLLEN   intIndi;
}; /* struct st_colbinding */


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

    retcode = FetchValues( hstmt );

exit:
    retcode = DropTable( hdbc, PROGNAME );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
}; /* main */


RETCODE FetchValues( HSTMT hstmt )
{
    RETCODE               retcode;
    UCHAR                 textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR                 stmtStr[O3UTIL_STMT_LEN];
    struct st_rowbinding  data[NUM_VALUES];
    SDWORD                rowNum = 0;
    SDWORD                bindOffset = 0;
    /* bind columns */
    retcode = SQLBindCol( hstmt, 1,
                          SQL_C_CHAR, data[0].charVal, CHAR_LEN,
                          &data[0].charIndi );
    SPRINTF( textBuffer,
             "FetchValues -- SQLBindCol(1) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 2,
                          SQL_C_SHORT, &data[0].intVal, 0,
                          &data[0].intIndi );
    SPRINTF( textBuffer,
             "FetchValues\t -- SQLBindCol(2) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* execute select stmt */
    SPRINTF( stmtStr, "select a, b from %s order by b", PROGNAME );
    retcode = SQLExecDirect( hstmt, stmtStr, SQL_NTS );
    SPRINTF( textBuffer,
             "FetchValues\t -- SQLExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    /* set bind offset */
    retcode = SQLSetStmtAttr( hstmt, SQL_ATTR_ROW_BIND_OFFSET_PTR,
                              &bindOffset,
                              0 );
    SPRINTF( textBuffer,
             "FetchValues\t -- SQLSetStmtAttr(ROW_BIND_OFFSET) retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    rowNum = 0;
    do {
        retcode = SQLFetch( hstmt );
        SPRINTF( textBuffer,
                 "FetchValues\t -- SQLFetch \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            rowNum++;
            bindOffset = (rowNum*sizeof( struct st_rowbinding ));
        }; /* if */
    } while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO);
    if (retcode == SQL_NO_DATA_FOUND) {
        SDWORD iRow;
        for (iRow = 0; iRow < rowNum; iRow++) {
            SPRINTF( textBuffer, "data[%d]: %d(%d) %s(%d)\n",
                     iRow,
                     data[iRow].intVal, data[iRow].intIndi,
                     data[iRow].charVal, data[iRow].charIndi );
            PRINTF( textBuffer );
        }; /* for */
    }; /* if */
    return retcode;
}; /* FetchValues */


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
    for (i=1; (i <= NUM_VALUES && i < CHAR_LEN); i++) {
        intValue = i;
        STRCAT( charValue, "1234567890abcdefghijklmnopqrstuvwxyz" );
        charIndi = strlen( charValue );
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
}; /* InsertValues */
