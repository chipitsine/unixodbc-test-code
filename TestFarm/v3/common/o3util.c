/*



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

char *datasourcename;
char *odbcuser;
char *odbcpassw;

void ProcessCmdArguments(int argc, argv_t argv)
{
  if (argc>1)
    datasourcename=argv[1];
  else
    datasourcename="";

  if (argc>2)
    odbcuser=argv[2];
  else
    odbcuser="";

  if (argc>3)
    odbcpassw=argv[3];
  else
    odbcpassw="";

}

void StartTest( char *testName )
{
    PRINTF( "Test " );
    PRINTF( testName );
    PRINTF( " started ..."CRLF );
} /* StartTest */


void EndTest( char *testName )
{
    PRINTF( "Test " );
    PRINTF( testName );
    PRINTF( " ended."CRLF );
} /* StartTest */


void ErrorCheck( SWORD handleType, SQLHANDLE handle )
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
                 "ErrorCheck\t -- SQLGetDiagField(DIAG_NUMBER) retcode: %s"CRLF,
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
                             "ErrorCheck\t-- SQLGetDiagRec retcode: %s"CRLF,
                             cnvret(retcode));
                    PRINTF( textBuffer );
                }; /* if */
                if ( retcode == SQL_SUCCESS
                     || retcode == SQL_SUCCESS_WITH_INFO) {
                    SPRINTF( (char*) textBuffer,
                             "ErrorCheck\t -- recNumber: %d  sqlstate: %s  native: %d"CRLF,
                             recNumber, sqlstate, native );
                    PRINTF( textBuffer );
                    SPRINTF( (char*) textBuffer, "messageText: %s"CRLF,
                             messageText );
                    PRINTF( textBuffer );
                }; /* if */
            }; /* for */
        }; /* if */
    }; /* if */
} /* ErrorCheck */


