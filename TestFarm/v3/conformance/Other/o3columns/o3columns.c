/*!**********************************************************************

  module: o3columns.c.OC

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Tests for the catalog function SQLColumns

  -----------------------------------------------------------------------





    ========== licence begin  GPL
    Copyright (c) 2000-2004 SAP AG

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

#define PROGNAME "o3columns"

#include "tst.h"
#include "o3util.h"

#define F1_LEN 11

SQLRETURN ColumnsTest( SQLHDBC hdbc );

void fetchout( SQLHDBC hdbc, SQLHSTMT hstmt );

int main(int argc, argv_t argv) {
    RETCODE retcode;
    SQLHENV    henv  = SQL_NULL_HENV;
    SQLHDBC    hdbc  = SQL_NULL_HDBC;
    SQLHSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = ColumnsTest( hdbc );

    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */


SQLRETURN ColumnsTest( SQLHDBC hdbc )
{
    SQLHSTMT hstmt;
    SQLRETURN retcode;
    char   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "TablesTest \t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLColumns (hstmt, (UCHAR*)"", 0, (UCHAR*)SUPERUID, SQL_NTS, (UCHAR*)"SYSODBCTYPES", SQL_NTS, (UCHAR*)"", 0);
    SPRINTF(textBuffer," SQLColumns (qualifier scan) retcode: %d"CRLF, retcode);
    PRINTF(textBuffer);
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    fetchout( hdbc, hstmt );

    retcode = SQLFreeStmt(hstmt, SQL_DROP);

    return SQL_SUCCESS;
} /* ColumnsTest */


