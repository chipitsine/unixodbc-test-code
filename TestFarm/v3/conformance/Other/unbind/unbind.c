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
#define PROGNAME "UNBIND"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
				    sqlstate, &esq_sql_code,\
				    error_txt, len_error_txt,\
				    &used_error_txt);\
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %ld"CRLF, esq_sql_code);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

UCHAR szQUALIFIER[QUALIFIER_LEN+1];
UCHAR szOWNER[OWNER_LEN+1];
UCHAR szTABLENAME[TABLENAME_LEN+1];
UCHAR szCOLUMNNAME[COLUMNNAME_LEN+1];
SWORD sDATATYPE;
UCHAR szTYPENAME[TYPENAME_LEN];
SWORD sPRECISION;
SWORD sLENGTH;
SWORD sSCALE;
SWORD sRADIX;
SWORD sNULLABLE;
UCHAR szREMARKS[REMARKS_LEN+1];

SQLLEN cbQUALIFIER, cbOWNER, cbTABLENAME, cbCOLUMNNAME, cbDATATYPE, cbTYPENAME;
SQLLEN cbPRECISION, cbLENGTH, cbSCALE, cbRADIX, cbNULLABLE, cbREMARKS;
void fetchout();

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
  UCHAR stmt [ 1024 ];
  UCHAR stmt2 [ 1024 ];
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  SDWORD size_1 = 0;
  SDWORD size_2 = 0;

  UCHAR name [ 21 ];
  SDWORD age = -1;

  SWORD description_type;
  UCHAR description [ 33 ];
  SDWORD description_num;
  SWORD description_size;
  UWORD icol;
  UWORD col_count = 2;

  RETCODE retcode;
  SWORD safe_cnt;

  UWORD      fInfoType;
  PTR        rgbInfoValue;
  SWORD      cbInfoValueMax;
  SWORD  FAR *pcbInfoValue;

  UCHAR stuff [ 80 ];

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, " %s running."CRLF, PROGNAME);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  STRCPY(stmt, "DROP TABLE "TABLE);
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);
  STRCPY(stmt, "create table "TABLE" ( f fixed(12,3), c char (10))");
  SPRINTF(buffer, "%s"CRLF, stmt);
  PRINTF(buffer);
  retcode = SQLExecDirect (hstmt, stmt, SQL_NTS);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
		       TABLE, SQL_NTS, NULL, 0);
  SPRINTF(buffer, " SQLColumns (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_UNBIND);
  SPRINTF(buffer, " SQLFreeStmt (SQL_UNBIND) (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt (SQL_CLOSE) (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLColumns (hstmt, NULL, 0, ODBCUSER, SQL_NTS,
		       TABLE, SQL_NTS,
		       NULL, 0);
  SPRINTF(buffer, " SQLColumns (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " SQLFreeStmt (SQL_DROP) (%s)"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

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

  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchout(henv, hdbc, hstmt)
HENV henv;
HDBC hdbc;
HSTMT hstmt;
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
  retcode = SQLBindCol (hstmt, 7, SQL_C_SHORT, &sPRECISION,
	                sizeof(sPRECISION), &cbPRECISION);
  retcode = SQLBindCol (hstmt, 8, SQL_C_SHORT, &sLENGTH,
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
      SPRINTF(buffer, "PRECISION = '%d' : %d"CRLF, sPRECISION, cbPRECISION);
      PRINTF(buffer);
      SPRINTF(buffer, "LENGTH = '%d' : %d"CRLF, sLENGTH, cbLENGTH);
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
      SPRINTF(buffer," SQLFetch (%s)"CRLF, cnvret(retcode));
      PRINTF(buffer);
    }
  }
}
