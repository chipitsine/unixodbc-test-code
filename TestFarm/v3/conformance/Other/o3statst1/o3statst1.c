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
#define PROGNAME "statst1"
#include "o3util.c"

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
			   SPRINTF(buffer," used_error_txt: %d"CRLF, used_error_txt);\
		  PRINTF(buffer);\
			   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
		  PRINTF(buffer);\
		  SPRINTF(buffer, " %s"CRLF, error_txt);\
		  PRINTF(buffer);\
                  switch (retcode) {\
		  case(SQL_NO_DATA_FOUND) :\
			  break;\
		  case(SQL_ERROR) : \
		          switch(esq_sql_code) {\
			        case(-6000):\
				   break;\
				default:\
				   exit(17);\
				   break;\
			  }\
			  break;\
		  default   :\
			  exit(16);\
			  break; } }

UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
SWORD len_error_txt = ERROR_TEXT_LEN;
SWORD used_error_txt;
UCHAR driverver [ 11 ];
UCHAR buffer [1024];


UCHAR szOWNER[OWNER_LEN+1];
UCHAR szTABLENAME[TABLENAME_LEN+1];
SWORD sNON_UNIQUE;
UCHAR szINDEX_QUALIFIER[INDEX_QUALIFIER_LEN+1];
UCHAR szINDEX_NAME[INDEX_NAME_LEN+1];
SWORD sTYPE;
UCHAR szCOLUMNNAME[COLUMNNAME_LEN+1];
UCHAR szCOLLATION[COLLATION_LEN+1];
SDWORD sCARDINALITY;
SDWORD sPAGES;

SQLLEN cbOWNER, cbTABLENAME, cbNON_UNIQUE, cbINDEX_QUALIFIER,
       cbINDEX_NAME, cbTYPE, cbCOLUMNNAME, cbCOLLATION,
       cbCARDINALITY, cbPAGES;

