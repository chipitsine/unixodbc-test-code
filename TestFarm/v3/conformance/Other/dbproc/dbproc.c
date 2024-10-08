/*!**********************************************************************

  module: dbproc.c

  -----------------------------------------------------------------------

  responsible:  BurkhardD

  special area: ODBC-Tests

  description:

  -----------------------------------------------------------------------


\if EMIT_LICENCE


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

\endif
************************************************************************/

#include "tst.h"

static SQLRETURN ProcTest1( HDBC hdbc );
static SQLRETURN ProcTest2( HDBC hdbc );
static SQLRETURN CreateDbProc( HDBC hdbc );
static SQLRETURN DropDbProc( HDBC hdbc );

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HDBC    hdbc  = SQL_NULL_HDBC;

    ProcessCmdArguments(argc,argv);


    retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    retcode = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION,
                             (void*) SQL_OV_ODBC3, 0 );
    printf("SQLSetEnvAttr retcode: %s"CRLF, cnvret( retcode ));
    retcode = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));


    retcode = SQLSetConnectAttr( hdbc, 1001 /* PASSTHROUGH */,
                                 (SQLPOINTER) 1, 0 );
    printf("SQLSetConnectAttr retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    retcode = SQLConnect(hdbc,
                         DBNODE,
                         (SWORD) SQL_NTS,
                         ODBCUSER,
                         (SWORD) SQL_NTS,
                         ODBCPASSW,
                         (SWORD) SQL_NTS );
    printf(" SQLConnect retcode: %d"CRLF, retcode);
    retcode = SQL_SUCCESS;
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    CreateDbProc( hdbc );

    retcode = ProcTest1( hdbc );

    if (retcode == SQL_SUCCESS)
      retcode = ProcTest2( hdbc );

    DropDbProc( hdbc );

    if (retcode == SQL_SUCCESS)
      printf ("Test OK\n");
    else
      printf ("Test Failure\n");

    return 0;
}; /* main */

static SQLRETURN CreateDbProc( HDBC hdbc )
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   stmtStr[500];

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    sprintf( stmtStr, "create dbproc test_proc (IN a int, INOUT b int, OUT s int) AS \
                       set s = a+b; set b=4;" );

    retcode = SQLExecDirect( hstmt, stmtStr, SQL_NTS );
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    sprintf( stmtStr, "CREATE DBPROC test_proc_rs RETURNS CURSOR AS SET $CURSOR = 'abc';\
                       DECLARE :$CURSOR CURSOR FOR SELECT '4165' FROM DOMAIN.USERS;");

    retcode = SQLExecDirect( hstmt, stmtStr, SQL_NTS );
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    return retcode;
}; /* CreateDbProc */

static SQLRETURN DropDbProc( HDBC hdbc )
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   stmtStr[500];

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    sprintf( stmtStr, "DROP DBPROC test_proc" );
    retcode = SQLExecDirect( hstmt, stmtStr, SQL_NTS );
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    sprintf( stmtStr, "DROP DBPROC test_proc_rs" );
    retcode = SQLExecDirect( hstmt, stmtStr, SQL_NTS );
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    printf("SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    return retcode;
}; /* DropDbProc */


static SQLRETURN ProcTest1( HDBC hdbc )
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;

    SWORD   type, digits, nullable;
    UDWORD  size;
    SQLLEN  len = 0;
    SQLINTEGER a, b, s;
    UCHAR   stmtStr[500];
    SQLSMALLINT cols;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    sprintf( stmtStr, "{ CALL test_proc (?,?,?) }" );
    retcode = SQLPrepare( hstmt, stmtStr, SQL_NTS );
    printf("SQLPrepare retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindParameter( hstmt, 1, SQL_PARAM_INPUT,
                                SQL_C_LONG, SQL_INTEGER, 72, 0,
                                &a, sizeof(a), &len );
    printf("SQLBindParameter retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindParameter( hstmt, 2, SQL_PARAM_INPUT_OUTPUT,
                                SQL_C_LONG, SQL_INTEGER, 72, 0,
                                &b, sizeof(b), &len );
    printf("SQLBindParameter retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLBindParameter( hstmt, 3, SQL_PARAM_OUTPUT,
                                SQL_C_LONG, SQL_INTEGER, 72, 0,
                                &s, sizeof(s), &len );
    printf("SQLBindParameter retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    a = 2;
    b = 3;
    s = 0;
    retcode = SQLExecute( hstmt );
    printf("SQLExecute retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLNumResultCols (hstmt, &cols );
    printf("SQLNumResultCols: %d retcode: %s"CRLF, cols, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );


    printf("a = %d   b = %d   s = %d\n", a, b, s);

    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    printf("SQLFreeHandle retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    if (a == 2 && b == 4 && s == 5)
      retcode = SQL_SUCCESS;
    else
      retcode = SQL_ERROR;

    return retcode;
}; /* ProcTest1 */

static SQLRETURN ProcTest2( HDBC hdbc )
{
    RETCODE retcode;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UDWORD  size;
    SQLLEN  len;
    UCHAR   stmtStr[500];
    SQLCHAR    data[50];
    SQLSMALLINT cols;

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    sprintf( stmtStr, "CALL test_proc_rs" );
    retcode = SQLPrepare( hstmt, stmtStr, SQL_NTS );
    printf("SQLPrepare retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecute( hstmt );
    printf("SQLExecute retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLNumResultCols (hstmt, &cols );
    printf("SQLNumResultCols: %d retcode: %s"CRLF, cols, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFetch( hstmt );
    printf("SQLFetch retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, data, sizeof(data), &len);
    printf("SQLGetData: <%s>(%d) retcode: %s"CRLF, data, len, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );


    if (strcmp ("4165", data) == 0 && len == 4)
      retcode = SQL_SUCCESS;
    else
      retcode = SQL_ERROR;

    return retcode;
}; /* ProcTes2t */
