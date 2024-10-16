/*!**********************************************************************

  module: o3desc4.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Set descriptor fields.

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

#define PROGNAME "o3desc4"

#define ODBCVER 0x0350

#define CHAR_LEN   100000

#include "o3util.c"
#include "o3utdesc.c"

RETCODE SetARD( SQLHSTMT hstmt );
RETCODE SetAPD( SQLHSTMT hstmt );
RETCODE SetIRD( SQLHSTMT hstmt );
RETCODE SetIPD( SQLHSTMT hstmt );


void ErrorCheckLocal( SWORD handleType, SQLHANDLE handle )
{
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    RETCODE retcode;
    SDWORD  rowCount;
    SDWORD  columnNumber;
    SDWORD  rowNumber;
    UCHAR   messageText[O3UTIL_TEXT_LEN];
    SDWORD  native;
    UCHAR   sqlstate[O3UTIL_SQLSTATE_LEN];
    SWORD   recNumber;
    SWORD   textLength;
    SWORD   dummyLength;

    retcode = SQLGetDiagField( handleType, handle,
                               0,
                               SQL_DIAG_NUMBER,
                               &rowCount,
                               0,
                               &dummyLength );
    if (retcode != SQL_SUCCESS) {
        SPRINTF( (char*) textBuffer,
                 "ErrorCheckLocal\t -- SQLGetDiagField(DIAG_NUMBER) retcode: %s"CRLF,
                 cnvret(retcode));
        PRINTF( textBuffer );
    }; /* if */
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        if (rowCount > 0) { /* Errors detected */
            SPRINTF( (char*) textBuffer, "%d Error(s):"CRLF, rowCount );
            PRINTF( textBuffer );
            for (recNumber = 1; recNumber <= rowCount; recNumber++ ) {
                retcode = SQLGetDiagRec( handleType,
                                         handle,
                                         recNumber,
                                         sqlstate,
                                         &native,
                                         messageText,
                                         O3UTIL_TEXT_LEN,
                                         &textLength );
                if (retcode != SQL_SUCCESS) {
                    SPRINTF( (char*) textBuffer,
                             "ErrorCheckLocal\t-- SQLGetDiagRec retcode: %s"CRLF,
                             cnvret(retcode));
                    PRINTF( textBuffer );
                }; /* if */

                /* different states between Windows driver manager and Unix stand alone driver */
                if (strcmp (sqlstate, "HY091") == 0  ||
                    strcmp (sqlstate, "HY092") == 0  ||
                    strcmp (sqlstate, "HY105") == 0  ||
                    strcmp (sqlstate, "HY016") == 0)
                  continue;


                if ( retcode == SQL_SUCCESS
                     || retcode == SQL_SUCCESS_WITH_INFO) {
                    SPRINTF( (char*) textBuffer,
                             "ErrorCheckLocal\t -- recNumber: %d  sqlstate: %s  native: %d"CRLF,
                             recNumber, sqlstate, native );
                    PRINTF( textBuffer );
                    SPRINTF( (char*) textBuffer, "messageText: %s"CRLF,
                             messageText );
                    PRINTF( textBuffer );
                }; /* if */
            }; /* for */
        }; /* if */
    }; /* if */
} /* ErrorCheckLocal */


