/* Dieses von extesing abgeleitete Programm testet das spezial Statement-
 * Attribut SQL_ATTR_OPTIMIZE_CURSOR, bei dem auf ein FOR REUSE verzichtet
 * wird. Die Ergebnismenge wird einmal durchgefetched und anschließend
 * wird ein fetch absolute auf die letzte Zeile genacht.



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

#define PROGNAME "OPTCUR"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#include <time.h>

#define MAXROWS 30
#define NUM_ROWS 5

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
			{ }\
		     break;\
		  case(100) :\
			  break;\
		  default   :\
		          if (STRCMP(sqlstate,"23000"))\
		          if (STRCMP(sqlstate,"01S01"))\
		       {return(16);}\
			  break; } } } }

UCHAR buffer [1024];

#define SQL_SELECT_NEW (1000)
#define SQL_UPDATE_ROW (1001)
#define SQL_INSERT_ROW (1002)
#define SQL_DELETE_ROW (1003)

TestTableStruct TestValues[] = {
{ SQL_SELECT_NEW, 1, "", SQL_CURSOR_KEYSET_DRIVEN, SQL_CONCUR_READ_ONLY },
{ SQL_FETCH_FIRST, 0, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
{ SQL_FETCH_ABSOLUTE, 5, "", 0, 0 }
};

UWORD NumTestValues = sizeof(TestValues) / sizeof(TestTableStruct);

UCHAR szCHAR[MAXROWS][255];
SQLINTEGER cbINTEGER[MAXROWS];
SQLLEN cbCharValue[MAXROWS];
SQLLEN cbIntValue[MAXROWS];

UWORD rgfRowStatus[MAXROWS];

#define FreeStatusArray MEMSET(rgfRowStatus, 0XCA, sizeof(rgfRowStatus));

VOID PrintArrSet(int rowset);
UCHAR FAR *cnvrowret(UWORD status);
int DoSelect(int rowset, DWORD fSqlCursorType, DWORD fSqlConcur);
int DoSetPos(TestTableStruct *p, UDWORD *row);

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
  TestTableStruct *p;
  int rowset=0;
  SWORD  FAR fSqlType;
  SQLULEN cbColDef;
  SWORD FAR ibScale;
  SWORD FAR fNullable;
  time_t ltime, starttime;

  ProcessCmdArguments(argc,argv);

  time( &starttime );

  NumTestValues = sizeof(TestValues) / sizeof(TestTableStruct);
  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);
  SPRINTF(buffer, "NumTestValues=%d"CRLF, NumTestValues);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(stmt, "drop table "TABLE);
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
  STRCPY(stmt, "create table "TABLE"(i integer, c char (20))");
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
  STRCPY(stmt, "create unique index i1 on "TABLE" (i)");
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

  retcode = SQLDescribeParam (hstmt, 1, &fSqlType, &cbColDef, &ibScale, &fNullable);
  SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_DEFAULT,
     fSqlType, cbColDef, ibScale, &cbINTEGER[0], 0, NULL);
  SPRINTF(buffer, " SQLBindParameter retcode '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLDescribeParam (hstmt, 2, &fSqlType, &cbColDef, &ibScale, &fNullable);
  SPRINTF(buffer," SQLDescribeParam retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_DEFAULT,
     fSqlType, cbColDef, ibScale, &szCHAR[0], 0, NULL);
  SPRINTF(buffer, " SQLBindParameter retcode '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  for (row = 0; row < NUM_ROWS; row++) {
     cbINTEGER[0] = row;
     SPRINTF(szCHAR[0], "Zeile %lu", row);
     retcode = SQLExecute (hstmt);
     if (retcode != SQL_SUCCESS) {
	SPRINTF(buffer, " inserting ros faild execution stoped."CRLF);
	ERROR_CHECK(hstmt);
	PRINTF(buffer);
     }
  }
  SPRINTF(buffer, " %ld row inserted."CRLF, row);
  PRINTF(buffer);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_ON);
  ERROR_CHECK(NULL);

  for (p=TestValues; NumTestValues > 0 ; p++, NumTestValues--) {
     switch(p->fFetchType) {
	case SQL_SELECT_NEW: {
	   rowset = p->irow;
	   DoSelect(rowset, p->fSqlCursorType, p->fSqlConcur);
	   break;
	}
	case SQL_UPDATE_ROW: {}
	case SQL_INSERT_ROW: {}
	case SQL_DELETE_ROW: {
	   DoSetPos(p, &row);
	   PrintArrSet(rowset);
	   break;
	}
	default:{
	   FreeStatusArray;
	   retcode = SQLExtendedFetch(hstmt, p->fFetchType, p->irow, &crow, rgfRowStatus);
	   SPRINTF(buffer, " SQLExtendedFetch '%s' (%ld) retcode: '%s'"CRLF, cnvFetchType(p->fFetchType), p->irow, cnvret(retcode));
	   PRINTF(buffer);
	   ERROR_CHECK(hstmt);
	   PrintArrSet(rowset);
	   break;
	}
     }
  }

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
  /*
  time( &ltime );
  printf( "Time in seconds since UTC 1/1/70:\t%ld\n", ltime-starttime );
  */

  return(0);
}

