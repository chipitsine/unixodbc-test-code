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


#define PROGNAME "statstc"

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
			   printf(" sqlstate: %s\n", sqlstate);\
		  printf(" esq_sql_code: %ld\n", esq_sql_code);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
		  default   :\
                     if (STRCMP(sqlstate,"HY009")) \
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
UCHAR buffer [1024];


UCHAR szQUALIFIER[QUALIFIER_LEN];
UCHAR szOWNER[OWNER_LEN];
UCHAR szTABLENAME[TABLENAME_LEN];
SWORD sNON_UNIQUE;
UCHAR szINDEX_QUALIFIER[INDEX_QUALIFIER_LEN];
UCHAR szINDEX_NAME[INDEX_NAME_LEN];
SWORD sTYPE;
SWORD sSEQ_IN_INDEX;
UCHAR szCOLUMNNAME[COLUMNNAME_LEN];
UCHAR szCOLLATION[COLLATION_LEN];
SWORD sCARDINALITY;
SWORD sPAGES;

SQLLEN cbTYPE;

SQLLEN cbQUALIFIER, cbOWNER, cbTABLENAME, cbNON_UNIQUE, cbINDEX_QUALIFIER,
       cbINDEX_NAME, cbSEQ_IN_INDEX, cbCOLUMNNAME, cbCOLLATION,
       cbCARDINALITY, cbPAGES;

