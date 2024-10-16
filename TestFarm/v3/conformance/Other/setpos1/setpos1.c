/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : D025086
**
** Program      : setpos1.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLBulkOperations
**
** Date         : 23.01.96
** l.aenderung  : 13:13pm 01-08-1997 (Harun)
******************************************************************************



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



*/
#include "tst.h"

/* ---------------------------------------------------------------------- */
#define PROGNAME "setpos"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "prec:Z"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ODBCUSER1  "ODBC1"
#define ODBCPASSW1 "ODBC1"
#define ODBCTABLE  "ODBCTEST"
#define ODBCSCRIPT "proc2inst"
#define DBPROC1	   "INSERT_TEST"
#define DBPROC2	   "SELECT_TEST"

#define ERROR_CHECK(hstmtx)  \
    if (rc != SQL_SUCCESS) {\
	   SQLError(henv, hdbc, hstmtx, sqlstate, &esq_sql_code, error_txt,\
		    len_error_txt, &used_error_txt);\
	   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
           PRINTF(buffer);\
	   SPRINTF(buffer," esq_sql_code: %d"CRLF, esq_sql_code);\
           PRINTF(buffer);\
	   SPRINTF(buffer," error_txt: %s"CRLF, error_txt);\
           PRINTF(buffer);\
           switch (rc) {\
		  case(1) :\
			  break;\
		  case(-1) :\
			  break;\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16); \
			  break; } \
	}

 UCHAR sqlstate  [ 10 ];
 SDWORD esq_sql_code;
 UCHAR error_txt  [ 101 ] ;
 SWORD len_error_txt = 100;
 SWORD used_error_txt;
 UCHAR buffer[1024];
/* ----------------------------------------------------------------------- */

#define TESTNAME "SQLSetPos"
#define TABLE "SETPOS"

#define MAX_NAME_LEN	512
#define MAX_STR_LEN	60
#define MAX_ROWS	20
#define MAX_CURSORNAME  18

#define ROW_SIZE	8
#define SETPOS0		0
#define SETPOS1		2
#define SETPOS2		4
#define SETPOS3		6
#define SETPOS4		8
#define SETPOS5		5
#define SETPOS6		7
#define SETPOSC		3

#define TESTUPD1	99
#define TESTUPD2	"SQLSetPos (UPDATED) %d"

#define TESTADD1	88
#define TESTADD2	"SQLSetPos (ADD) %d"

#define TXTCOPY "SQLSetPos String "

/* ------------------------------------------------------------------------ */
RETCODE DoPrintArrayPos(UCHAR fType[MAX_NAME_LEN], UDWORD crow, UDWORD rowpos,
		     UWORD rgfRowStatus[ROW_SIZE], SWORD sNumber_rc[ROW_SIZE],
		     UCHAR szString_rc[ROW_SIZE][MAX_STR_LEN]);
/*
 -------------------------------------------------------------------------
| DoSQLSetPos:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 -------------------------------------------------------------------------
*/
int main(int argc, argv_t argv)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	HENV henv   = SQL_NULL_HENV;
	HENV FAR * phenv;
	HDBC hdbc   = SQL_NULL_HDBC;
	HDBC FAR * phdbc;
	HSTMT hstmt = SQL_NULL_HSTMT;
	HSTMT hstmt1 = SQL_NULL_HSTMT;
	HSTMT FAR * phstmt;

	HSTMT 		        hstmt_cur = SQL_NULL_HSTMT;
