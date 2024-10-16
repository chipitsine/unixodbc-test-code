/* extlong2.c.OC
 *
 * Holger Bischoff
 *
 * - Aufbau einer Tabelle mit 2 Long-Spalten
 * - insert von Daten, wobei in die 2. Spalte > 32k kommen
 * - Lesen der Daten mit ExtendedFetch (Rowsize=1,Keysize=1,
 *   Cursor-Type=static) & GetData (C-Type=Binary).
 *
 * Dieses Testprogramm diente zur Reproduktion des Fehlers ADIS 1001445



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

#define PROGNAME "EXTLONG2"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "ADMIN"
#define PASSWORD "ADMIN"
#define DBNODE   ""

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define MAXROWS 17

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			      RETCODE r=SQL_SUCCESS;\
				 for(;r!=SQL_NO_DATA_FOUND;){\
			   r = SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
				       if(r!=SQL_NO_DATA_FOUND) {\
			   SPRINTF( buffer, " sqlstate: %s"CRLF, sqlstate);\
			      PRINTF(buffer);\
		  SPRINTF(buffer, "%ld:'%s'"CRLF, esq_sql_code, error_txt);\
		  PRINTF(buffer);\
                  switch (retcode) {\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
		          if (STRCMP(sqlstate,"01000"))\
		          if (STRCMP(sqlstate,"23000"))\
		          if (STRCMP(sqlstate,"01S01"))\
			{ return(16); }\
		     break;\
		  case(100) :\
			  break;\
		  default   :\
		          if (STRCMP(sqlstate,"23000"))\
		          if (STRCMP(sqlstate,"01S01"))\
		       {return(16);}\
			  break; } } } }

UCHAR buffer [1024];

UCHAR szLONG1[68000];
UCHAR szLONG2[68000];
SDWORD cbINTEGER[MAXROWS];
SDWORD cbCharValue[MAXROWS];
SDWORD cbLongValue[MAXROWS];
SDWORD cbINTEGER[MAXROWS];
SDWORD cbIntValue[MAXROWS];

UWORD rgfRowStatus[MAXROWS];

#define FreeStatusArray MEMSET(rgfRowStatus, 0XCA, sizeof(rgfRowStatus));

VOID PrintArrSet(int rowset);
UCHAR FAR *cnvrowret(UWORD status);
UCHAR FAR *cnvFetchType(UWORD fFetchType);
int DoSelect(int rowset, DWORD fSqlCursorType, DWORD fSqlConcur);
/*
int DoSetPos(TestTableStruct *p, UDWORD *row);
*/

HENV henv   = SQL_NULL_HENV;
HDBC hdbc   = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;
RETCODE retcode;
UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ 101 ] ;
SWORD len_error_txt = 100;
SWORD used_error_txt;
UCHAR stmt [ 1024 ];