void fetchout (HENV henv, HDBC hdbc, HSTMT hstmt);
void makestr( char * str, short lstr, short type, void * val, SDWORD len);

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
  UCHAR error_txt  [ ERROR_TEXT_LEN + 1 ] ;
  SWORD len_error_txt = ERROR_TEXT_LEN;
  SWORD used_error_txt;

  RETCODE retcode;

  ProcessCmdArguments(argc,argv);

  printf(" statstc running.\n\n");

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;
  phstmt2 = (HSTMT FAR *) &hstmt2;

  FullConnect(phenv, phdbc);

  retcode = SQLAllocStmt (hdbc, phstmt);

  retcode = SQLExecDirect (hstmt, "drop table o3initab", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "drop table o3tab1", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create table o3initab (s1 int)", SQL_NTS);
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

  retcode = SQLExecDirect (hstmt, "create unique index o3inda on o3tab1 (s1 asc)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create index o3ind1 on o3tab1 (s2 asc, s3 desc)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "drop table o3tab2", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create table o3tab2 (s1 int, s2 int, s3 int)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create index o3ind1 on o3tab2 (s1 asc, s2 asc)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLExecDirect (hstmt, "create index o3ind2 on o3tab2 (s2 asc, s1 asc)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_ALL,
		       SQL_QUICK);
  printf(" SQLStatistics retcode: %d\n", retcode);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, sizeof(ODBCUSER)-1,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_UNIQUE,
		       SQL_QUICK);
  printf(" SQLStatistics retcode: %d\n", retcode);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", (SWORD)STRLEN("TAB1"),
		       SQL_INDEX_ALL,
		       SQL_ENSURE);
  printf(" SQLStatistics retcode: %d\n", retcode);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       ODBCUSER, SQL_NTS,
		       "O3TAB1", SQL_NTS,
		       SQL_INDEX_UNIQUE,
		       SQL_ENSURE);
  printf(" SQLStatistics retcode: %d\n", retcode);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  retcode = SQLStatistics (hstmt,
	               NULL, 0,
		       NULL, 0,
		       NULL, 0,
		       SQL_INDEX_ALL,
		       SQL_QUICK);
  printf(" SQLStatistics retcode: %d\n", retcode);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
  printf(" SQLFreeStmt retcode: %d\n", retcode);
  ERROR_CHECK(hstmt);

  retcode = SQLTransact(henv, hdbc, SQL_COMMIT);
  printf(" SQLTransact retcode: %d\n", retcode);

  retcode = SQLDisconnect(hdbc);
  printf(" SQLDisconnect retcode: %d\n", retcode);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  printf(" SQLFreeConnect retcode: %d\n", retcode);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  printf(" SQLFreeEnv retcode: %d\n", retcode);
  ERROR_CHECK(NULL);

  printf(" %s ending\n", PROGNAME);

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
  UCHAR local_str[100];
  retcode = SQLBindCol (hstmt, 1, SQL_C_CHAR, szQUALIFIER,
	                sizeof(szQUALIFIER), &cbQUALIFIER);

  retcode = SQLBindCol (hstmt, 2, SQL_C_CHAR, szOWNER,
	                sizeof(szOWNER), &cbOWNER);

  retcode = SQLBindCol (hstmt, 3, SQL_C_CHAR, szTABLENAME,
	                sizeof(szTABLENAME), &cbTABLENAME);

  retcode = SQLBindCol (hstmt, 4, SQL_C_SHORT, &sNON_UNIQUE,
	                sizeof(sNON_UNIQUE), &cbNON_UNIQUE);

  retcode = SQLBindCol (hstmt, 5, SQL_C_CHAR, szINDEX_QUALIFIER,
	                sizeof(szINDEX_QUALIFIER), &cbINDEX_QUALIFIER);

  retcode = SQLBindCol (hstmt, 6, SQL_C_CHAR, szINDEX_NAME,
	                sizeof(szINDEX_NAME), &cbINDEX_NAME);

  retcode = SQLBindCol (hstmt, 7, SQL_C_SHORT, &sTYPE,
	                sizeof(sTYPE), &cbTYPE);

  retcode = SQLBindCol (hstmt, 8, SQL_C_SHORT, &sSEQ_IN_INDEX,
	                sizeof(sSEQ_IN_INDEX), &cbSEQ_IN_INDEX);

  retcode = SQLBindCol (hstmt, 9, SQL_C_CHAR, szCOLUMNNAME,
	                sizeof(szCOLUMNNAME), &cbCOLUMNNAME);

  retcode = SQLBindCol (hstmt, 10, SQL_C_CHAR, szCOLLATION,
	                sizeof(szCOLLATION), &cbCOLLATION);

  retcode = SQLBindCol (hstmt, 11, SQL_C_SHORT, &sCARDINALITY,
	                sizeof(sCARDINALITY), &cbCARDINALITY);

  retcode = SQLBindCol (hstmt, 12, SQL_C_SHORT, &sPAGES,
	                sizeof(sPAGES), &cbPAGES);

  while (retcode == SQL_SUCCESS) {
    retcode = SQLFetch (hstmt);
    if (retcode == SQL_SUCCESS_WITH_INFO) {
      SQLError(henv, hdbc, hstmt, state,  &sql_code,
	       error_txt, txt_len, &used_error_txt);
    }
    if (retcode == SQL_SUCCESS ){
       SPRINTF(buffer," SQLFetch retcode: '%d'\n", retcode);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szQUALIFIER, cbQUALIFIER);
       SPRINTF(buffer,"TABLE_QUALIFIER = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szOWNER, cbOWNER);
       SPRINTF(buffer,"TABLE_OWNER = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szTABLENAME, cbTABLENAME);
       SPRINTF(buffer,"TABLE_NAME = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_SHORT, &sNON_UNIQUE, cbNON_UNIQUE);
       SPRINTF(buffer, "NON_UNIQUE = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szINDEX_QUALIFIER, cbINDEX_QUALIFIER);
       SPRINTF(buffer, "INDEX_QUALIFIER = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szINDEX_NAME, cbINDEX_NAME);
       SPRINTF(buffer, "INDEX_NAME = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_SHORT, &sTYPE, cbTYPE);
       SPRINTF(buffer, "TYPE = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_SHORT, &sSEQ_IN_INDEX, cbSEQ_IN_INDEX);
       SPRINTF(buffer, "SEQ_IN_INDEX = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szCOLUMNNAME, cbCOLUMNNAME);
       SPRINTF(buffer, "COLUMNNAME = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_CHAR, szCOLLATION, cbCOLLATION);
       SPRINTF(buffer, "COLLATION = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_SHORT, &sCARDINALITY, cbCARDINALITY);
       SPRINTF(buffer, "CARDINALITY = '%s'\n", local_str);
       PRINTF(buffer);
       makestr( local_str, sizeof(local_str), SQL_C_SHORT, &sPAGES, cbPAGES);
       SPRINTF(buffer, "PAGES = '%s'\n", local_str);
       PRINTF(buffer);
    }
    else {
      SPRINTF(buffer," SQLFetch retcode: %d\n", retcode);
      PRINTF(buffer);
    }
  }
}

void makestr(char * str,
short lstr,
short type,
void * val,
SDWORD len)
{
#define NULLSTR "(is null)"
#define UNKNOWN "unknown datatype"
   if (lstr > 0) {
      if ( len == SQL_NULL_DATA ) {
         STRNCPY(str, NULLSTR, lstr);
   }
   else {
      switch (type) {
	 case SQL_C_SHORT:
	    SPRINTF(str, "%d", * (short *) val);
	    break;
         case SQL_C_CHAR:
	    SPRINTF(str, "%.*s", len, val);
	    break;
	 default:
            STRNCPY(str, UNKNOWN, STRLEN(UNKNOWN));
	    break;
      }
   }
  }
}
