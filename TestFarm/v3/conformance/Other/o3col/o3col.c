/*!**********************************************************************

  module: o3col.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Erzeugung einer Tabelle mit mehr als 1000 columns, dann
               select auf domain.columns. Die Spalte Pos war als FIXED(3)
               deklariert, so da� bei der Konvertierung ab der 1000sten
               Zeile eine -811 auftrat.

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

#define PROGNAME "o3col4"

#define ODBCVER 0x0350

#include "o3util.c"

#define NUM_COLS 1023

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HDBC    hdbc  = SQL_NULL_HDBC;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SWORD   iCol;
    UCHAR   tableDesc[NUM_COLS*20];
    SQLROWSETSIZE   crow;

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );
    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    STRCPY( tableDesc, "CREATE TABLE O3COL4 ( COL_0 CHAR(2)" );
    for (iCol = 1; iCol<NUM_COLS; iCol++) {
        SPRINTF( textBuffer, ",C%d CHAR(6)", iCol );
        STRCAT( tableDesc, textBuffer );
    }; /* for */
    STRCAT( tableDesc, ")" );


    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLExecDirect( hstmt, tableDesc, SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLSetStmtAttr( hstmt, SQL_CURSOR_TYPE,
                              (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLSetStmtAttr \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode =
        SQLExecDirect( hstmt,
                       "SELECT * FROM DOMAIN.COLUMNS WHERE TABLENAME='O3COL4'",
                       SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLExtendedFetch( hstmt, SQL_FETCH_ABSOLUTE, 999, &crow, NULL );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExtendedFetch \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLExtendedFetch( hstmt, SQL_FETCH_ABSOLUTE, 1000, &crow, NULL );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExtendedFetch \t retcode: %s"CRLF,
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
}; /* main */
