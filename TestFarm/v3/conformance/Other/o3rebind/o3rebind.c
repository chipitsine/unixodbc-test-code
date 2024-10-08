/*!**********************************************************************

  module: o3rebind.c

  -----------------------------------------------------------------------

  responsible:  TTK

  special area: ODBC-Tests

  description:  (re-) bind after SQLFreeStmt (SQL_UNBIND)  cmp PTS 1116229

  -----------------------------------------------------------------------


\if EMIT_LICENCE


    ========== licence begin  GPL
    Copyright (c) 2002-2004 SAP AG

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

#define PROGNAME "O3REBIND"
#define TABLE    PROGNAME

SQLRETURN ExecuteBindFetch (SQLHSTMT);
UCHAR FAR *cnvret(RETCODE retcode);

int main(int argc, argv_t argv) {
    RETCODE retcode;
    SQLHENV    henv  = SQL_NULL_HENV;
    SQLHDBC    hdbc  = SQL_NULL_HDBC;
    SQLHSTMT   hstmt = SQL_NULL_HDBC;

    ProcessCmdArguments(argc,argv);

    retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));
    retcode = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION,
                             (void*) SQL_OV_ODBC3, 0 );
    printf("SQLSetEnvAttr retcode: %s"CRLF, cnvret( retcode ));
    retcode = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));

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

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    printf("SQLAllocHandle retcode: %s"CRLF, cnvret( retcode ));

    /* create and fill table */
    retcode = SQLExecDirect (hstmt, "create table "TABLE" (name char(20))", SQL_NTS);
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecDirect (hstmt, "insert into o3rebind values ('Opteron')", SQL_NTS);
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    retcode = SQLExecDirect (hstmt, "insert into o3rebind values ('x86-64')", SQL_NTS);
    printf("SQLExecDirect retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );


    retcode = SQLSetStmtAttr (hstmt, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) 2, SQL_IS_UINTEGER);
    /*    retcode = SQLSetStmtOption (hstmt, SQL_ROWSET_SIZE, 2); */
    printf("SQLSetStmtAttr retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLPrepare (hstmt, "select name from "TABLE, SQL_NTS);
    printf("SQLPrepare retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );


    retcode = ExecuteBindFetch (hstmt);

    retcode = SQLFreeStmt (hstmt, SQL_UNBIND);
    printf("SQLFreeStmt retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
    printf("SQLFreeStmt retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = ExecuteBindFetch (hstmt);

    if (retcode == SQL_SUCCESS)
      printf ("Test OK\n");
    else
      printf ("Test Failure\n");

    return 0;
} /* main */


SQLRETURN ExecuteBindFetch (SQLHSTMT hstmt)
{
    UCHAR   string[3][20];
    RETCODE retcode;
    SQLLEN  length[2];
    SQLULEN rowCount;
    SQLUSMALLINT rowStatus[2];

    memset (string, '\0', 40);
    length[0] = length[1] = 1;

    retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, &(string[0]), sizeof(string[0]), &length[0]);
    printf("SQLBindCol retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecute (hstmt);
    printf("SQLExecute retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExtendedFetch (hstmt, SQL_FETCH_NEXT, 0, &rowCount, rowStatus);
    printf("SQLFetch retcode: %s"CRLF, cnvret( retcode ));
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    printf("Fetched: %s\t%d\n", string[0], (int) length[0]);
    printf("Fetched: %s\t%d\n", string[1], (int) length[1]);

    return retcode;
} /* CreateDbProc */