VOID PrintArrSet(int rowset)
{
   int row;
   for (row=0; row < rowset; row++) {
     SPRINTF(buffer, "s='%s', i=(%ld)%ld, c=(%ld)'%s'"CRLF, cnvrowret(rgfRowStatus[row]), cbIntValue[row], cbINTEGER[row], cbCharValue[row], szCHAR[row]);
     PRINTF(buffer);
  }
}

int DoSelect(int rowset, DWORD fSqlCursorType, DWORD fSqlConcur)
{
  SPRINTF(buffer, " New Test with %d rows and '%s'"CRLF, rowset, cnvcurs(fSqlCursorType));
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

  retcode = SQLSetStmtOption(hstmt, SQL_CURSOR_TYPE, fSqlCursorType);
  SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLSetStmtOption(hstmt, SQL_CONCURRENCY, fSqlConcur);
  SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  /* set SQL_ATTR_OPTIMIZE_CURSOR */
  retcode = SQLSetStmtOption(hstmt, 3002, 1 );
  SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(stmt, "select * from optcur");
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
  SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLBindCol (hstmt, 1, SQL_C_DEFAULT, &cbINTEGER[0],
     sizeof(cbINTEGER[0]), &cbIntValue[0]);
  retcode = SQLBindCol (hstmt, 2, SQL_C_DEFAULT, szCHAR[0],
     sizeof(szCHAR[0]), &cbCharValue[0]);

  return 1;
}

int DoSetPos(TestTableStruct *p, UDWORD *row)
{
   UWORD       fOption;
   switch(p->fFetchType) {
      case (SQL_UPDATE_ROW):{
	 fOption = SQL_UPDATE;
	 SPRINTF(buffer, " Update row %ld to '%s'."CRLF, p->irow, p->szChar);
	 if (p->irow > 0) {
	   STRNCPY(szCHAR[p->irow-1], p->szChar, sizeof(szCHAR[0]));
	   cbCharValue[p->irow-1] = SQL_NTS;
	   cbINTEGER[p->irow-1] = p->irow-1;
	   cbIntValue[p->irow-1] = 0;
	 }
	 break;
      }
      case (SQL_INSERT_ROW):{
	 fOption = SQL_ADD;
	 SPRINTF(buffer, " Insert row %ld with %s."CRLF, p->irow, p->szChar);
	 if (p->irow > 0) {
	   cbINTEGER[p->irow-1]=(*row)++;
	   cbIntValue[p->irow-1] = 0;
	   STRNCPY(szCHAR[p->irow-1], p->szChar, sizeof(szCHAR[0]));
	   cbCharValue[p->irow-1] = SQL_NTS;
	 }
	 break;
      }
      case (SQL_DELETE_ROW):{
	 fOption = SQL_DELETE;
	 SPRINTF(buffer, " Delete row %ld."CRLF, p->irow);
	 break;
      }
   }
   PRINTF(buffer);
   retcode = SQLSetPos(hstmt, (UWORD)p->irow, fOption, SQL_LOCK_NO_CHANGE);
   SPRINTF(buffer, " SQLSetPos retcode: '%s'"CRLF, cnvret(retcode));
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
      case 0xca: {
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