int main(int argc, argv_t argv) {
    RETCODE retcode;
    SQLHENV    henv  = SQL_NULL_HENV;
    SQLHDBC    hdbc  = SQL_NULL_HDBC;
    SQLHSTMT   hstmt = SQL_NULL_HSTMT;
    char   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SetARD( hstmt );
    retcode = SetAPD( hstmt );
    retcode = SetIRD( hstmt );
    retcode = SetIPD( hstmt );


exit:
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* MAIN */


RETCODE SetDescAll( SQLHDESC hdesc );


RETCODE SetARD( SQLHSTMT hstmt )
{
    RETCODE   retcode = SQL_SUCCESS;
    char     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLHDESC  hdesc;

    /* Get application Row Desc */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_APP_ROW_DESC, &hdesc, 0, NULL );
    SPRINTF( textBuffer,
             "SetARD\t\t -- GetStmtAttr(ARD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( CRLF"ARD:"CRLF );
    ShowDescAll( hdesc );
    retcode = SQLSetDescField( hdesc, 0,
                               SQL_DESC_COUNT,
                               (SQLPOINTER) 1, 0 );
    SPRINTF( textBuffer,
             "SetARD\t\t -- SetDescField(COUNT) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );

    retcode = SetDescAll( hdesc );

    ShowDescAll( hdesc );

    return retcode;
} /* SetARD */


RETCODE SetAPD( SQLHSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    char     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLHDESC  hdesc;

    /* Get application Row Desc */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_APP_PARAM_DESC,&hdesc, 0, NULL );
    SPRINTF( textBuffer,
             "SetAPD\t\t -- GetStmtAttr(ARD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( CRLF"APD:"CRLF );
    ShowDescAll( hdesc );
    retcode = SQLSetDescField( hdesc, 0,
                               SQL_DESC_COUNT,
                               (SQLPOINTER) 1, 0 );
    SPRINTF( textBuffer,
             "SetAPD\t\t -- SetDescField(COUNT) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );

    retcode = SetDescAll( hdesc );

    ShowDescAll( hdesc );

    return retcode;
} /* SetAPD */


RETCODE SetIRD( SQLHSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    char     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLHDESC  hdesc;

    /* Get Impl Row Descriptor */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_IMP_ROW_DESC, &hdesc, 0, NULL );
    SPRINTF( textBuffer,
             "SetIRD\t\t -- GetStmtAttr(IRD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */


    PRINTF( CRLF"IRD:"CRLF );
    ShowDescAll( hdesc );
    retcode = SQLSetDescField( hdesc, 0,
                               SQL_DESC_COUNT,
                               (SQLPOINTER) 1, 0 );
    SPRINTF( textBuffer,
             "SetIRD\t\t -- SetDescField(COUNT) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );

    retcode = SetDescAll( hdesc );

    ShowDescAll( hdesc );

    return retcode;
} /* SetIRD */


RETCODE SetIPD( SQLHSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    char     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLHDESC  hdesc;

    /* Get impl param Descriptor */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_IMP_PARAM_DESC,&hdesc, 0, NULL );
    SPRINTF( textBuffer,
             "SetIPD\t\t -- GetStmtAttr(ARD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    PRINTF( CRLF"IPD:"CRLF );
    ShowDescAll( hdesc );
    retcode = SQLSetDescField( hdesc, 0,
                               SQL_DESC_COUNT,
                               (SQLPOINTER) 1, 0 );
    SPRINTF( textBuffer,
             "SetIPD\t\t -- SetDescField(COUNT) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );

    retcode = SetDescAll( hdesc );

    ShowDescAll( hdesc );

    return retcode;
} /* SetIPD */


RETCODE SetDescField( SQLHDESC hdesc, descFieldType *field );


RETCODE SetDescAll( SQLHDESC hdesc )
{
    RETCODE retcode = SQL_SUCCESS;
    SWORD   numHeaderFields = sizeof(headerFields) / sizeof(descFieldType);
    SWORD   numDescFields = sizeof(descFields) / sizeof(descFieldType);
    SWORD   numRecords = 0;
    SWORD   i, rec;
    char   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    /* set header fields */
    for (i=0; i<numHeaderFields; i++) {
        retcode = SetDescField( hdesc, &headerFields[i] );
    }; /* for */

    /* set record fields */
    for (i=0; i<numDescFields; i++) {
        retcode = SetDescField( hdesc, &descFields[i] );
    }; /* for */

    return SQL_SUCCESS;
} /* SetDescAll */



RETCODE SetDescField( SQLHDESC hdesc, descFieldType *field )
{
    RETCODE retcode = SQL_SUCCESS;
    SQLLEN  intValue = 0;
    SWORD   smallintValue = 0;
    UWORD   usmallintValue = 0;
    SQLULEN uintValue = 0;
    UCHAR   charValue[80];
    char   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SWORD   recNo = 1;

    switch ( field->dataType ) {
    case O3TYPE_SQLSMALLINT:
        smallintValue = 3;
        retcode = SQLSetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   (SQLPOINTER) smallintValue,
                                   sizeof( smallintValue ) );
        SPRINTF( textBuffer,
                 "SetDescField\t -- SetDescField(%s)  retcode: %s"CRLF,
                 field->name,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );
        break;
    case O3TYPE_SQLINTEGER:
        intValue = 2;
        retcode = SQLSetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   (SQLPOINTER) intValue,
                                   sizeof( intValue ) );
        SPRINTF( textBuffer,
                 "SetDescField\t -- SetDescField(%s)  retcode: %s"CRLF,
                 field->name,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );
        break;
    case O3TYPE_SQLLEN:
        intValue = 2;
        retcode = SQLSetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   (SQLPOINTER) intValue,
                                   sizeof( intValue ) );
        SPRINTF( textBuffer,
                 "SetDescField\t -- SetDescField(%s)  retcode: %s"CRLF,
                 field->name,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );
        break;
    case O3TYPE_SQLUINTEGER:
        uintValue = 4;
        retcode = SQLSetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   (SQLPOINTER) uintValue,
                                   sizeof( uintValue ) );
        SPRINTF( textBuffer,
                 "SetDescField\t -- SetDescField(%s)  retcode: %s"CRLF,
                 field->name,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );

        break;
    case O3TYPE_SQLPOINTER:
        uintValue = 1234;
        retcode = SQLSetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   (SQLPOINTER) uintValue,
                                   sizeof( uintValue ) );
        SPRINTF( textBuffer,
                 "SetDescField\t -- SetDescField(%s)  retcode: %s"CRLF,
                 field->name,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );
        break;
    case O3TYPE_SQLCHARS:
        STRCPY( charValue, field->name );
        retcode = SQLSetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   charValue,
                                   sizeof( charValue ) );
        SPRINTF( textBuffer,
                 "SetDescField\t -- SetDescField(%s)  retcode: %s"CRLF,
                 field->name,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ErrorCheckLocal( SQL_HANDLE_DESC, hdesc );
        break;
    default:
        break;
    }; /* switch */

    return SQL_SUCCESS;
} /* SetDescField */
