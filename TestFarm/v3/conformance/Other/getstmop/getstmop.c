/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : getstmop.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGetStmtOption
**
** Date         : 08.12.94
** l.aenderung  : 14:56pm 03-14-1996 (Harun)
****************************************************************************



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

/* ---------------------------------------------------------------------- */
#define PROGNAME "getstmop"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ODBCUSER1  "ODBC1"
#define ODBCPASSW1 "ODBC1"
#define ODBCTABLE  "ODBCTEST"
#define ODBCSCRIPT "proc2inst"
#define DBPROC1	   "INSERT_TEST"
#define DBPROC2	   "SELECT_TEST"

#define ERROR_CHECK(hstmtx)  \
    if (rc != SQL_SUCCESS) {\
	   SQLError(henv, hdbc, hstmtx, sqlstate, &esq_sql_code, error_txt,\
		    len_error_txt, &used_error_txt);\
	   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
           PRINTF(buffer);\
	   SPRINTF(buffer," (%ld):%s"CRLF, esq_sql_code, error_txt);\
           PRINTF(buffer);\
           switch (rc) {\
		  case(1) :\
			  break;\
		  case(-1) :\
			  break;\
		  case(100) :\
			  break;\
		  default   :\
			  exit(16); \
			  break; } \
	}

 UCHAR sqlstate  [ 10 ];
 SDWORD esq_sql_code;
 UCHAR error_txt  [ 101 ] ;
 SWORD len_error_txt = 100;
 SWORD used_error_txt;
 UCHAR buffer[1024];
/* ----------------------------------------------------------------------- */

#define TESTNAME "SQLGetStmtOptions"
#define TABLE "HARUN.DATATYPE"

#define MAX_NAME_LEN 512
#define MAX_RGB_VALUE 30

#define STR_LEN 128+1
#define REM_LEN 254+1

/* ---------------------------------------------------------------------- */
/* SQLGetStmtOption, SQLSetStmtOption Parameters : */
/* ---------------------------------------------------------------------- */
/*
		1. SQL_ASYNC_ENABLE
		2. SQL_BIND_TYPE
		3. SQL_CONCURRENCY (ODBC 2.0)
		4. SQL_CURSOR_TYPE (ODBC 2.0)
		5. SQL_KEYSET_SIZE (ODBC 2.0)
		6. SQL_MAX_LENGTH
		7. SQL_MAX_ROWS
		8. SQL_NOSCAN
		9. SQL_QUERY_TIMEOUT
	       10. SQL_RETRIEVE_DATA (ODBC 2.0)
	       11. SQL_ROWSET_SIZE (ODBC 2.0)
	       12. SQL_SIMULATE_CURSOR (ODBC 2.0)
	       13. SQL_USE_BOOKMARKS (ODBC 2.0)

   Only SQLGetStmtOption:
		1. SQL_GET_BOOKMARK (ODBC 2.0)
		2. SQL_ROW_NUMBER (OBDC 2.0)
*/
/* ---------------------------------------------------------------------- */

/*
 -------------------------------------------------------------------------
| DoSQLGetStmtOption:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 -------------------------------------------------------------------------
*/
int main(int argc, argv_t argv)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	HENV henv   = SQL_NULL_HENV;
	HENV FAR * phenv;
	HDBC hdbc   = SQL_NULL_HDBC;
	HDBC FAR * phdbc;
	HSTMT hstmt = SQL_NULL_HSTMT;
	HSTMT FAR * phstmt;

/* ---------------------------------------------------------------------- */
	UCHAR                   select[MAX_NAME_LEN];
	UCHAR                   tabname[MAX_NAME_LEN];
	UCHAR                   buf[MAX_NAME_LEN];

	UWORD                   fOption;
	SQLUINTEGER             pvParam;
	UDWORD                  pPar;
	UCHAR                   pvParamChar[MAX_RGB_VALUE];

	SWORD                   cAbort=0;
