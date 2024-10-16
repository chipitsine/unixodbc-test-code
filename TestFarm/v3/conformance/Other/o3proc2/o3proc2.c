/*!**********************************************************************

  module: o3proc.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description:  Create and execute DB-Procedure with and without
                result (PTS 1103839).

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

#define PROGNAME "o3proc2"

#define ODBCVER 0x0350

#include "o3util.c"

#define CREATE_PROC_STMT \
"create dbproc ins_t1 (in first int, out s1 int, out s2 int) as \
try \
  if first > 0 \
  then \
     select res(*) from ODBC.t1; \
  fetch res into :s1, :s2; \
catch \
  stop ($rc);"

int main(int argc, argv_t argv) {
    RETCODE retcode;
    HDBC    hdbc  = SQL_NULL_HDBC;
    HENV    henv  = SQL_NULL_HENV;
    HSTMT   hstmt = SQL_NULL_HSTMT;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SWORD   intVal;
    char    byteVal;

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    retcode = CreateTable( hdbc, "ODBC.T1", "(s1 int, s2 int)" );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocHandle(STMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLExecDirect( hstmt, "insert ODBC.t1 values (1,2)", SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect(INSERT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLExecDirect( hstmt, CREATE_PROC_STMT, SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect(CREATE PROC) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = SQLExecDirect( hstmt, "call ODBC.ins_t1 (0, ?, ?)", SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect(CALL) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecDirect( hstmt, "call ODBC.ins_t1 (1, ?, ?)", SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect(CALL) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

    retcode = SQLExecDirect( hstmt, "call ODBC.ins_t1 (0, ?, ?)", SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect(CALL) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );

exit:
    retcode = SQLExecDirect( hstmt, "DROP DBPROC INS_T1", SQL_NTS );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLExecDirect(DROP PROC) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    retcode = SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLFreeHandle(HSTMT) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DBC, hdbc );

    retcode = DropTable( hdbc, "t1" );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
    return 0;
} /* main */