void fetchout( SQLHDBC hdbc, SQLHSTMT hstmt )
{
    UCHAR szF1[F1_LEN];
    UCHAR szQUALIFIER[QUALIFIER_LEN+1];
    UCHAR szOWNER[OWNER_LEN+1];
    UCHAR szNAME[NAME_LEN+1];
    UCHAR szCOLNAME[NAME_LEN+1];
    SWORD szDATATYPE;
    SWORD szSQLTYPE;
    UCHAR szTYPENAME[NAME_LEN+1];
    UCHAR szCOLDEF[NAME_LEN+1];
    UCHAR szREMARKS[REMARKS_LEN+1];
    SWORD szDIGITS, szNUMPRECRADIX, szNULLABLE, szDATETIMESUB;
    UCHAR szISNULLABLE[NAME_LEN+1];
    SDWORD szOCTETLEN;
    SDWORD szCOLSIZE;
    SDWORD szBUFLEN;
    SDWORD szORDPOS;
    SDWORD cbF1, cbC1;
    SQLLEN cbQUALIFIER, cbOWNER, cbNAME, cbDATATYPE, cbTYPENAME;
    SQLLEN cbREMARKS, cbCOLNAME, cbCOLSIZE, cbBUFLEN;
    SQLLEN cbDIGITS, cbNUMPRECRADIX, cbNULLABLE, cbSQLTYPE, cbCOLDEF;
    SQLLEN cbDATETIMESUB, cbOCTETLEN, cbORDPOS, cbISNULLABLE;
    RETCODE retcode;
    char   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

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

    retcode = SQLBindCol (hstmt, 4, SQL_C_CHAR, szCOLNAME,
                          sizeof(szCOLNAME), &cbCOLNAME);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 5, SQL_C_SHORT, &szDATATYPE,
                          sizeof(szDATATYPE), &cbDATATYPE);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 6, SQL_C_CHAR, szTYPENAME,
                          sizeof(szTYPENAME), &cbTYPENAME);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 7, SQL_C_LONG, &szCOLSIZE,
                          sizeof(szCOLSIZE), &cbCOLSIZE);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 8, SQL_C_LONG, &szBUFLEN,
                          sizeof(szBUFLEN), &cbBUFLEN);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 9, SQL_C_SHORT, &szDIGITS,
                          sizeof(szDIGITS), &cbDIGITS);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 10, SQL_C_SHORT, &szNUMPRECRADIX,
                          sizeof(szNUMPRECRADIX), &cbNUMPRECRADIX);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 11, SQL_C_SHORT, &szNULLABLE,
                          sizeof(szNULLABLE), &cbNULLABLE);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 12, SQL_C_CHAR, szREMARKS,
                          sizeof(szREMARKS), &cbREMARKS);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 13, SQL_C_CHAR, szCOLDEF,
                          sizeof(szCOLDEF), &cbCOLDEF);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 14, SQL_C_SHORT, &szSQLTYPE,
                          sizeof(szSQLTYPE), &cbSQLTYPE);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 15, SQL_C_SHORT, &szDATETIMESUB,
                          sizeof(szDATETIMESUB), &cbDATETIMESUB);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 16, SQL_C_LONG, &szOCTETLEN,
                          sizeof(szOCTETLEN), &cbOCTETLEN);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 17, SQL_C_LONG, &szORDPOS,
                          sizeof(szORDPOS), &cbORDPOS);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    retcode = SQLBindCol (hstmt, 18, SQL_C_CHAR, szISNULLABLE,
                          sizeof(szISNULLABLE), &cbISNULLABLE);
    SPRINTF(textBuffer," SQLBindCol retcode: %s"CRLF, cnvret(retcode));
    PRINTF(textBuffer);

    while (retcode == SQL_SUCCESS) {
        cbQUALIFIER = SQL_NULL_DATA;
        cbOWNER = SQL_NULL_DATA;
        cbNAME=SQL_NULL_DATA;
        cbDATATYPE = SQL_NULL_DATA;
        cbREMARKS=SQL_NULL_DATA;
        szDATETIMESUB = 0;
        szNUMPRECRADIX = 0;
        szDIGITS = cbDIGITS = 4444;
        MEMSET( szQUALIFIER, 'x', sizeof(szQUALIFIER));
        MEMSET( szOWNER, 'x', sizeof(szOWNER));
        MEMSET( szNAME, 'x', sizeof(szNAME));
        MEMSET( szTYPENAME, 'x', sizeof(szTYPENAME));
        MEMSET( szREMARKS, 'x', sizeof(szREMARKS));
        retcode = SQLFetch (hstmt);
        if (retcode == SQL_SUCCESS_WITH_INFO || retcode == SQL_ERROR) {
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
            SPRINTF(textBuffer,"COLUMN_NAME = '%s':%d"CRLF, szCOLNAME, cbCOLNAME);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"DATA_TYPE = %d:%d"CRLF, szDATATYPE, cbDATATYPE);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"TYPE_NAME = '%s':%d"CRLF, szTYPENAME, cbTYPENAME);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"COLUMN_SIZE = %d:%d"CRLF, szCOLSIZE, cbCOLSIZE);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"BUFFER_LENGTH = %d:%d"CRLF, szBUFLEN, cbBUFLEN);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"DECIMAL_DIGITS = %d:%d"CRLF, szDIGITS, cbDIGITS);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"NUM_PREC_RADIX = %d:%d"CRLF, szNUMPRECRADIX, cbNUMPRECRADIX);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"NULLABLE = %d:%d"CRLF, szNULLABLE, cbNULLABLE);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"REMARKS = '%s':%d"CRLF, szREMARKS, cbREMARKS);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"COLUMN_DEF = '%s':%d"CRLF, szCOLDEF, cbCOLDEF);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"SQL_DATA_TYPE = %d:%d"CRLF, szSQLTYPE, cbSQLTYPE);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"DATETIMESUB = %d:%d"CRLF, szDATETIMESUB, cbDATETIMESUB);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"CHAR_OCTET_LENGTH = %d:%d"CRLF, szOCTETLEN, cbOCTETLEN);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"ORDINAL_POSTITION = %d:%d"CRLF, szORDPOS, cbORDPOS);
            PRINTF(textBuffer);
            SPRINTF(textBuffer,"IS_NULLABLE = %s:%d"CRLF, szISNULLABLE, cbISNULLABLE);
            PRINTF(textBuffer);
        }
        else {
            SPRINTF(textBuffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
            PRINTF(textBuffer);
        }
    }
} /* fetchout */
