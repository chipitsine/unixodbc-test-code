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
#define PROGNAME "tables"
#include "tst.h"

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				 len_error_txt, &used_error_txt);\
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
		  SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
                  PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

#define F1_LEN 11

UCHAR szF1[F1_LEN];
UCHAR szQUALIFIER[QUALIFIER_LEN+1];
UCHAR szOWNER[OWNER_LEN+1];
UCHAR szNAME[NAME_LEN+1];
UCHAR szTYPE[TYPE_LEN+1];
UCHAR szREMARKS[REMARKS_LEN+1];
SDWORD cbF1, cbC1;
SQLLEN cbQUALIFIER, cbOWNER, cbNAME, cbTYPE, cbREMARKS;

void fetchout (HENV henv, HDBC hdbc, HSTMT hstmt);

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HENV FAR * phenv;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC FAR * phdbc;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  HSTMT FAR * phstmt2;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  RETCODE retcode;
  WORD cbTableNameMax;
  UCHAR szLongTableName[255];

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer," %s running."CRLF, PROGNAME);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc,
		       DBNODE,
		       (SWORD) SQL_NTS,
		       ODBCUSER,
		       (SWORD) SQL_NTS,
		       ODBCPASSW,
		       (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);

  retcode = SQLTables (hstmt, "%", SQL_NTS, "", 0, "", 0, NULL, 0);
  SPRINTF(buffer," SQLTables (qualifier scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, "", 0, "%", SQL_NTS, "", 0, NULL, 0);
  SPRINTF(buffer," SQLTables (owner scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, "%", SQL_NTS, "%", SQL_NTS, "'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables (table scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, "%", SQL_NTS, "%", SQL_NTS, "'SYSTEM TABLE', 'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables (systemtable scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, ODBCUSER, SQL_NTS, "ATMP", SQL_NTS, "'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables (table scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  #define LONGTABLENAME1 "1_3_5_7_9_1_3_5_7_"
  #define LONGTABLENAME2 "123456789012345678"
  #define LONGTABLENAMEESCAPE "1\\_3\\_5\\_7\\_9\\_1\\_3\\_5\\_7\\_"
  retcode = SQLExecDirect (hstmt, "drop table \""LONGTABLENAME1"\"", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "create table \""LONGTABLENAME1"\" (col1 char (10) not null)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "drop table \""LONGTABLENAME2"\"", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLExecDirect (hstmt, "create table \""LONGTABLENAME2"\" (col1 char (10) not null)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLGetInfo(hdbc, SQL_MAX_TABLE_NAME_LEN, &cbTableNameMax, sizeof(cbTableNameMax), NULL);

  MakeLongName(LONGTABLENAME2, szLongTableName, cbTableNameMax);
  SPRINTF(buffer, "drop table \"%s\"", szLongTableName);
  retcode = SQLExecDirect (hstmt, buffer, SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, "create table \"%s\" (\"%s\" char (10))", szLongTableName, szLongTableName);
  retcode = SQLExecDirect (hstmt, buffer, SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, ODBCUSER, SQL_NTS, LONGTABLENAME1, SQL_NTS, "'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables (table scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, ODBCUSER, SQL_NTS, LONGTABLENAMEESCAPE, SQL_NTS, "'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables (table scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, ODBCUSER, SQL_NTS, szLongTableName, SQL_NTS, "'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables (langer Tabellen name) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, "", 0, "", 0, "", 0, NULL, SQL_NTS);
  SPRINTF(buffer," SQLTables (type scan) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout (henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  SPRINTF(buffer," SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchout (HENV henv, HDBC hdbc, HSTMT hstmt)
{
  RETCODE retcode;
  UCHAR state  [ 10 ];
  SDWORD sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD txt_len = 100;
  SWORD used_error_txt;
  retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szQUALIFIER,
	                sizeof(szQUALIFIER), &cbQUALIFIER);
  SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szOWNER,
	                sizeof(szOWNER), &cbOWNER);
  SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szNAME,
	                sizeof(szNAME), &cbNAME);
  SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 4, SQL_C_CHAR, szTYPE,
	                sizeof(szTYPE), &cbTYPE);
  SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 5, SQL_C_CHAR, szREMARKS,
	                sizeof(szREMARKS), &cbREMARKS);
  SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  while (retcode == SQL_SUCCESS) {
     cbQUALIFIER = SQL_NULL_DATA;
     cbOWNER = SQL_NULL_DATA;
     cbNAME=SQL_NULL_DATA;
     cbTYPE = SQL_NULL_DATA;
     cbREMARKS=SQL_NULL_DATA;
     MEMSET(szQUALIFIER, 'x', sizeof(szQUALIFIER));
     MEMSET(szOWNER, 'x', sizeof(szOWNER));
     MEMSET(szNAME, 'x', sizeof(szNAME));
     MEMSET(szTYPE, 'x', sizeof(szTYPE));
     MEMSET(szREMARKS, 'x', sizeof(szREMARKS));
    retcode = SQLFetch (hstmt);
    if (retcode == SQL_SUCCESS_WITH_INFO) {
      SQLError(henv, hdbc, hstmt, state,  &sql_code,
	       error_txt, txt_len, &used_error_txt);
    }
    if (retcode == SQL_SUCCESS ){
       SPRINTF(buffer," SQLFetch retcode: '%d'"CRLF, retcode);
       PRINTF(buffer);
       SPRINTF(buffer,"TABLE_QUALIFIER = '%s':%d"CRLF, szQUALIFIER, cbQUALIFIER);
       PRINTF(buffer);
       SPRINTF(buffer,"TABLE_OWNER = '%s':%d"CRLF, szOWNER, cbOWNER);
       PRINTF(buffer);
       SPRINTF(buffer,"TABLE_NAME = '%s':%d"CRLF, szNAME, cbNAME);
       PRINTF(buffer);
       SPRINTF(buffer,"TABLE_TYPE = '%s':%d"CRLF, szTYPE, cbTYPE);
       PRINTF(buffer);
       SPRINTF(buffer,"REMARKS = '%s':%d"CRLF, szREMARKS, cbREMARKS);
       PRINTF(buffer);
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
      PRINTF(buffer);
    }
  }
}
