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
#define PROGNAME "STATGRNT"
#include "tst.h"

#define TABLE PROGNAME
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"
#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif
#define ODBCUSER1  "GRANTUSER"
#define ODBCPASSW1 "PC"


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
				   return(17);\
				   break;\
			  }\
			  break;\
		  default   :\
			  return(16);\
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
SWORD sSEQ_IN_INDEX;
UCHAR szCOLUMNNAME[COLUMNNAME_LEN+1];
UCHAR szCOLLATION[COLLATION_LEN+1];
SDWORD sCARDINALITY;
SDWORD sPAGES;

SQLLEN cbQUALIFIER, cbOWNER, cbTABLENAME, cbNON_UNIQUE, cbINDEX_QUALIFIER,
       cbINDEX_NAME, cbTYPE, cbSEQ_IN_INDEX, cbCOLUMNNAME, cbCOLLATION,
       cbCARDINALITY, cbPAGES;

int fetchout (HENV henv, HDBC hdbc, HSTMT hstmt);

int main(int argc, argv_t argv)
{

  HENV henv   = SQL_NULL_HENV;
  HDBC hdbc   = SQL_NULL_HDBC;
  HDBC hdbc2  = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  HSTMT hstmt2 = SQL_NULL_HSTMT;
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

  SPRINTF(buffer, "%s running."CRLF, PROGNAME);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
                       ODBCUSER1, (SWORD) SQL_NTS, ODBCPASSW1, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  if (retcode == SQL_ERROR)
  {
     SDWORD sql_code;
     SQLError(henv, hdbc, SQL_NULL_HSTMT, NULL, &sql_code, NULL, 0, NULL);
     if (sql_code == -4008) {
	retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
                             USERNAME, (SWORD) SQL_NTS, PASSWORD, (SWORD) SQL_NTS );
        SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
        PRINTF(buffer);
	ERROR_CHECK(NULL);

	retcode = SQLAllocStmt (hdbc, &hstmt);
        SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
        PRINTF(buffer);
	ERROR_CHECK(NULL);

	retcode = SQLExecDirect (hstmt, "create user "ODBCUSER1" password "ODBCPASSW1" dba not exclusive", SQL_NTS);
        SPRINTF(buffer, " SQLExecDirect(create user) retcode: %d"CRLF, retcode);
        PRINTF(buffer);
	ERROR_CHECK(hstmt);

	retcode = SQLFreeStmt(hstmt, SQL_DROP);
        SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
        PRINTF(buffer);
	ERROR_CHECK(hstmt);

	retcode = SQLDisconnect(hdbc);
        SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
        PRINTF(buffer);
	ERROR_CHECK(NULL);

	retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
	   ODBCUSER1, (SWORD) SQL_NTS, ODBCPASSW1, (SWORD) SQL_NTS );
        SPRINTF(buffer, " SQLConnect  retcode: %d"CRLF, retcode);
        PRINTF(buffer);
	ERROR_CHECK(NULL);
     }
  }
  retcode = SQLAllocStmt (hdbc, &hstmt);
  retcode = SQLExecDirect (hstmt, "create table \""TABLE"\" (\""TABLE"\" int)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect(create table) retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  if (retcode == SQL_SUCCESS) {
     retcode = SQLExecDirect (hstmt, "create unique index ind on \""TABLE"\" (\""TABLE"\" asc)", SQL_NTS);
  }
  SPRINTF(buffer, " SQLExecDirect(create index) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer,"grant all on \""TABLE"\" to %s",ODBCUSER);
  retcode = SQLExecDirect (hstmt, buffer, SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect(grant all) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
                       ODBCUSER, (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLStatistics (hstmt, NULL, 0, ODBCUSER1, SQL_NTS,
                           TABLE, SQL_NTS, SQL_INDEX_ALL, SQL_QUICK);
  SPRINTF(buffer, " SQLStatistics retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout(henv, hdbc, hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);

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

  /* drop user */
  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS,
                       USERNAME, (SWORD) SQL_NTS, PASSWORD, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  SPRINTF(buffer, " SQLAllocStmt retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLExecDirect (hstmt, "drop user "ODBCUSER1"", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect(drop user) retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
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

int fetchout (HENV henv, HDBC hdbc, HSTMT hstmt)
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

	  SPRINTF(buffer, " SQLFetch retcode: '%d'"CRLF, retcode);
	  PRINTF(buffer);
	  SPRINTF(buffer, "OWNER = '%s'"CRLF, szOWNER);
	  PRINTF(buffer);
	  SPRINTF(buffer, "TABLENAME = '%s'"CRLF, szTABLENAME);
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
	  SPRINTF(buffer, "CARDINALITY = '%d'"CRLF, (int)sCARDINALITY);
	  PRINTF(buffer);
	  SPRINTF(buffer, "PAGES = '%d'"CRLF, (int)sPAGES);
	  PRINTF(buffer);
       }
       else {
	  SPRINTF(buffer, " SQLFetch retcode: %d"CRLF, retcode);
	  PRINTF(buffer);
	  ERROR_CHECK(hstmt);
       }
    }
  }
  return (retcode);
}