/* ---------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   delete[MAX_NAME_LEN+1];
	UCHAR                   buf[MAX_NAME_LEN+1];

	UCHAR                   szCursorName[MAX_CURSORNAME+1];
	SWORD			pcbCursor;
	SWORD			cbCursorMax;

	UCHAR                   szString[MAX_STR_LEN];
	SWORD			sNumber;
	SDWORD			cbNumber=SQL_NTS;
	SDWORD			cbString=SQL_NTS;

	UCHAR			szString_rc[ROW_SIZE+1][MAX_STR_LEN];
	SWORD			sNumber_rc[ROW_SIZE+1];
	SQLLEN			cbNumber_rc[ROW_SIZE+1]={SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLLEN			cbString_rc[ROW_SIZE+1]={SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};

	UWORD			rgfRowStatus[ROW_SIZE];

	UDWORD			irow, crow;
	UDWORD			cbRowCount, cbRowsFetched, cbFetchBookMark, cbBindOffset;
	SWORD			cAbort=0;
        int i;
/* ---------------------------------------------------------------------har- */

 ProcessCmdArguments(argc,argv);

 SPRINTF(buffer, " %s running.\n", PROGNAME);
 PRINTF(buffer);

 phenv = (HENV FAR *) &henv;
 phdbc = (HDBC FAR *) &hdbc;
 phstmt = (HSTMT FAR *) &hstmt;

 rc = SQLAllocEnv(phenv);
 SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0 );
 SPRINTF(buffer," SQLSetEnvAttr retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocConnect(henv, phdbc);
 SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLConnect(hdbc, DBNODE, SQL_NTS, ODBCUSER, SQL_NTS, ODBCPASSW, SQL_NTS );
 SPRINTF(buffer," SQLConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocStmt (hdbc, phstmt);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(hstmt);

 rc = SQLAllocStmt (hdbc, &hstmt1);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(hstmt1);

 STRCPY(create,"DROP TABLE OPENWIKI_PAGES");
 PRINTF(create);

 rc = SQLExecDirect(hstmt, create, SQL_NTS);
 SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(hstmt);

 STRCPY(create,"CREATE TABLE openwiki_pages ( wpg_name VARCHAR (128), wpg_lastmajor INTEGER, wpg_lastminor INTEGER, wpg_changes INTEGER)");
 PRINTF(create);

 rc = SQLExecDirect(hstmt, create, SQL_NTS);
 SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(hstmt);

 /* --- Select Table --------------------------------------------- */

 rc = SQLAllocStmt(hdbc, &hstmt);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(hstmt);

 rc = SQLSetStmtAttr(hstmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER) 60, SQL_IS_INTEGER);
 SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_QUERY_TIMEOUT (%ld) retcode: '%s'"CRLF, 60, cnvret(rc));
 PRINTF( buffer );

 rc = SQLSetStmtAttr(hstmt, SQL_ATTR_USE_BOOKMARKS, (SQLPOINTER) SQL_UB_ON, SQL_IS_INTEGER);
 SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_USE_BOOKMARKS (%s=%ld) retcode: '%s'"CRLF, "SQL_UB_ON", 1, cnvret(rc));
 PRINTF( buffer );

 rc = SQLSetStmtAttr(hstmt, SQL_ATTR_CONCURRENCY, (SQLPOINTER) SQL_CONCUR_ROWVER, SQL_IS_INTEGER);
 SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_CONCURRENCY (%s=%ld) retcode: '%s'"CRLF, "SQL_CONCUR_ROWVER", SQL_CONCUR_ROWVER, cnvret(rc));
 PRINTF( buffer );

 rc = SQLSetStmtAttr(hstmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_INTEGER);
 SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_CURSOR_TYPE (%s=%ld) retcode: '%s'"CRLF, "SQL_CURSOR_KEYSET_DRIVEN", SQL_CURSOR_KEYSET_DRIVEN, cnvret(rc));
 PRINTF( buffer );

 STRCPY(select,"select * from openwiki_pages");
 SPRINTF(buffer,"Select Stmt= '%s'"CRLF,select);
 PRINTF(buffer);

 rc = SQLExecDirect(hstmt, select, SQL_NTS);
 SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(hstmt);

 for (i=0;i<ROW_SIZE;i++){
   sNumber_rc[i]=i;
   sprintf(szString_rc[i], TESTUPD2, sNumber_rc[i]);
   cbString_rc[i]=STRLEN(szString_rc[i]);
 }

 if (rc == SQL_SUCCESS) {
   rc = SQLBindCol(hstmt, 1, SQL_C_CHAR, szString_rc, MAX_STR_LEN, cbString_rc);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   rc = SQLBindCol(hstmt, 2, SQL_C_SSHORT, &sNumber_rc, sizeof(SWORD), cbNumber_rc);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   rc = SQLBindCol(hstmt, 3, SQL_C_SSHORT, &sNumber_rc, sizeof(SWORD), cbNumber_rc);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   rc = SQLBindCol(hstmt, 4, SQL_C_SSHORT, &sNumber_rc, sizeof(SWORD), cbNumber_rc);
   SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);

   rc = SQLSetStmtAttr(hstmt, SQL_ROWSET_SIZE, (SQLPOINTER) 1, SQL_IS_INTEGER);
   SPRINTF( buffer, "SQLSetStmtAttr SQL_ROWSET_SIZE (%ld) retcode: '%s'"CRLF, 2, cnvret(rc));
   PRINTF( buffer );

   rc = SQLSetStmtAttr(hstmt, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) 2, SQL_IS_INTEGER);
   SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_ROW_ARRAY_SIZE (%ld) retcode: '%s'"CRLF, 2, cnvret(rc));
   PRINTF( buffer );

   rc = SQLSetStmtAttr(hstmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER) 178, SQL_IS_INTEGER);
   SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_ROW_BIND_TYPE (%ld) retcode: '%s'"CRLF, 178, cnvret(rc));
   PRINTF( buffer );

   rc = SQLSetStmtAttr(hstmt, SQL_ATTR_ROW_BIND_OFFSET_PTR, (SQLPOINTER) 0, SQL_IS_INTEGER);
   SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_ROW_BIND_OFFSET_PTR (%ld) retcode: '%s'"CRLF, 0, cnvret(rc));
   PRINTF( buffer );

   rc = SQLSetStmtAttr(hstmt, SQL_ATTR_RETRIEVE_DATA, (SQLPOINTER) 1, SQL_IS_INTEGER);
   SPRINTF( buffer, "SQLSetStmtAttr SQL_ATTR_RETRIEVE_DATA (%ld) retcode: '%s'"CRLF, 1, cnvret(rc));
   PRINTF( buffer );

   rc = SQLFetchScroll( hstmt, SQL_FETCH_ABSOLUTE, 1 );
   SPRINTF( buffer, "SQLFetchScroll SQL_FETCH_ABSOLUTE (%ld) retcode: '%s'"CRLF, 1, cnvret(rc));
   PRINTF( buffer );

/*--------*/
   rc = SQLBulkOperations (hstmt, SQL_ADD);
   SPRINTF( buffer, "SQLBulkOperations SQL_ADD retcode: '%s'"CRLF, cnvret(rc));
   PRINTF( buffer );
   SQLError(NULL, NULL, hstmt, sqlstate, &esq_sql_code, error_txt,
            len_error_txt, &used_error_txt);

 }

 return 0;
}
