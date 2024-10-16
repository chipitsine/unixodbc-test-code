/*!**********************************************************************

  module: o3tables.c.OC

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Tests for the catalog function SQLTables

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

#define PROGNAME "o3tables"

#define ODBCVER 0x0350

#include "o3util.c"

#define F1_LEN 11

SQLRETURN TablesTest( HDBC hdbc );

void fetchout( HDBC hdbc, HSTMT hstmt );

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

    retcode = TablesTest( hdbc );

    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
}; /* main */


SQLRETURN TablesTest( HDBC hdbc )
{
    HSTMT hstmt;
    SQLRETURN retcode;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "TablesTest \t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLTables (hstmt, "%", SQL_NTS, "", 0, "", 0, NULL, 0);
    SPRINTF(textBuffer," SQLTables (qualifier scan) retcode: %d"CRLF, retcode);
    PRINTF(textBuffer);
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    fetchout( hdbc, hstmt );

    retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

    retcode = SQLTables (hstmt, "", 0, "%", SQL_NTS, "", 0, NULL, 0);
    SPRINTF( textBuffer,
             " SQLTables (owner scan) retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    fetchout( hdbc, hstmt );

    retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

    retcode = SQLTables (hstmt, NULL, 0, "%", SQL_NTS, "%", SQL_NTS,
                         "'TABLE'", SQL_NTS);
    SPRINTF(textBuffer," SQLTables (table scan) retcode: %s"CRLF,
            cnvret(retcode));
    PRINTF(textBuffer);
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    fetchout( hdbc, hstmt);
    retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

    retcode = SQLTables (hstmt, NULL, 0, "%", SQL_NTS, "%", SQL_NTS,
                         "'SYSTEM TABLE', 'TABLE'", SQL_NTS);
    SPRINTF( textBuffer," SQLTables (systemtable scan) retcode: %s"CRLF,
             cnvret(retcode));
    PRINTF(textBuffer);
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    fetchout( hdbc, hstmt);
    retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

    retcode = SQLTables(hstmt, NULL, 0, ODBCUSER, SQL_NTS, "ATMP", SQL_NTS,
                        "'TABLE'", SQL_NTS);
    SPRINTF(textBuffer," SQLTables (table scan) retcode: %s"CRLF,
            cnvret(retcode));
    PRINTF( textBuffer);

    fetchout( hdbc, hstmt);
    retcode = SQLFreeStmt(hstmt, SQL_DROP);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    return SQL_SUCCESS;
}; /* TablesTest */

void fetchout( HDBC hdbc, HSTMT hstmt )
{
    UCHAR szF1[F1_LEN];
    UCHAR szQUALIFIER[QUALIFIER_LEN+1];
    UCHAR szOWNER[OWNER_LEN+1];
    UCHAR szNAME[NAME_LEN+1];
    UCHAR szTYPE[TYPE_LEN+1];
    UCHAR szREMARKS[REMARKS_LEN+1];
    SDWORD cbF1, cbC1;
    SQLLEN cbQUALIFIER, cbOWNER, cbNAME, cbTYPE, cbREMARKS;
    RETCODE retcode;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szQUALIFIER,
                          sizeof(szQUALIFIER), &cbQUALIFIER);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szOWNER,
                          sizeof(szOWNER), &cbOWNER);
    SPRINTF( textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szNAME,
                          sizeof(szNAME), &cbNAME);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 4, SQL_C_CHAR, szTYPE,
                          sizeof(szTYPE), &cbTYPE);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 5, SQL_C_CHAR, szREMARKS,
                          sizeof(szREMARKS), &cbREMARKS);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    while (retcode == SQL_SUCCESS) {
        cbQUALIFIER = SQL_NULL_DATA;
        cbOWNER = SQL_NULL_DATA;
        cbNAME=SQL_NULL_DATA;
        cbTYPE = SQL_NULL_DATA;
        cbREMARKS=SQL_NULL_DATA;
        MEMSET( szQUALIFIER, 'x', sizeof(szQUALIFIER));
        MEMSET( szOWNER, 'x', sizeof(szOWNER));
        MEMSET( szNAME, 'x', sizeof(szNAME));
        MEMSET( szTYPE, 'x', sizeof(szTYPE));
        MEMSET( szREMARKS, 'x', sizeof(szREMARKS));
        retcode = SQLFetch (hstmt);
        if (retcode == SQL_SUCCESS_WITH_INFO) {
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
        }
        if (retcode == SQL_SUCCESS ){
            SPRINTF(textBuffer," SQLFetch retcode: '%s'"CRLF, cnvret(retcode));
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"TABLE_QUALIFIER = '%s':%d"CRLF,
                    szQUALIFIER, cbQUALIFIER);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"TABLE_OWNER = '%s':%d"CRLF, szOWNER, cbOWNER);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"TABLE_NAME = '%s':%d"CRLF, szNAME, cbNAME);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"TABLE_TYPE = '%s':%d"CRLF, szTYPE, cbTYPE);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"REMARKS = '%s':%d"CRLF, szREMARKS, cbREMARKS);
            PRINTF(textBuffer);
        }
        else {
            SPRINTF(textBuffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
            PRINTF(textBuffer);
        }
    }
}; /* fetchout */
