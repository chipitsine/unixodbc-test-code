/*!**********************************************************************

  module: codepage.c

  -----------------------------------------------------------------------

  responsible:  TTK (D024584)

  special area: ODBC-Tests

  description:  set/get of codepage (SQLSetConnectAttr (SQL_ATTR_CODEPAGE))

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

#define PROGNAME "codepage"
#define TABLE PROGNAME

#define SQL_ATTR_CODEPAGE 1013

#define CODEPAGE "ISO-8859-8"

#include "tst.h"

int main(int argc, argv_t argv) {
    RETCODE    retcode = SQL_SUCCESS;
    SQLHENV    henv  = SQL_NULL_HENV;
    SQLHDBC    hdbc  = SQL_NULL_HDBC;
    SQLHSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR      textBuffer[O3UTIL_TEXT_BUFFER_LEN];

    SQLCHAR    buffer[50], cp[15];
    SQLINTEGER len;

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );




    /* get default codepage */
    retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );
    SPRINTF (textBuffer, "SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    retcode = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION,
                             (void*) SQL_OV_ODBC2, 0 );

    retcode = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );
    SPRINTF (textBuffer, "SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);

    retcode = SQLConnect(hdbc,
                         DBNODE,
                         (SWORD) SQL_NTS,
                         ODBCUSER,
                         (SWORD) SQL_NTS,
                         ODBCPASSW,
                         (SWORD) SQL_NTS );
    SPRINTF (textBuffer, " SQLConnect retcode: %d"CRLF, retcode);
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );


    retcode = SQLGetConnectAttr( hdbc, SQL_ATTR_CODEPAGE,
                                 (SQLPOINTER) buffer, sizeof(buffer), &len);
    SPRINTF (textBuffer, "SQLGetConnectAttr retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );


    printf ("Codepage: <%s>\n", buffer);


    /*
    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF (textBuffer, "SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    */

    retcode = SQLDisconnect(hdbc);
    SPRINTF (textBuffer, " SQLDisonnect retcode: %d"CRLF, retcode);
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    retcode = SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
    SPRINTF (textBuffer, "SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);

    retcode = SQLFreeHandle( SQL_HANDLE_ENV, henv );
    SPRINTF (textBuffer, "SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);




    /* set codepage exlicitely */
    retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );
    SPRINTF (textBuffer, "SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    retcode = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION,
                             (void*) SQL_OV_ODBC3, 0 );

    retcode = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );
    SPRINTF (textBuffer, "SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);

    strcpy (cp, CODEPAGE);
    retcode = SQLSetConnectAttr( hdbc, SQL_ATTR_CODEPAGE,
                                 (SQLPOINTER) cp, strlen(cp));
    SPRINTF (textBuffer, "SQLSetConnectAttr retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    retcode = SQLConnect(hdbc,
                         DBNODE,
                         (SWORD) SQL_NTS,
                         ODBCUSER,
                         (SWORD) SQL_NTS,
                         ODBCPASSW,
                         (SWORD) SQL_NTS );
    SPRINTF (textBuffer, " SQLConnect retcode: %d"CRLF, retcode);
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );


    retcode = SQLGetConnectAttr( hdbc, SQL_ATTR_CODEPAGE,
                                 (SQLPOINTER) buffer, sizeof(buffer), &len);
    SPRINTF (textBuffer, "SQLGetConnectAttr retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );


    printf ("Codepage: <%s>\n", buffer);


    /*
    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF (textBuffer, "SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);
    */

    retcode = SQLDisconnect(hdbc);
    SPRINTF (textBuffer, " SQLDisonnect retcode: %d"CRLF, retcode);
    PRINTF (textBuffer);
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    retcode = SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
    SPRINTF (textBuffer, "SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);

#ifdef WIN32
    /* following call fails on IA64. Don't know, why. */
    if (sizeof (SQLLEN) != 8)
#endif
    retcode = SQLFreeHandle( SQL_HANDLE_ENV, henv );

    SPRINTF (textBuffer, "SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    PRINTF (textBuffer);


    EndTest( PROGNAME );

    return 0;
}; /* main */

