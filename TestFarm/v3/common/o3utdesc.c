
/* Show all descriptor fields


    ========== licence begin  GPL
    Copyright (c) 2001-2004 SAP AG

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
#include "tst.h"


RETCODE ShowStmtDesc( HSTMT hstmt )
{
    RETCODE   retcode     = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLHDESC  ard = SQL_NULL_HDESC;
    SQLHDESC  apd = SQL_NULL_HDESC;
    SQLHDESC  ird = SQL_NULL_HDESC;
    SQLHDESC  ipd = SQL_NULL_HDESC;

    /* Get application Row Descriptor */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_APP_ROW_DESC, &ard, 0, NULL );
    SPRINTF( textBuffer,
             "ShowStmtDesc\t -- GetStmtAttr(ARD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    ShowDescAll( ard );

    /* Get application param descriptor */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_APP_PARAM_DESC, &apd, 0, NULL );
    SPRINTF( textBuffer,
             "ShowStmtDesc\t -- GetStmtAttr(APD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    ShowDescAll( apd );

    /* Get impl Row Descriptor */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_IMP_ROW_DESC, &ird, 0, NULL );
    SPRINTF( textBuffer,
             "ShowStmtDesc\t -- GetStmtAttr(IRD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    ShowDescAll( ird );

    /* Get impl param Descriptor */
    retcode = SQLGetStmtAttr( hstmt, SQL_ATTR_IMP_PARAM_DESC, &ipd, 0, NULL );
    SPRINTF( textBuffer,
             "ShowStmtDesc\t -- GetStmtAttr(IPD) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    ShowDescAll( ipd );

    return retcode;
}; /* ShowStmtDesc */

static descFieldType headerFields[] = {
    { SQL_DESC_ALLOC_TYPE                    , "SQL_DESC_ALLOC_TYPE       ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_ARRAY_SIZE                    , "SQL_DESC_ARRAY_SIZE       ",
      O3TYPE_SQLUINTEGER },
    { SQL_DESC_ARRAY_STATUS_PTR              , "SQL_DESC_ARRAY_STATUS_PTR ",
      O3TYPE_SQLPOINTER },
    { SQL_DESC_BIND_OFFSET_PTR               , "SQL_DESC_BIND_OFFSET_PTR  ",
      O3TYPE_SQLPOINTER },
    { SQL_DESC_BIND_TYPE                     , "SQL_DESC_BIND_TYPE        ",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_ROWS_PROCESSED_PTR            , "SQL_DESC_ROWS_PROCESSED_PTR",
      O3TYPE_SQLPOINTER }
}; /* headerFields */

static descFieldType descFields[] = {
    { SQL_DESC_AUTO_UNIQUE_VALUE             , "SQL_DESC_AUTO_UNIQUE_VALUE",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_BASE_COLUMN_NAME              , "SQL_DESC_BASE_COLUMN_NAME ",
      O3TYPE_SQLCHARS },
    { SQL_DESC_BASE_TABLE_NAME               , "SQL_DESC_BASE_TABLE_NAME  ",
      O3TYPE_SQLCHARS },
    { SQL_DESC_CASE_SENSITIVE                , "SQL_DESC_CASE_SENSITIVE   ",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_CATALOG_NAME                  , "SQL_DESC_CATALOG_NAME     ",
      O3TYPE_SQLCHARS },
    { SQL_DESC_CONCISE_TYPE                  , "SQL_DESC_CONCISE_TYPE     ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_DATA_PTR                      , "SQL_DESC_DATA_PTR         ",
      O3TYPE_SQLPOINTER },
    { SQL_DESC_DATETIME_INTERVAL_CODE   ,
      "SQL_DESC_DATETIME_INTERVAL_CODE",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_DATETIME_INTERVAL_PRECISION   ,
      "SQL_DESC_DATETIME_INTERVAL_PRECISION",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_DISPLAY_SIZE                  , "SQL_DESC_DISPLAY_SIZE     ",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_FIXED_PREC_SCALE              , "SQL_DESC_FIXED_PREC_SCALE ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_INDICATOR_PTR                 , "SQL_DESC_INDICATOR_PTR    ",
      O3TYPE_SQLPOINTER },
    { SQL_DESC_LABEL                         , "SQL_DESC_LABEL            ",
      O3TYPE_SQLCHARS },
    { SQL_DESC_LENGTH                        , "SQL_DESC_LENGTH           ",
      O3TYPE_SQLUINTEGER },
    { SQL_DESC_LITERAL_PREFIX                , "SQL_DESC_LITERAL_PREFIX   ",
      O3TYPE_SQLCHARS  },
    { SQL_DESC_LITERAL_SUFFIX                , "SQL_DESC_LITERAL_SUFFIX   ",
      O3TYPE_SQLCHARS  },
    { SQL_DESC_LOCAL_TYPE_NAME               , "SQL_DESC_LOCAL_TYPE_NAME  ",
      O3TYPE_SQLCHARS  },
    /*
    { SQL_DESC_MAXIMUM_SCALE                 , "SQL_DESC_MAXIMUM_SCALE    ",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_MINIMUM_SCALE                 , "SQL_DESC_MINIMUM_SCALE    ",
      O3TYPE_SQLINTEGER },
      */
    { SQL_DESC_NAME                          , "SQL_DESC_NAME             ",
      O3TYPE_SQLCHARS },
    { SQL_DESC_NULLABLE                      , "SQL_DESC_NULLABLE         ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_NUM_PREC_RADIX                , "SQL_DESC_NUM_PREC_RADIX   ",
      O3TYPE_SQLINTEGER },
    { SQL_DESC_OCTET_LENGTH                  , "SQL_DESC_OCTET_LENGTH     ",
      O3TYPE_SQLLEN },
    { SQL_DESC_OCTET_LENGTH_PTR              , "SQL_DESC_OCTET_LENGTH_PTR ",
      O3TYPE_SQLPOINTER },
    { SQL_DESC_PARAMETER_TYPE                , "SQL_DESC_PARAMETER_TYPE   ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_PRECISION                     , "SQL_DESC_PRECISION        ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_SCALE                         , "SQL_DESC_SCALE            ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_ROWVER                        , "SQL_DESC_ROWVER           ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_SCHEMA_NAME                   , "SQL_DESC_SCHEMA_NAME      ",
      O3TYPE_SQLCHARS  },
    { SQL_DESC_SEARCHABLE                    , "SQL_DESC_SEARCHABLE       ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_TABLE_NAME                    , "SQL_DESC_TABLE_NAME       ",
      O3TYPE_SQLCHARS  },
    { SQL_DESC_TYPE                          , "SQL_DESC_TYPE             ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_TYPE_NAME                     , "SQL_DESC_TYPE_NAME        ",
      O3TYPE_SQLCHARS  },
    { SQL_DESC_UNNAMED                       , "SQL_DESC_UNNAMED          ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_UNSIGNED                      , "SQL_DESC_UNSIGNED         ",
      O3TYPE_SQLSMALLINT },
    { SQL_DESC_UPDATABLE                     , "SQL_DESC_UPDATABLE        ",
      O3TYPE_SQLSMALLINT }
}; /* descFields */


RETCODE ShowDescAll( SQLHDESC hdesc )
{
    RETCODE retcode = SQL_SUCCESS;
    SWORD   numHeaderFields = sizeof(headerFields) / sizeof(descFieldType);
    SWORD   numDescFields = sizeof(descFields) / sizeof(descFieldType);
    SWORD   numRecords = 0;
    SWORD   i, rec;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    /* show header fields */
    for (i=0; i<numHeaderFields; i++) {
        retcode = ShowDescField( hdesc, &headerFields[i], 0 );
    }; /* for */

    /* Get number of records */
    retcode = SQLGetDescField( hdesc,
                               0,
                               SQL_DESC_COUNT,
                               &numRecords,
                               sizeof( numRecords ),
                               NULL );
    SPRINTF( textBuffer,
             "ShowDesc\t -- GetDescField(COUNT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
    SPRINTF( textBuffer, "%s \t= %d"CRLF,
             "SQL_DESC_COUNT",
             numRecords );
    PRINTF( textBuffer );

    /* show records */
    for (rec=1; rec<=numRecords; rec++) {
        SPRINTF( textBuffer, CRLF"Record-number: %d"CRLF, rec );
        PRINTF( textBuffer );
        for (i=0; i<numDescFields; i++) {
            retcode = ShowDescField( hdesc, &descFields[i], rec );
        }; /* for */
    }; /* for */

    return retcode;
}; /* ShowDescAll */


RETCODE ShowDescField( SQLHDESC hdesc, descFieldType *field, SWORD recNo )
{
    RETCODE retcode = SQL_SUCCESS;
    SQLLEN         lenValue = 0;
    SQLINTEGER     intValue = 0;
    SQLSMALLINT    smallintValue = 0;
    SQLUSMALLINT   usmallintValue = 0;
    SQLUINTEGER    uintValue = 0;
    UCHAR   charValue[80];
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SDWORD  stringLength = 0;

    switch ( field->dataType ) {
    case O3TYPE_SQLSMALLINT:
        smallintValue = 0;
        retcode = SQLGetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   &smallintValue,
                                   sizeof( smallintValue ),
                                   &stringLength );
        ErrorCheck( SQL_HANDLE_DESC, hdesc );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "%s \t= %d"CRLF,
                     field->name,
                     smallintValue );
            PRINTF( textBuffer );
        } else {
            SPRINTF( textBuffer, "%s"CRLF, field->name );
            PRINTF( textBuffer );
        };
        break;
    case O3TYPE_SQLINTEGER:
        intValue = 0;
        retcode = SQLGetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   &intValue,
                                   sizeof( intValue ),
                                   &stringLength );
        ErrorCheck( SQL_HANDLE_DESC, hdesc );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "%s \t= %d"CRLF,
                     field->name,
                     intValue );
            PRINTF( textBuffer );
        } else {
            SPRINTF( textBuffer, "%s"CRLF, field->name );
            PRINTF( textBuffer );
        };
        break;
    case O3TYPE_SQLLEN:
        lenValue = 0;
        retcode = SQLGetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   &lenValue,
                                   sizeof( intValue ),
                                   &stringLength );
        ErrorCheck( SQL_HANDLE_DESC, hdesc );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "%s \t= %d"CRLF,
                     field->name,
                     lenValue );
            PRINTF( textBuffer );
        } else {
            SPRINTF( textBuffer, "%s"CRLF, field->name );
            PRINTF( textBuffer );
        };
        break;
    case O3TYPE_SQLUINTEGER:
        uintValue = 0;
        retcode = SQLGetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   &uintValue,
                                   sizeof( uintValue ),
                                   &stringLength );
        ErrorCheck( SQL_HANDLE_DESC, hdesc );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "%s \t= %d"CRLF,
                     field->name,
                     uintValue );
            PRINTF( textBuffer );
        } else {
            SPRINTF( textBuffer, "%s"CRLF, field->name );
            PRINTF( textBuffer );
        };
        break;
    case O3TYPE_SQLPOINTER: {
	SQLLEN   uintValue = 0;
        retcode = SQLGetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   &uintValue,
                                   sizeof( uintValue ),
                                   &stringLength );
        ErrorCheck( SQL_HANDLE_DESC, hdesc );
        if (retcode == SQL_SUCCESS) {
            SPRINTF( textBuffer, "%s \t= %s"CRLF,
                     field->name,
                     (uintValue==0) ? "==NULL" : "!=NULL" );
            PRINTF( textBuffer );
        } else {
            SPRINTF( textBuffer, "%s"CRLF, field->name );
            PRINTF( textBuffer );
        };
        break;
	}
    case O3TYPE_SQLCHARS:
        charValue[0] = '\0';
        retcode = SQLGetDescField( hdesc,
                                   recNo,
                                   field->fieldType,
                                   charValue,
                                   sizeof( charValue ),
                                   &stringLength );
        if (stringLength == 0)
          charValue[0] = '\0';  /* bug in driver manager: copies back old */
                                /* buffer even stringLength is 0!         */

        ErrorCheck( SQL_HANDLE_DESC, hdesc );
        if (retcode == SQL_SUCCESS) {
            if (charValue!=NULL) {
                if (stringLength == 0)    /* charValue contains sometimes */
                    charValue[0] = '\0';  /* garbage from driver manager */
                SPRINTF( textBuffer, "%s \t= %s (len=%d)"CRLF,
                         field->name,
                         charValue,
                         stringLength );
                PRINTF( textBuffer );
            } else {
                PRINTF( "<NULL>" );
            };
        } else {
            SPRINTF( textBuffer, "%s"CRLF, field->name );
            PRINTF( textBuffer );
        };
        break;
    default:
        break;
    }; /* switch */

    return retcode;
}; /* ShowDescField */