void fetchout ();

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
  UCHAR stmt [ 3000 ];
  UCHAR stmt2 [ 50 ];
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

  printf(" statstc running."CRLF);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  FullConnect(phenv, phdbc);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "create table o3initab (s1 int)", SQL_NTS);
  ERROR_CHECK(hstmt);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create table o3tab1 (s1 int, s2 int, s3 int)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert o3tab1 values (1, 2, 3)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert o3tab1 values (2, 2, 3)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert o3tab1 values (3, 3, 4)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert o3tab1 values (4, 3, 3)", SQL_NTS);
 retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create unique index o3inda on tab1 (s1 asc)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create index o3ind1 on o3tab1 (s2 asc, s3 desc)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create view o3view1 as select * from tab1", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create synonym o3syn1 for tab1", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create table o3pk (col int, primary key (col))", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert o3pk values (1)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "insert o3pk values (2)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_ALL, SQL_QUICK retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_ENSURE);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_ALL, SQL_ENSURE retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_UNIQUE,
		       SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_UNIQUE, SQL_QUICK retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_UNIQUE,
		       SQL_ENSURE);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_UNIQUE, SQL_ENSURE retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3VIEW1", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_ALL, SQL_QUICK retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3SYN1", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_ALL, SQL_QUICK retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3PK", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_ALL, SQL_QUICK retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3PK", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_ENSURE);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_ALL, SQL_ENSURE retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3PK", SQL_NTS,
		       SQL_INDEX_UNIQUE,
		       SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_UNIQUE, SQL_QUICK retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3PK", SQL_NTS,
		       SQL_INDEX_UNIQUE,
		       SQL_ENSURE);
  SPRINTF(buffer, " SQLStatistics SQL_INDEX_UNIQUE, SQL_ENSURE retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);

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

void fetchout (henv, hdbc, hstmt)
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

  retcode = SQL_SUCCESS;
  while (retcode == SQL_SUCCESS) {
    retcode = SQLFetch (hstmt);
    if (retcode == SQL_SUCCESS_WITH_INFO) {
       SQLError(henv, hdbc, hstmt, state,  &sql_code,
	  error_txt, txt_len, &used_error_txt);
    }
    else {
       if ( retcode == SQL_SUCCESS ) {
	  STRCPY(szOWNER, "*****");
	  retcode = SQLGetData (hstmt, 2, SQL_C_DEFAULT, szOWNER,
	     sizeof(szOWNER), &cbOWNER);

	  STRCPY(szTABLENAME, "*****");
	  retcode = SQLGetData (hstmt, 3, SQL_C_DEFAULT, szTABLENAME,
	     sizeof(szTABLENAME), &cbTABLENAME);

	  sNON_UNIQUE=99;
	  retcode = SQLGetData (hstmt, 4, SQL_C_DEFAULT, &sNON_UNIQUE,
	     sizeof(sNON_UNIQUE), &cbNON_UNIQUE);

	  STRCPY(szINDEX_QUALIFIER, "*****");
	  retcode = SQLGetData (hstmt, 5, SQL_C_DEFAULT, szINDEX_QUALIFIER,
	     sizeof(szINDEX_QUALIFIER), &cbINDEX_QUALIFIER);

	  STRCPY(szINDEX_NAME, "*****");
	  retcode = SQLGetData (hstmt, 6, SQL_C_DEFAULT, szINDEX_NAME,
	     sizeof(szINDEX_NAME), &cbINDEX_NAME);

	  sTYPE=99;
	  retcode = SQLGetData (hstmt, 7, SQL_C_DEFAULT, &sTYPE,
	     sizeof(sTYPE), &cbTYPE);

	  STRCPY(szCOLUMNNAME, "*****");
	  retcode = SQLGetData (hstmt, 9, SQL_C_DEFAULT, szCOLUMNNAME,
	     sizeof(szCOLUMNNAME), &cbCOLUMNNAME);

	  STRCPY(szCOLLATION, "**");
	  retcode = SQLGetData (hstmt, 10, SQL_C_DEFAULT, szCOLLATION,
	     sizeof(szCOLLATION), &cbCOLLATION);

	  sCARDINALITY=99;
	  retcode = SQLGetData (hstmt, 11, SQL_C_DEFAULT, &sCARDINALITY,
	     sizeof(sCARDINALITY), &cbCARDINALITY);

	  sPAGES=99;
	  retcode = SQLGetData (hstmt, 12, SQL_C_DEFAULT, &sPAGES,
	     sizeof(sPAGES), &cbPAGES);

	  SPRINTF(buffer," SQLFetch retcode: '%d'"CRLF, retcode);
	  PRINTF(buffer);
	  SPRINTF(buffer,"OWNER = '%s'"CRLF, szOWNER);
	  PRINTF(buffer);
	  SPRINTF(buffer,"TABLENAME = '%s'"CRLF, szTABLENAME);
	  PRINTF(buffer);
	  SPRINTF(buffer, "NON_UNIQUE = '%d'"CRLF, sNON_UNIQUE);
	  PRINTF(buffer);
	  SPRINTF(buffer, "INDEX_QUALIFIER = '%s'"CRLF, szINDEX_QUALIFIER);
	  PRINTF(buffer);
	  SPRINTF(buffer, "INDEX_NAME = '%s'"CRLF, szINDEX_NAME);
	  PRINTF(buffer);
	  SPRINTF(buffer, "TYPE = '%d'"CRLF, sTYPE);
	  PRINTF(buffer);
	  SPRINTF(buffer, "COLUMNNAME = '%s'"CRLF, szCOLUMNNAME);
	  PRINTF(buffer);
	  SPRINTF(buffer, "COLLATION = '%s'"CRLF, szCOLLATION);
	  PRINTF(buffer);
	  SPRINTF(buffer, "CARDINALITY = '%d'"CRLF, sCARDINALITY);
	  PRINTF(buffer);
	  SPRINTF(buffer, "PAGES = '%d'"CRLF, sPAGES);
	  PRINTF(buffer);
       }
       else {
	  SPRINTF(buffer," SQLFetch retcode: %d"CRLF, retcode);
	  PRINTF(buffer);
	  ERROR_CHECK(hstmt);
       }
    }
  }
}