int main(int argc, argv_t argv)
{
    UDWORD row;
    SQLROWSETSIZE crow;
    SWORD   fSqlType;
    SQLULEN cbColDef;
    SWORD   ibScale;
    SWORD   fNullable;
    SQLLEN  pcbValue1 = 0;
    SQLLEN  pcbValue2 = 0;

    UCHAR data[30000];
    SQLLEN len = 0;

    ProcessCmdArguments(argc,argv);

    retcode = SQLAllocEnv(&henv);
    SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLAllocConnect(henv, &hdbc);
    SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLConnect( hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
                          (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
    SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);

    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLAllocStmt (hdbc, &hstmt);
    SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    STRCPY(stmt, "DROP TABLE "TABLE);
    SPRINTF(buffer, "%s"CRLF, stmt);
    PRINTF(buffer);
    retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
    STRCPY(stmt, "create table "TABLE"( c long varchar, b long varchar)");
    SPRINTF(buffer, "%s"CRLF, stmt);
    PRINTF(buffer);
    retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
    retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
    SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    STRCPY(stmt, "insert into "TABLE" values (?, ?)");
    SPRINTF(buffer, "%s"CRLF, stmt);
    PRINTF(buffer);
    retcode = SQLPrepare (hstmt, stmt, STRLEN(stmt));
    SPRINTF(buffer, " SQLPrepare retcode: '%s'"CRLF, cnvret(retcode));
    PRINTF(buffer);

    retcode = SQLDescribeParam (hstmt, 1, &fSqlType, &cbColDef,
                                &ibScale, &fNullable);
    SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    MEMSET(szLONG1, 'x', sizeof(szLONG1));
    szLONG1[1000] = '\0';
    pcbValue1 = 1000;
    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                               SQL_LONGVARCHAR, cbColDef, ibScale,
                               &szLONG1, sizeof(szLONG1), &pcbValue1);
    SPRINTF(buffer, " SQLBindParameter retcode '%s'"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLDescribeParam ( hstmt, 2, &fSqlType, &cbColDef, &ibScale,
                                 &fNullable);
    SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    MEMSET(szLONG2, 'x', sizeof(szLONG2));
    szLONG2[4000] = '\0';
    pcbValue2 = 35000;
    retcode = SQLBindParameter( hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY,
                                SQL_LONGVARCHAR, 0, 0,
                                &szLONG2, sizeof(szLONG2), &pcbValue2 );
    SPRINTF(buffer, " SQLBindParameter retcode '%s'"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    for (row = 0; row < MAXROWS; row++) {
        retcode = SQLExecute (hstmt);
        if (retcode != SQL_SUCCESS) {
            SPRINTF(buffer, " inserting ros faild execution stoped."CRLF);
            ERROR_CHECK(hstmt);
            PRINTF(buffer);
        }
    }
    SPRINTF(buffer, " %ld row inserted."CRLF, row);
    PRINTF(buffer);

    retcode = SQLFreeStmt(hstmt, SQL_DROP);
    SPRINTF(buffer, " SQLFreeStmt (SQL_DROP) retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLAllocStmt (hdbc, &hstmt);
    SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    DoSelect( 1, SQL_CURSOR_STATIC, SQL_CONCUR_READ_ONLY );

    retcode = SQLExtendedFetch( hstmt, SQL_FETCH_FIRST, 1, &crow,
                                rgfRowStatus);
    SPRINTF( buffer, " SQLExtendedFetch '%s' (%ld) retcode: '%s' %d rows"CRLF,
             "SQL_FETCH_FIRST", 1, cnvret(retcode), crow);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);
    retcode = SQLGetData( hstmt, 1, SQL_C_BINARY, data, 18000, &len );
    SPRINTF( buffer, " SQLGetData retcode: '%s' len: %d"CRLF,
             cnvret(retcode), len);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLGetData( hstmt, 2, SQL_C_BINARY, data, 18000, &len );
    SPRINTF( buffer, " SQLGetData retcode: '%s' len: %d"CRLF,
             cnvret(retcode), len);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLExtendedFetch( hstmt, SQL_FETCH_NEXT, 1, &crow,
                                rgfRowStatus);
    SPRINTF( buffer, " SQLExtendedFetch '%s' (%ld) retcode: '%s' %d rows"CRLF,
             "SQL_FETCH_NEXT", 1, cnvret(retcode), crow);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLGetData( hstmt, 1, SQL_C_BINARY, data, 4096, &len );
    SPRINTF( buffer, " SQLGetData retcode: '%s' len: %d"CRLF,
             cnvret(retcode), len);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);
    retcode = SQLGetData( hstmt, 2, SQL_C_BINARY, data, 4096, &len );
    SPRINTF( buffer, " SQLGetData retcode: '%s' len: %d"CRLF,
             cnvret(retcode), len);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
    SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLDisconnect(hdbc);
    SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLFreeConnect(hdbc);
    SPRINTF(buffer, " SQLFreeConnect retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLFreeEnv(henv);
    SPRINTF(buffer, " SQLFreeEnv retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    SPRINTF(buffer, PROGNAME" ending."CRLF);
    PRINTF(buffer);

    return(0);
}

int DoSelect(int rowset, DWORD fSqlCursorType, DWORD fSqlConcur)
{
    SPRINTF(buffer, " New Test with %d rows"CRLF, rowset);
    PRINTF(buffer);
    retcode = SQLFreeStmt(hstmt, SQL_DROP);
    SPRINTF(buffer, " SQLSetFreeStmt retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLAllocStmt (hdbc, &hstmt);
    SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLSetStmtOption(hstmt, SQL_ROWSET_SIZE, rowset);
    SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLSetStmtOption(hstmt, SQL_KEYSET_SIZE, 1);
    SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLSetStmtOption(hstmt, SQL_CURSOR_TYPE, fSqlCursorType);
    SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLSetStmtOption(hstmt, SQL_CONCURRENCY, fSqlConcur);
    SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    STRCPY(stmt, "select * from "TABLE);
    SPRINTF(buffer, "%s"CRLF, stmt);
    PRINTF(buffer);

    retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
    SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    return 1;
}

UCHAR FAR *cnvrowret(UWORD status)
{
    static UCHAR str[20];
    switch(status) {
        WHCASESTRING(str, SQL_ROW_SUCCESS);
        WHCASESTRING(str, SQL_ROW_DELETED);
        WHCASESTRING(str, SQL_ROW_UPDATED);
        WHCASESTRING(str, SQL_ROW_NOROW);
        WHCASESTRING(str, SQL_ROW_ADDED);
        WHCASESTRING(str, SQL_ROW_ERROR);
    case 0xcaca: {
        STRCPY(str, "## UNTOUCHED ##");
        break;
    }
    default: {
        STRCPY(str, "## INVALID ##");
        break;
    }
    }
    return (str);
}
