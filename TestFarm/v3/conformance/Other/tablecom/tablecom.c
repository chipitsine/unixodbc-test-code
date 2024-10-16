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
#include "tst.h"

#define PROGNAME "TABLECOM"
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
UCHAR szQUALIFIER[QUALIFIER_LEN];
UCHAR szOWNER[OWNER_LEN];
UCHAR szNAME[TABLENAME_LEN];
UCHAR szTYPE[TYPE_LEN];
UCHAR szREMARKS[REMARKS_LEN];
UCHAR szTABLENAME[TABLENAME_LEN];
UCHAR szCOLUMNNAME[COLUMNNAME_LEN];
SWORD sDATATYPE;
UCHAR szTYPENAME[TYPENAME_LEN];
SDWORD sPRECISION;
SDWORD sLENGTH;
SWORD sSCALE;
SWORD sRADIX;
SWORD sNULLABLE;

SQLLEN cbQUALIFIER, cbOWNER, cbTABLENAME, cbCOLUMNNAME, cbDATATYPE, cbTYPENAME;
SQLLEN cbPRECISION, cbLENGTH, cbSCALE, cbRADIX, cbNULLABLE, cbREMARKS;
SQLLEN cbF1, cbC1;
SQLLEN cbNAME, cbTYPE;

void fetchouttable(HENV henv, HDBC hdbc, HSTMT hstmt);
void fetchoutcolumn(HENV henv, HDBC hdbc, HSTMT hstmt);

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

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
     ODBCUSER, (SWORD) SQL_NTS,	ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);

  retcode = SQLExecDirect (hstmt, "DROP TABLE "PROGNAME, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "CREATE TABLE "PROGNAME" (c char(10))", SQL_NTS);
  retcode = SQLExecDirect (hstmt, "CREATE SYNONYM odbc.S"PROGNAME" for "PROGNAME, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "COMMENT on TABLE "PROGNAME" IS 'Tabelle von ODBC Testprogramm'", SQL_NTS);
  retcode = SQLExecDirect (hstmt, "COMMENT on SYNONYM S"PROGNAME" IS 'Synonym von ODBC Testprogramm'", SQL_NTS);
  retcode = SQLExecDirect (hstmt, "COMMENT on COLUMN "PROGNAME".c IS 'Column von ODBC Testtabelle "PROGNAME" im ODBC Testprogramm'", SQL_NTS);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTables (hstmt, NULL, 0, ODBCUSER, SQL_NTS, PROGNAME, SQL_NTS, "'TABLE'", SQL_NTS);
  SPRINTF(buffer," SQLTables '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchouttable(henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
		       PROGNAME, SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLColumns '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchoutcolumn(henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
		       "S"PROGNAME, SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLColumns '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchoutcolumn(henv, hdbc, hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv '%s'"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchouttable(HENV henv, HDBC hdbc, HSTMT hstmt)
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
	                sizeof(szNAME)-1, &cbNAME);
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

VOID fetchoutcolumn(HENV henv, HDBC hdbc, HSTMT hstmt)
{
  RETCODE retcode;
  UCHAR state  [ 10 ];
  SDWORD sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD txt_len = 100;
  SWORD used_error_txt;
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
  while (retcode == SQL_SUCCESS) {
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
     cbREMARKS=SQL_NULL_DATA;

     *szQUALIFIER=0;
     *szOWNER=0;
     *szTABLENAME=0;
     *szCOLUMNNAME=0;
     sDATATYPE=99;
     *szTYPENAME=0;
     sPRECISION=99;
     sLENGTH=99;
     sSCALE=99;
     sRADIX=99;
     sNULLABLE=99;
     *szREMARKS=0;

     retcode = SQLFetch (hstmt);
    if (retcode == SQL_SUCCESS_WITH_INFO) {
      SQLError(henv, hdbc, hstmt, state,  &sql_code,
	       error_txt, txt_len, &used_error_txt);
    }
    if (retcode == SQL_SUCCESS)
    {
      SPRINTF(buffer, "TABLE_QUALIFIER = '%s' : %d"CRLF, szQUALIFIER, cbQUALIFIER);
      PRINTF(buffer);
      SPRINTF(buffer, "TABLE_OWNER = '%s' : %d"CRLF, szOWNER, cbOWNER);
      PRINTF(buffer);
      SPRINTF(buffer, "TABLE_NAME = '%s' : %d"CRLF, szTABLENAME, cbTABLENAME);
      PRINTF(buffer);
      SPRINTF(buffer, "COLUMN_NAME = '%s' : %d"CRLF, szCOLUMNNAME, cbCOLUMNNAME);
      PRINTF(buffer);
      SPRINTF(buffer, "DATA_TYPE = '%d' : %d"CRLF, sDATATYPE, cbDATATYPE);
      PRINTF(buffer);
      SPRINTF(buffer, "TYPE_NAME = '%s' : %d"CRLF, szTYPENAME, cbTYPENAME);
      PRINTF(buffer);
      SPRINTF(buffer, "PRECISION = '%ld' : %d"CRLF, sPRECISION, cbPRECISION);
      PRINTF(buffer);
      SPRINTF(buffer, "LENGTH = '%ld' : %d"CRLF, sLENGTH, cbLENGTH);
      PRINTF(buffer);
      SPRINTF(buffer, "SCALE = '%d' : %d"CRLF, sSCALE, cbSCALE);
      PRINTF(buffer);
      SPRINTF(buffer, "RADIX = '%d' : %d"CRLF, sRADIX, cbRADIX);
      PRINTF(buffer);
      SPRINTF(buffer, "NULLABLE = '%d' : %d"CRLF, sNULLABLE, cbNULLABLE);
      PRINTF(buffer);
      SPRINTF(buffer, "REMARKS = '%s' : %d"CRLF, szREMARKS, cbREMARKS);
      PRINTF(buffer);
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
      PRINTF(buffer);
    }
  }
}
