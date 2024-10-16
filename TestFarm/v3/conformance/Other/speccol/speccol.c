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
#define PROGNAME "speccol"
#include "tst.h"

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
			   printf(" sqlstate: %s\n", sqlstate);\
		  printf(" esq_sql_code: %ld\n", esq_sql_code);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];

UCHAR szCOLUMNNAME[COLUMNNAME_LEN+1];
SWORD sDATATYPE;
SWORD sSCOPE;
UCHAR szTYPENAME[TYPENAME_LEN+1];
SWORD sPRECISION;
SWORD sLENGTH;
SWORD sSCALE;

SQLLEN cbQUALIFIER, cbOWNER, cbTABLENAME, cbCOLUMNNAME, cbDATATYPE, cbTYPENAME;
SQLLEN cbPRECISION, cbLENGTH, cbSCALE, cbSCOPE, cbRADIX, cbNULLABLE, cbREMARKS;
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
  UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
  SWORD len_error_txt = ERROR_TEXT_LEN;
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

  SPRINTF(buffer," %s running.\n", PROGNAME);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) STRLEN(DBNODE), ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLSpecialColumns (hstmt,
	               SQL_ROWVER,
	               NULL, 0,
		       ODBCUSER, (SWORD) STRLEN(ODBCUSER),
		       "ATMP", SQL_NTS,
		       SQL_SCOPE_TRANSACTION,
		       SQL_NULLABLE);
  SPRINTF(buffer, " SQLSpecialColumns retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  SPRINTF(buffer, " SQLTransact retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %d\n", retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer," %s ending\n", PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchout(hstmt)
HSTMT hstmt;
{
  RETCODE retcode;
  retcode = SQLBindCol (hstmt, 1, SQL_C_SHORT, &sSCOPE,
	                sizeof(sSCOPE), &cbSCOPE);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szCOLUMNNAME,
	                sizeof(szCOLUMNNAME), &cbCOLUMNNAME);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 3, SQL_C_SHORT, &sDATATYPE,
	                sizeof(sDATATYPE), &cbDATATYPE);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 4, SQL_C_CHAR, szTYPENAME,
	                sizeof(szTYPENAME), &cbTYPENAME);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 5, SQL_C_SHORT, &sPRECISION,
	                sizeof(sPRECISION), &cbPRECISION);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 6, SQL_C_SHORT, &sLENGTH,
	                sizeof(sLENGTH), &cbLENGTH);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  retcode = SQLBindCol (hstmt, 9, SQL_C_SHORT, &sSCALE,
	                sizeof(sSCALE), &cbSCALE);
  SPRINTF(buffer, " SQLBindCol retcode: %d\n", retcode);
  PRINTF(buffer);

  while (retcode == SQL_SUCCESS) {
  retcode = SQLFetch (hstmt);
  SPRINTF(buffer, " SQLFetch retcode: '%d'\n", retcode);
  PRINTF(buffer);
  SPRINTF(buffer, "SCOPE = '%d'\n", sSCOPE);
  PRINTF(buffer);
  SPRINTF(buffer, "COLUMN_NAME = '%s'\n", szCOLUMNNAME);
  PRINTF(buffer);
  SPRINTF(buffer, "DATA_TYPE = '%d'\n", sDATATYPE);
  PRINTF(buffer);
  SPRINTF(buffer, "TYPE_NAME = '%s'\n", szTYPENAME);
  PRINTF(buffer);
  SPRINTF(buffer, "PRECISION = '%d'\n", sPRECISION);
  PRINTF(buffer);
  SPRINTF(buffer, "LENGTH = '%d'\n", sLENGTH);
  PRINTF(buffer);
  SPRINTF(buffer, "SCALE = '%d'\n", sSCALE);
  PRINTF(buffer);
  }
}
