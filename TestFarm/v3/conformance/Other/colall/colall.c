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
#define PROGNAME "COLALL"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

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
		  case(100) :\
			  break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

UCHAR szQUALIFIER[QUALIFIER_LEN];
UCHAR szOWNER[OWNER_LEN];
UCHAR szTABLENAME[TABLENAME_LEN];
UCHAR szCOLUMNNAME[COLUMNNAME_LEN];
SWORD sDATATYPE;
UCHAR szTYPENAME[TYPENAME_LEN];
SDWORD sPRECISION;
SDWORD sLENGTH;
SWORD sSCALE;
SWORD sRADIX;
SWORD sNULLABLE;
UCHAR szREMARKS[REMARKS_LEN];

SQLLEN cbQUALIFIER, cbOWNER, cbTABLENAME, cbCOLUMNNAME, cbDATATYPE, cbTYPENAME;
SQLLEN cbPRECISION, cbLENGTH, cbSCALE, cbRADIX, cbNULLABLE, cbREMARKS;

int fetchout(HENV henv, HDBC hdbc, HSTMT hstmt);

int main(int argc, argv_t argv)
{
  HENV henv   = SQL_NULL_HENV;
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
  UCHAR stmt [ 1024 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;

  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(stmt, "DROP TABLE "TABLE);
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
  STRCPY(stmt, "create table "TABLE" ( ");
  STRCAT(stmt, "c1 char (10), ");
  STRCAT(stmt, "c2 decimal(10,0), ");
  STRCAT(stmt, "c3 fixed(10, 0), ");
  STRCAT(stmt, "c4 integer, ");
  STRCAT(stmt, "c5 smallint, ");
  STRCAT(stmt, "c6 float, ");
  STRCAT(stmt, "c7 real, ");
  STRCAT(stmt, "c8 double precision, ");
  STRCAT(stmt, "c9 date, ");
  STRCAT(stmt, "c10 time, ");
  STRCAT(stmt, "c11 varchar(100), ");
  STRCAT(stmt, "c12 long, ");
  STRCAT(stmt, "c13 char(20) byte, ");
  STRCAT(stmt, "c14 varchar(200) byte, ");
  STRCAT(stmt, "c15 long byte, ");
  STRCAT(stmt, "c16 boolean, ");
  STRCAT(stmt, "c17 timestamp ");
  STRCAT(stmt, ")");
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
     TABLE, SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLColumns retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " SQLFreeStmt (SQL_DROP) retcode: %d"CRLF, retcode);
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

int fetchout(HENV henv, HDBC hdbc, HSTMT hstmt)
{
  RETCODE retcode;
  UCHAR error_txt  [ 101 ] ;
  SWORD txt_len = 100;
  SWORD used_error_txt;
  SWORD len_error_txt = 100;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szQUALIFIER,
	                sizeof(szQUALIFIER), &cbQUALIFIER);
  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szOWNER,
	                sizeof(szOWNER), &cbOWNER);
  retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szTABLENAME,
	                sizeof(szTABLENAME), &cbTABLENAME);
  retcode = SQLBindCol (hstmt, 4, SQL_C_CHAR, szCOLUMNNAME,
	                sizeof(szCOLUMNNAME), &cbCOLUMNNAME);
  retcode = SQLBindCol (hstmt, 5, SQL_C_SHORT, &sDATATYPE,
	                sizeof(sDATATYPE), &cbDATATYPE);
  retcode = SQLBindCol (hstmt, 6, SQL_C_CHAR, szTYPENAME,
	                sizeof(szTYPENAME), &cbTYPENAME);
  retcode = SQLBindCol (hstmt, 7, SQL_C_LONG, &sPRECISION,
	                sizeof(sPRECISION), &cbPRECISION);
  retcode = SQLBindCol (hstmt, 8, SQL_C_LONG, &sLENGTH,
	                sizeof(sLENGTH), &cbLENGTH);
  retcode = SQLBindCol (hstmt, 9, SQL_C_SHORT, &sSCALE,
	                sizeof(sSCALE), &cbSCALE);
  retcode = SQLBindCol (hstmt, 10, SQL_C_SHORT, &sRADIX,
	                sizeof(sRADIX), &cbRADIX);
  retcode = SQLBindCol (hstmt, 11, SQL_C_SHORT, &sNULLABLE,
	                sizeof(sNULLABLE), &cbNULLABLE);
  retcode = SQLBindCol (hstmt, 12, SQL_C_CHAR, szREMARKS,
	                sizeof(szREMARKS), &cbREMARKS);
  SPRINTF(buffer, " SQLBindCol retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
     cbQUALIFIER=SQL_NULL_DATA;
     cbREMARKS=SQL_NULL_DATA;
     cbNULLABLE=SQL_NULL_DATA;
     cbRADIX=SQL_NULL_DATA;
     cbSCALE=SQL_NULL_DATA;
     cbLENGTH=SQL_NULL_DATA;
     cbPRECISION=SQL_NULL_DATA;
     cbTYPENAME=SQL_NULL_DATA;
     cbDATATYPE=SQL_NULL_DATA;
     cbCOLUMNNAME=SQL_NULL_DATA;
     cbTABLENAME=SQL_NULL_DATA;
     cbOWNER=SQL_NULL_DATA;
     cbQUALIFIER=SQL_NULL_DATA;
     retcode = SQLFetch (hstmt);
    if (retcode == SQL_ERROR) {
       ERROR_CHECK(hstmt);
    }
    if (retcode == SQL_SUCCESS)
    {
      SPRINTF(buffer, "TABLE_QUALIFIER = '%s'"CRLF, szQUALIFIER);
      PRINTF(buffer);
      SPRINTF(buffer, "TABLE_OWNER = '%s'"CRLF, szOWNER);
      PRINTF(buffer);
      SPRINTF(buffer, "TABLE_NAME = '%s'"CRLF, szTABLENAME);
      PRINTF(buffer);
      SPRINTF(buffer, "COLUMN_NAME = '%s'"CRLF, szCOLUMNNAME);
      PRINTF(buffer);
      SPRINTF(buffer, "DATA_TYPE = '%d'"CRLF, (cbDATATYPE == SQL_NULL_DATA) ? cbDATATYPE : sDATATYPE);
      PRINTF(buffer);
      SPRINTF(buffer, "TYPE_NAME = '%s'"CRLF, szTYPENAME);
      PRINTF(buffer);
      SPRINTF(buffer, "PRECISION = '%ld'"CRLF, (cbPRECISION == SQL_NULL_DATA) ? cbPRECISION : sPRECISION);
      PRINTF(buffer);
      SPRINTF(buffer, "LENGTH = '%d'"CRLF, (cbLENGTH == SQL_NULL_DATA) ? cbLENGTH : sLENGTH);
      PRINTF(buffer);
      SPRINTF(buffer, "SCALE = '%d'"CRLF, (cbSCALE == SQL_NULL_DATA) ? cbSCALE : sSCALE);
      PRINTF(buffer);
      SPRINTF(buffer, "RADIX = '%d'"CRLF, (cbRADIX == SQL_NULL_DATA) ? cbRADIX : sRADIX);
      PRINTF(buffer);
      SPRINTF(buffer, "NULLABLE = '%d'"CRLF, (cbNULLABLE == SQL_NULL_DATA) ? cbNULLABLE : sNULLABLE);
      PRINTF(buffer);
      SPRINTF(buffer, "REMARKS = '%s'"CRLF, szREMARKS);
      PRINTF(buffer);
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %s"CRLF, cnvret(retcode));
      PRINTF(buffer);
    }
  }
  return 1;
}
