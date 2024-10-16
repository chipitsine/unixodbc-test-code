/*



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




*/
#ifndef O3CNVTAB_H
#define O3CNVTAB_H
/*!**********************************************************************

  module: o3cnvtab.h

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Structs for conversion tests (o3bigint, o3numeric, ... )

  -----------------------------------------------------------------------


************************************************************************/
#ifdef WIN32
typedef __int64 bigint;
#else
typedef long long bigint;
#endif

typedef struct {
    PTR     val;        /* pointer to value             */
    SWORD   fCTypeIn;   /* C type for inserting in table */
    SQLLEN *pcbValue;   /* strlen or indicator pointer  */
    UDWORD  cbDataSize; /* size of val                  */
} valueStruct;


typedef struct {
    SWORD        fCTypeOut;  /* C type for reading from table */
    PTR          val;        /* pointer to buffer for value   */
    SDWORD       bufLen;     /* size of buffer val            */
    SQLLEN      *lenIndiPtr; /* pointer string len or indicator variable */
} testTypesStruct;


typedef struct {
    SWORD fSqlType;           /* SQL type of column col               */
    UCHAR *comment;
    UCHAR *sCreateString;  /* table definition                     */
    valueStruct     *testValues;
    testTypesStruct *pTestTypes; /* tests to perform on this table    */
} testTabStruct;

RETCODE PrepareTable( HDBC hdbc, testTabStruct *test );

RETCODE ReadTable( HDBC hdbc, testTabStruct *test );

void ShowTestType( testTypesStruct *testType );

void ShowTestVal( valueStruct *testVal );

void ConversionTest( HDBC hdbc, testTabStruct *testTab )
{
    testTabStruct *test;
    RETCODE        retcode;
    UCHAR          textBuffer[1024];

    for (test = testTab; test->testValues!= NULL; test++) {
        PRINTF( CRLF );
        retcode = PrepareTable( hdbc, test );
        if ( retcode != SQL_SUCCESS) {
            return;
        }; /* if */
        retcode = ReadTable( hdbc, test );
        if ( retcode != SQL_SUCCESS) {
            return;
        }; /* if */
    }; /* for */

    retcode = DropTable( hdbc, "CNV" );
} /* ConversionTest */