/* ---------------------------------------------------------------------har- */

 ProcessCmdArguments(argc,argv);

 SPRINTF(buffer, " %s running.\n", PROGNAME);
 PRINTF(buffer);

 phenv = (HENV FAR *) &henv;
 phdbc = (HDBC FAR *) &hdbc;
 phstmt = (HSTMT FAR *) &hstmt;

 rc = SQLAllocEnv(phenv);
 SPRINTF(buffer," SQLAllocEnv retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocConnect(henv, phdbc);
 SPRINTF(buffer," SQLAllocConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLConnect(hdbc, DBNODE, SQL_NTS, ODBCUSER, SQL_NTS,
 						ODBCPASSW, SQL_NTS );
 SPRINTF(buffer," SQLConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocStmt (hdbc, phstmt);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

/* --- SQLGetStmtOption --------------------------------------------------- */
	/* *** 1. SQL_ASYNC_ENABLE */
	fOption = SQL_ASYNC_ENABLE; STRCPY(buffer,"\0");
	SPRINTF(buffer,"1. SQL_ASYNC_ENABLE : "CRLF);
	PRINTF(buffer);
	rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	  case SQL_ASYNC_ENABLE_OFF : STRCPY(buf,"SQL_ASYNC_ENABLE_OFF"); break;
	  case SQL_ASYNC_ENABLE_ON  : STRCPY(buf,"SQL_ASYNC_ENABLE_ON"); break;
	  default                   : STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *** 2. SQL_BIND_TYPE */
	fOption = SQL_BIND_TYPE;
	SPRINTF(buffer,"2. SQL_BIND_TYPE : "CRLF);
	PRINTF(buffer);
	rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 3. SQL_CONCURRENCY (ODBC 2.0) */
	fOption = SQL_CONCURRENCY; STRCPY(buffer,"\0");
	SPRINTF(buffer,"3. SQL_CONCURRENCY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_CONCUR_READ_ONLY : STRCPY(buf,"SQL_CONSUR_READ_ONLY"); break;
	 case SQL_CONCUR_LOCK      : STRCPY(buf,"SQL_CONSUR_LOCK"); break;
	 case SQL_CONCUR_ROWVER    : STRCPY(buf,"SQL_CONSUR_ROWCUR"); break;
	 case SQL_CONCUR_VALUES    : STRCPY(buf,"SQL_CONSUR_VALUES"); break;
	 default                   : STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *** 4. SQL_CURSOR_TYPE (ODBC 2.0) */
	fOption = SQL_CURSOR_TYPE; STRCPY(buffer,"\0");
	SPRINTF(buffer,"4. SQL_CURSOR_TYPE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_CURSOR_FORWARD_ONLY   : STRCPY(buf,"SQL_CURSOR_FORWARD_ONLY"); break;
	 case SQL_CURSOR_STATIC		: STRCPY(buf,"SQL_CURSOR_STATIC"); break;
	 case SQL_CURSOR_KEYSET_DRIVEN  : STRCPY(buf,"SQL_CURSOR_KEYSET_DRIVEN"); break;
	 case SQL_CURSOR_DYNAMIC	: STRCPY(buf,"SQL_CURSOR_DYNAMIC"); break;
	 default			: STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *** 5. SQL_KEYSET_SIZE (ODBC 2.0) */
	fOption = SQL_KEYSET_SIZE;
	SPRINTF(buffer,"5. SQL_KEYSET_SIZE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 6. SQL_MAX_LENGTH */
	fOption = SQL_MAX_LENGTH;
	SPRINTF(buffer,"6. SQL_MAX_LENGTH : "CRLF);
	PRINTF(buffer);
	rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 7. SQL_MAX_ROWS */
	fOption = SQL_MAX_ROWS;
	SPRINTF(buffer,"7. SQL_MAX_ROWS : "CRLF);
	PRINTF(buffer);
	rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 8. SQL_NOSCAN */
	fOption = SQL_NOSCAN; STRCPY(buffer,"\0");
	SPRINTF(buffer,"8. SQL_NOSCAN : "CRLF);
	PRINTF(buffer);
	rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_NOSCAN_OFF : STRCPY(buf,"SQL_NOSCAN_OFF"); break;
	 case SQL_NOSCAN_ON  : STRCPY(buf,"SQL_NOSCAN_ON"); break;
	 default             : STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *** 9. SQL_QUERY_TIMEOUT */
	fOption = SQL_QUERY_TIMEOUT;
	SPRINTF(buffer,"9. SQL_QUERY_TIMEOUT : "CRLF);
	PRINTF(buffer);
	rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 10. SQL_RETRIEVE_DATA (ODBC 2.0) */
	fOption = SQL_RETRIEVE_DATA; STRCPY(buffer,"\0");
	SPRINTF(buffer,"10. SQL_RETRIEVE_DATA : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_RD_OFF : STRCPY(buf,"SQL_RD_OFF"); break;
	 case SQL_RD_ON  : STRCPY(buf,"SQL_RD_ON"); break;
	 default         : STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *** 11. SQL_ROWSET_SIZE (ODBC 2.0) */
	fOption = SQL_ROWSET_SIZE;
	SPRINTF(buffer,"11. SQL_ROWSET_SIZE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 12. SQL_SIMULATE_CURSOR (ODBC 2.0) */
	fOption = SQL_SIMULATE_CURSOR; STRCPY(buffer,"\0");
	SPRINTF(buffer,"12. SQL_SIMULATE_CURSOR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_SC_NON_UNIQUE : STRCPY(buf,"SQL_SC_NON_UNIQUE"); break;
	 case SQL_SC_TRY_UNIQUE : STRCPY(buf,"SQL_SC_TRY_UNIQUE"); break;
	 case SQL_SC_UNIQUE     : STRCPY(buf,"SQL_SC_UNIQUE"); break;
	 default                : STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *** 13. SQL_USE_BOOKMARKS (ODBC 2.0) */
	fOption = SQL_USE_BOOKMARKS; STRCPY(buffer,"\0");
	SPRINTF(buffer,"13. SQL_USE_BOOKMARKS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	pPar = (int) pvParam;
	switch (pPar) {
	 case SQL_UB_OFF : STRCPY(buf,"SQL_UB_OFF"); break;
	 case SQL_UB_ON  : STRCPY(buf,"SQL_UB_ON"); break;
	 default         : STRCPY(buf,"!!!");
	}
	SPRINTF(buffer,"Value = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* ***  14. SQL_GET_BOOKMARK (ODBC 2.0) */
	/* set SQL USE_BOOKMARKS=SQL_UB_ON */
	SPRINTF(buffer,"Set SQL_USE_BOOKMARKS : SQL_UB_ON"CRLF);
	PRINTF(buffer);
        rc = SQLSetStmtOption(hstmt, SQL_USE_BOOKMARKS, SQL_UB_ON);
	SPRINTF(buffer," SQLSetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	rc = SQLExecDirect(hstmt, "select * from dual union all select * from dual", SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);

	rc = SQLFetch(hstmt);
	SPRINTF(buffer," SQLFetch retcode: %d"CRLF, rc);
	PRINTF(buffer);

	rc = SQLFetch(hstmt);
	SPRINTF(buffer," SQLFetch retcode: %d"CRLF, rc);
	PRINTF(buffer);

	fOption = SQL_GET_BOOKMARK;
	SPRINTF(buffer,"14. SQL_GET_BOOKMARK : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

	/* *** 15. SQL_ROW_NUMBER (OBDC 2.0) */
	fOption = SQL_ROW_NUMBER;
	SPRINTF(buffer,"15. SQL_ROW_NUMBER : "CRLF);
	PRINTF(buffer);
        rc = SQLGetStmtOption(hstmt, fOption, (PTR) &pvParam);
	SPRINTF(buffer," SQLGetStmtOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
	SPRINTF(buffer,"Value = '%d'"CRLF,pvParam);
	PRINTF(buffer);

/* ------------------------------------------------------------------------- */
  rc = SQLFreeStmt(hstmt, SQL_DROP);
  SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  rc = SQLDisconnect(hdbc);
  SPRINTF(buffer," SQLDisconnect retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  rc = SQLFreeConnect(hdbc);
  SPRINTF(buffer," SQLFreeConnect retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  rc = SQLFreeEnv(henv);
  SPRINTF(buffer," SQLFreeEnv retcode: %d"CRLF, rc);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
/* ------------------------------------------------------------------------- */