RETCODE FullConnect( HENV *phenv, HDBC *phdbc )
{
    RETCODE retcode;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv );
    SPRINTF( textBuffer,
             "FullConnect\t -- AllocHandle (ENV) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetEnvAttr( *phenv, SQL_ATTR_ODBC_VERSION,
                             (void*) SQL_OV_ODBC3, 0 );
    SPRINTF( textBuffer,
             "FullConnect\t -- SetEnvAttr (ODBC_VERSION) \t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_DBC, *phenv, phdbc );
    SPRINTF( textBuffer,
             "FullConnect\t -- AllocHandle (DBC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLConnect( *phdbc,
                          (UCHAR*)DBNODE, (SWORD) SQL_NTS,
                          (UCHAR*)ODBCUSER, (SWORD) SQL_NTS,
                          (UCHAR*)ODBCPASSW, (SWORD) SQL_NTS );
    SPRINTF( textBuffer, "FullConnect \t -- Connect \t\t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DBC, *phdbc );

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        exit(4);

    return retcode;
} /* FullConnect */


void FullDisconnect(  HENV *phenv, HDBC *phdbc )
{
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    RETCODE retcode;

    retcode = SQLDisconnect( *phdbc );
    SPRINTF( textBuffer,
             "FullDisconnect\t -- Disconnect \t\t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DBC, *phdbc );

    retcode = SQLFreeHandle( SQL_HANDLE_DBC, *phdbc );
    SPRINTF( textBuffer,
             "FullDisconnect\t -- FreeHandle (hdbc) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode == SQL_ERROR) {
        ErrorCheck( SQL_HANDLE_DBC, *phdbc );
    }; /* if */

    retcode = SQLFreeHandle( SQL_HANDLE_ENV, *phenv );
    SPRINTF( textBuffer,
             "FullDisconnect\t -- FreeHandle (henv) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode == SQL_ERROR) {
        ErrorCheck( SQL_HANDLE_ENV, *phenv );
    }; /* if */
} /* FullDisconnect */


RETCODE CreateTable( HDBC hdbc, UCHAR *tableName, UCHAR *tableDef )
{
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "CreateTable\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    SPRINTF( (char*) stmtStr, "DROP TABLE %s", tableName );
    SPRINTF( textBuffer, "CreateTable\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );
    retcode = SQLExecDirect( hstmt, stmtStr, STRLEN(stmtStr) );
    SPRINTF( textBuffer,
             "CreateTable\t -- ExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    SPRINTF( stmtStr, "create table %s %s", tableName, tableDef );
    SPRINTF( textBuffer, "CreateTable\t -- %s"CRLF, stmtStr);
    PRINTF( textBuffer );
    retcode = SQLExecDirect( hstmt, stmtStr, STRLEN(stmtStr) );
    SPRINTF( textBuffer,
             "CreateTable\t -- ExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "CreateTable\t -- SQLFreeHandle(STMT) \t retcode: %s"CRLF,
             cnvret(retcode));
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
    }; /* if */

    return retcode;
} /* CreateTable */


RETCODE DropTable( HDBC hdbc, UCHAR *tableName ) {
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "DropTable\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    SPRINTF( (char*) stmtStr, "DROP TABLE %s", tableName );
    SPRINTF( textBuffer, "DropTable\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );
    retcode = SQLExecDirect( hstmt, stmtStr, STRLEN(stmtStr) );
    SPRINTF( textBuffer,
             "DropTable\t -- ExecDirect \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "DropTable\t -- SQLFreeHandle(STMT) \t retcode: %s"CRLF,
             cnvret(retcode));
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        ErrorCheck( SQL_HANDLE_STMT, hstmt );
    }; /* if */

    return retcode;
} /* DropTable */


void ShowDesc( SQLHDESC hdesc )
{
    RETCODE retcode;
    SWORD   allocType;
    SWORD   smallInt;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    retcode = SQLGetDescField( hdesc, 0, SQL_DESC_ALLOC_TYPE, &allocType,
                               0, NULL );
    SPRINTF( textBuffer,
             "ShowDesc\t -- GetDescField(ALLOC_TYPE) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DESC, hdesc );
    if (retcode != SQL_SUCCESS) {
        return;
    }; /* if */
    switch (allocType) {
    case SQL_DESC_ALLOC_AUTO:
        PRINTF( "\t SQL_DESC_ALLLOC_TYPE = SQL_DESC_ALLOC_AUTO"CRLF );
        break;
    case SQL_DESC_ALLOC_USER:
        PRINTF( "\t SQL_DESC_ALLLOC_TYPE = SQL_DESC_ALLOC_USER"CRLF );
        break;
    default:
        PRINTF( "\t SQL_DESC_ALLLOC_TYPE = unknown !!!"CRLF );
        break;
    }; /* switch */
} /* ShowDesc */


void ShowLongChar( char *longChar )
{
    char    textBuffer[80];
    UDWORD  len;

    len = STRLEN( longChar );
    if (len < sizeof(textBuffer))
	   SPRINTF( textBuffer, "'%s'"CRLF, longChar );
	else
	   SPRINTF( textBuffer, "'%.*s"CRLF"........"CRLF"%s'"CRLF,
                sizeof(textBuffer)/3,
                longChar,
                ((char *)longChar+len- sizeof(textBuffer)/3));
	PRINTF( textBuffer );
} /* showLongChar */


UCHAR FAR *cnvFetchType(UWORD fFetchType)
{
    static UCHAR str[20];
    switch(fFetchType) {
        WHCASESTRING(str, SQL_FETCH_NEXT);
        WHCASESTRING(str, SQL_FETCH_FIRST);
        WHCASESTRING(str, SQL_FETCH_LAST);
        WHCASESTRING(str, SQL_FETCH_PRIOR);
        WHCASESTRING(str, SQL_FETCH_ABSOLUTE);
        WHCASESTRING(str, SQL_FETCH_RELATIVE);
        /*WHCASESTRING(str, SQL_FETCH_RESUME); */
        WHCASESTRING(str, SQL_FETCH_BOOKMARK);
    default: {
            SPRINTF(str, "SQL_FETCH_(%d)", fFetchType);
            break;
        }
    }
   return (str);
} /* cnvFetchType */


UCHAR  *cnvcurs(DWORD fSqlCursorType)
{
   static UCHAR str[30];
   switch(fSqlCursorType) {
      WHCASESTRING(str, SQL_CURSOR_KEYSET_DRIVEN);
      WHCASESTRING(str, SQL_CURSOR_DYNAMIC);
      WHCASESTRING(str, SQL_CURSOR_STATIC);
      WHCASESTRING(str, SQL_CURSOR_FORWARD_ONLY);
      WHCASESTRING(str, SQL_ROW_ADDED);
      default: {
	 STRCPY(str, "## INVALID ##");
	 break;
      }
   }
   return (str);
} /* cnvcurs */


UCHAR* CTypeToString( SWORD cType )
{
    static UCHAR str[30];
    switch (cType) {
        WHCASESTRING(str, SQL_C_CHAR   );
        WHCASESTRING(str, SQL_C_WCHAR   );
        WHCASESTRING(str, SQL_C_LONG   );
        WHCASESTRING(str, SQL_C_SHORT  );
        WHCASESTRING(str, SQL_C_FLOAT  );
        WHCASESTRING(str, SQL_C_DOUBLE );
        WHCASESTRING(str, SQL_C_NUMERIC);
        WHCASESTRING(str, SQL_C_DEFAULT);
        WHCASESTRING(str, SQL_C_DATE);
        WHCASESTRING(str, SQL_C_TIME);
        WHCASESTRING(str, SQL_C_TIMESTAMP  );
        WHCASESTRING(str, SQL_C_TYPE_DATE     );
        WHCASESTRING(str, SQL_C_TYPE_TIME     );
        WHCASESTRING(str, SQL_C_TYPE_TIMESTAMP);
        WHCASESTRING(str, SQL_C_BINARY  );
        WHCASESTRING(str, SQL_C_BIT     );
        WHCASESTRING(str, SQL_C_SBIGINT);
        WHCASESTRING(str, SQL_C_UBIGINT);
        WHCASESTRING(str, SQL_C_TINYINT );
        WHCASESTRING(str, SQL_C_SLONG   );
        WHCASESTRING(str, SQL_C_SSHORT  );
        WHCASESTRING(str, SQL_C_STINYINT);
        WHCASESTRING(str, SQL_C_ULONG  );
        WHCASESTRING(str, SQL_C_USHORT );
        WHCASESTRING(str, SQL_C_UTINYINT);
        WHCASESTRING(str, SQL_C_GUID);
    default:
        STRCPY( str, "## Unknown ##" );
        break;
    }; /* switch */

    return (str);
} /* CTypeToString */


UCHAR* SqlTypeToString( SWORD sqlType )
{
    static UCHAR str[30];
    switch (sqlType) {
        WHCASESTRING(str, SQL_UNKNOWN_TYPE  );
        WHCASESTRING(str, SQL_CHAR       );
        WHCASESTRING(str, SQL_WCHAR      );
        WHCASESTRING(str, SQL_NUMERIC    );
        WHCASESTRING(str, SQL_DECIMAL    );
        WHCASESTRING(str, SQL_INTEGER    );
        WHCASESTRING(str, SQL_SMALLINT   );
        WHCASESTRING(str, SQL_FLOAT      );
        WHCASESTRING(str, SQL_REAL       );
        WHCASESTRING(str, SQL_DOUBLE     );
        WHCASESTRING(str, SQL_DATETIME   );
        WHCASESTRING(str, SQL_VARCHAR    );
        WHCASESTRING(str, SQL_WVARCHAR   );
        WHCASESTRING(str, SQL_TYPE_DATE  );
        WHCASESTRING(str, SQL_TYPE_TIME  );
        WHCASESTRING(str, SQL_TYPE_TIMESTAMP);
        WHCASESTRING(str, SQL_INTERVAL   );
        WHCASESTRING(str, SQL_TIMESTAMP  );
        WHCASESTRING(str, SQL_LONGVARCHAR);
        WHCASESTRING(str, SQL_WLONGVARCHAR);
        WHCASESTRING(str, SQL_BINARY     );
        WHCASESTRING(str, SQL_VARBINARY  );
        WHCASESTRING(str, SQL_LONGVARBINARY);
        WHCASESTRING(str, SQL_BIGINT     );
        WHCASESTRING(str, SQL_TINYINT    );
        WHCASESTRING(str, SQL_BIT        );
        WHCASESTRING(str, SQL_GUID       );
    default:
        STRCPY( str, "## Unknown ##" );
        break;
    }; /* switch */

    return (str);
} /* SqlTypeToString */

void InitLongBuf( char *buf, UDWORD len )
{
    UDWORD pos = 0;
    UDWORD strLen = 0;
    UCHAR  textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    buf[0] = '\0';
    while (pos < len) {
        SPRINTF( textBuffer, "|<-%d ", pos+1 );
        strLen = STRLEN( textBuffer );
        if ( pos+strLen < len) {
            STRCAT( buf, textBuffer );
        }; /* if */
        pos += strLen;
    }; /* while */
} /* InitLongBuf */
