/*



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
#define PROGNAME "EXT750"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define MAXROWS 30

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			   SPRINTF( buffer, " sqlstate: %s"CRLF, sqlstate);\
			      PRINTF(buffer);\
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
			      PRINTF(buffer);\
                  switch (retcode) {\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ return(16); }\
		     break;\
		  case(100) :\
			  break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR buffer [1024];

#define SQL_SELECT_NEW (1000)

TestTableStruct TestValues[] = {
{ SQL_SELECT_NEW, 5, "", 0, 0 },
{ SQL_FETCH_NEXT, 0, "", 0, 0 },
};

UWORD NumTestValues = sizeof(TestValues) / sizeof(TestTableStruct);

UCHAR szCHAR[MAXROWS][255];
SDWORD cbINTEGER[MAXROWS];
SQLLEN cbCharValue[MAXROWS];
SDWORD cbINTEGER[MAXROWS];
SQLLEN cbIntValue[MAXROWS];


UWORD rgfRowStatus[MAXROWS];
#define FreeStatusArray MEMSET(rgfRowStatus, 0XCA, sizeof(rgfRowStatus));
VOID PrintArrSet(int rowset);
UCHAR FAR *cnvrowret(UWORD status);
int DoSelect(int rowset);

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

HSTMT hstmtloop[17];

int main(int argc, argv_t argv)
{
  UDWORD row;
  SQLROWSETSIZE crow;
  TestTableStruct *p;
  int rowset=0;
  int i;

  ProcessCmdArguments(argc,argv);

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

  retcode = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "DROP TABLE "TABLE);
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, STRLEN(stmt));
  STRCPY(stmt, "create table "TABLE"(i integer, c char (20))");
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

  retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_DEFAULT,
			     SQL_INTEGER, 0, 0, &cbINTEGER[0], 0, NULL);
  SPRINTF(buffer, " SQLBindParameter retcode '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_DEFAULT,
			     SQL_CHAR, 0, 0, &szCHAR[0], 0, NULL);
  SPRINTF(buffer, " SQLBindParameter retcode '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  for (row = 0; row < MAXROWS; row++) {
    cbINTEGER[0] = row;
    SPRINTF(szCHAR[0], "Zeile %lu", row);
    retcode = SQLExecute (hstmt);
    SPRINTF(buffer, " SQLExecute retcode: '%s'"CRLF, cnvret(retcode));
    PRINTF(buffer);
  }

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  ERROR_CHECK(NULL);

  retcode = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_ON);
  ERROR_CHECK(NULL);

  STRCPY(stmt, "select * from "TABLE" order by 1");
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);

  retcode = SQLPrepare (hstmt, stmt, STRLEN(stmt));
  SPRINTF(buffer, " SQLPrepare retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  for(i=0;i<(sizeof(hstmtloop)/sizeof(hstmtloop[0]));i++) {
    retcode = SQLAllocStmt(hdbc, &hstmtloop[i]);
    ERROR_CHECK(NULL);
    STRCPY(stmt, "select * from "TABLE);
    retcode = SQLExecDirect (hstmtloop[i], stmt, STRLEN(stmt));
    ERROR_CHECK(hstmt);
    retcode = SQLFetch(hstmtloop[i]);
  }

  for (p=TestValues; NumTestValues > 0 ; p++, NumTestValues--) {
     if (p->fFetchType == SQL_SELECT_NEW) {
	rowset = p->irow;
	DoSelect(rowset);
     }
     else {
	FreeStatusArray;
	retcode = SQLExtendedFetch(hstmt, p->fFetchType, p->irow, &crow, rgfRowStatus);
	SPRINTF(buffer, " SQLExtendedFetch '%s' (%ld) retcode: '%s'"CRLF, cnvFetchType(p->fFetchType), p->irow, cnvret(retcode));
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	PrintArrSet(rowset);
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

int DoSelect(int rowset) {
  SPRINTF(buffer, " New Test with %d rows"CRLF, rowset);
  PRINTF(buffer);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLSetFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLSetStmtOption(hstmt, SQL_ROWSET_SIZE, rowset);
  SPRINTF(buffer, " SQLSetStmtOption retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLBindCol (hstmt, 1, SQL_C_DEFAULT, &cbINTEGER[0],
     sizeof(cbINTEGER[0]), &cbIntValue[0]);
  retcode = SQLBindCol (hstmt, 2, SQL_C_DEFAULT, szCHAR[0],
     sizeof(szCHAR[0]), &cbCharValue[0]);

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