RETCODE PrepareTable( HDBC hdbc, testTabStruct *test )
{
    RETCODE        retcode;
    HSTMT          hstmt;
    UCHAR          textBuffer[1024];
    SWORD          num;
    valueStruct   *testVal;

    retcode = CreateTable( hdbc, "CNV", test->sCreateString );
    if ( retcode != SQL_SUCCESS) {
        return retcode;
    } /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "PrepareTable\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* insert values */
    retcode = SQLBindParameter( hstmt, 1, SQL_PARAM_INPUT,
                                SQL_C_SHORT, SQL_INTEGER,
                                0, 0, &num, 0, NULL );
    SPRINTF( textBuffer,
             "PrepareTable\t -- SQLBindParameter \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    num = 1;
    SPRINTF( textBuffer,
             "Insert values for %s (%s).\n",
             SqlTypeToString( test->fSqlType ), test->comment );
    for (testVal = test->testValues; testVal->val != NULL; testVal++) {
        retcode = SQLBindParameter( hstmt, 2, SQL_PARAM_INPUT,
                                    testVal->fCTypeIn, test->fSqlType,
                                    0, 0, testVal->val, 0, testVal->pcbValue );
        SPRINTF( textBuffer,
                 "PrepareTable\t -- SQLBindParameter \t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */

        *testVal->pcbValue = testVal->cbDataSize;
        ShowTestVal( testVal );
        if (testVal->fCTypeIn == SQL_C_NUMERIC) {
            SQLHDESC hdescAPD = SQL_NULL_HDESC;
            retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_APP_PARAM_DESC,
                                      &hdescAPD, 0, NULL );
            SPRINTF( textBuffer,
                     "PrepareTable\t -- SQLGetStmtAttr\t\t\t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
            /* set desc-field SQL_SCALE */
            retcode = SQLSetDescField( hdescAPD, 2, SQL_DESC_SCALE,
                                       (PTR)
                                       ((SQL_NUMERIC_STRUCT*)testVal->val)->scale,
                                       0 );
            SPRINTF( textBuffer,
                     "PrepareTable\t -- SQLSetDescField(SCALE)\t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
        }; /* if */
        retcode = SQLExecDirect( hstmt, "insert into cnv values (?,?)",
                                 SQL_NTS );
        SPRINTF( textBuffer,
                 "PrepareTable\t -- SQLExecDirect\t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        num++;
    }; /* for */

    SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    return SQL_SUCCESS;
} /* PrepareTable */


RETCODE ReadTable( HDBC hdbc, testTabStruct *test )
{
    RETCODE          retcode;
    HSTMT            hstmt;
    UCHAR            textBuffer[1024];
    SWORD            num;
    testTypesStruct *testType;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "PrepareTable\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    for (testType = test->pTestTypes; testType->fCTypeOut != 0; testType++) {
        SPRINTF( textBuffer, "ReadTable \t -- Start reading: %s"CRLF,
                 CTypeToString( testType->fCTypeOut) );
        PRINTF( textBuffer );
        retcode = SQLBindCol( hstmt, 1, SQL_C_SHORT, &num, 0, NULL );
        SPRINTF( textBuffer,
                 "ReadTable\t -- SQLBindCol \t\t \t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
        retcode = SQLBindCol( hstmt, 2, testType->fCTypeOut,
                              testType->val, testType->bufLen,
                              testType->lenIndiPtr );
        SPRINTF( textBuffer,
                 "ReadTable\t -- SQLBindCol \t\t \t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */

        retcode = SQLExecDirect( hstmt, "select * from cnv order by n",
                                 SQL_NTS );
        SPRINTF( textBuffer,
                 "ReadTable\t -- SQLExecDirect \t \t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
        do {
            *testType->lenIndiPtr = -1;
            MEMSET( testType->val, 0, testType->bufLen );
            retcode = SQLFetch( hstmt );
            SPRINTF( textBuffer,
                     "ReadTable\t -- SQLFetch \t\t \t retcode: %s"CRLF,
                     cnvret( retcode ));
            PRINTF( textBuffer );
            ErrorCheck( SQL_HANDLE_STMT, hstmt );
            if (retcode != SQL_NO_DATA_FOUND && retcode != SQL_ERROR) {
                SPRINTF( textBuffer, "num=%d ", num );
                PRINTF( textBuffer );
                ShowTestType( testType );
            }; /* if */
        } while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO
                 || retcode == SQL_ERROR);

        retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
        SPRINTF( textBuffer,
                 "ReadTable\t -- SQLFreeStmt \t \t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
    }; /* for */

    SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    return retcode;
} /* ReadTable */


void ShowNumeric( SQL_NUMERIC_STRUCT *numeric )
{
    UCHAR textBuffer[1024];
    SWORD i;

    SPRINTF( textBuffer, "prec=%d scale=%d sign=%d\n ",
             numeric->precision, numeric->scale, numeric->sign );
    PRINTF( textBuffer );
    for (i=0; i<SQL_MAX_NUMERIC_LEN; i++ ) {
        SPRINTF( textBuffer, "%2x ", numeric->val[i] );
        PRINTF( textBuffer );
    }; /* for */
    PRINTF( CRLF );
} /* ShowNumeric */


void ShowBigint( bigint n )
{
    UCHAR textBuffer[1024];
    SWORD i, j = 0;
    int di = 1;
    char *q = (char*) &di;

    SPRINTF( textBuffer, "SDWORD=%d  ", (SDWORD) n );
    PRINTF( textBuffer );

    if (*q == 0) {  /* endian? */
      j = 7;
      di = -1;
    }

    for (i=0; i<8; i++, j+=di) {
        SPRINTF( textBuffer, "%x ", *((UCHAR*)&n+j) );
        PRINTF( textBuffer );
    }; /* for */
    PRINTF( CRLF );
} /* ShowBigint */


void ShowValue( SWORD cType, PTR val, SQLLEN *lenIndiPtr );

void ShowTestVal( valueStruct *testVal )
{
    ShowValue( testVal->fCTypeIn, testVal->val, testVal->pcbValue );
} /* ShowTestVal */


void ShowTestType( testTypesStruct *testType )
{
    ShowValue( testType->fCTypeOut, testType->val, testType->lenIndiPtr );
} /* ShowTestType */


void ShowValue( SWORD cType, PTR val, SQLLEN *lenIndiPtr )
{
    UCHAR textBuffer[1024];

    switch (cType) {
    case SQL_C_CHAR:
        SPRINTF( textBuffer, "%s: value=%s lenIndi=%d"CRLF,
                 CTypeToString(cType),
                 val, *lenIndiPtr );
        PRINTF( textBuffer );
        break;
    case SQL_C_SHORT:
        SPRINTF( textBuffer, "%s: value=%d lenIndi=%d"CRLF,
                 CTypeToString(cType),
                 *((SWORD*) val), *lenIndiPtr );
        PRINTF( textBuffer );
        break;
    case SQL_C_LONG:
        SPRINTF( textBuffer, "%s: value=%d lenIndi=%d"CRLF,
                 CTypeToString(cType),
                 *((SDWORD*) val), *lenIndiPtr );
        PRINTF( textBuffer );
        break;
    case SQL_C_FLOAT:
        SPRINTF( textBuffer, "%s: value=%f lenIndi=%d"CRLF,
                 CTypeToString(cType),
                 *((float*) val), *lenIndiPtr );
        PRINTF( textBuffer );
        break;
    case SQL_C_DOUBLE:
        SPRINTF( textBuffer, "%s: value=%f lenIndi=%d"CRLF,
                 CTypeToString(cType),
                 *((double*) val), *lenIndiPtr );
        PRINTF( textBuffer );
        break;
    case SQL_C_NUMERIC:
        SPRINTF( textBuffer, "%s: ", CTypeToString(cType) );
        PRINTF( textBuffer );
        ShowNumeric( (SQL_NUMERIC_STRUCT*) val );
        break;
    case SQL_C_UBIGINT:
        SPRINTF( textBuffer, "%s: ", CTypeToString(cType) );
        PRINTF( textBuffer );
        ShowBigint( *((bigint*)val) );
        break;
    case SQL_C_SBIGINT:
        SPRINTF( textBuffer, "%s: ", CTypeToString(cType) );
        PRINTF( textBuffer );
        ShowBigint( *((bigint*)val) );
        break;
    default:
        PRINTF( "C type unknown/not implemented."CRLF );
        break;
    }; /* switch */
} /* ShowTestType */

#endif
