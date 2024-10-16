/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : scrollop.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLSetScrollOptions
**
** Date         : 30.11.95
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
#define PROGNAME "scrollop"
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
                        /* exit(16);*/ \
			  break; } \
	}

 UCHAR sqlstate  [ 10 ];
 SDWORD esq_sql_code;
 UCHAR error_txt  [ 101 ] ;
 SWORD len_error_txt = 100;
 SWORD used_error_txt;
 UCHAR buffer[1024];
/* ----------------------------------------------------------------------- */

#define TESTNAME "SQLSetScrollOptions"
#define PREPARESEL "select * from sysodbctypes"

#define MAX_NAME_LEN 512
#define MAX_RGB_VALUE 30

#define STR_LEN 128+1
#define REM_LEN 254+1

/*
 -------------------------------------------------------------------------
| DoSQLSetScrollOptions:
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
	UCHAR                   buf[MAX_NAME_LEN];

	UWORD                   fConcurrency;
	SDWORD			crowKeyset;
	UWORD			crowRowset;

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

/* --- SQLSetScrollOptions --------------------------------------------------- */
	/* *** 1. SQLSetScrollOptions -> Set defaults */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 1;
	STRCPY(buffer,"1. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 2. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_STATIC; crowRowset = 5;
	STRCPY(buffer,"2. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_STATIC "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 3. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_KEYSET_DRIVEN; crowRowset = 10;
	STRCPY(buffer,"3. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_KEYSET_DRIVEN "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 4. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_DYNAMIC; crowRowset = 5;
	STRCPY(buffer,"4. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_DYNAMIC "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 5. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_LOCK;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	STRCPY(buffer,"5. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_LOCK "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 6. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_ROWVER;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	STRCPY(buffer,"6. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_ROWVER "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 7. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_VALUES;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	STRCPY(buffer,"7. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_VALUES "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 8. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 0;
	STRCPY(buffer,"8. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d"CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 9. SQLSetScrollOptions */
	fConcurrency = 99;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	STRCPY(buffer,"9. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d "CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 10. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = 999; crowRowset = 5;
	STRCPY(buffer,"10. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d "CRLF,crowRowset);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** 11. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	STRCPY(buffer,"11. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d  "CRLF,crowRowset);
	STRCAT(buf,   "		state	     = API_STMT_PREPARED"CRLF);
	PRINTF(buffer);PRINTF(buf);

	rc = SQLPrepare(hstmt, PREPARESEL, sizeof(PREPARESEL)-1);
	SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	if (rc == SQL_SUCCESS) {
	   rc = SQLSetScrollOptions(hstmt, fConcurrency, crowKeyset, crowRowset);
	   SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	   PRINTF(buffer);
	   ERROR_CHECK(hstmt);
	}

	/* *** 12. SQLSetScrollOptions */
	fConcurrency = SQL_CONCUR_READ_ONLY;
	crowKeyset = SQL_SCROLL_FORWARD_ONLY; crowRowset = 5;
	STRCPY(buffer,"12. SQLSetScrollOptions -> \n ");
	STRCAT(buffer,"		fConcurrency = SQL_CONCUR_READ_ONLY "CRLF);
	STRCAT(buffer,"		crowKeyset   = SQL_SCROLL_FORWARD_ONLY "CRLF);
	SPRINTF(buf,  "		crowRowset   = %d "CRLF,crowRowset);
	STRCAT(buf,   "		HSTMT !!"CRLF);
	PRINTF(buffer);PRINTF(buf);
	rc = SQLSetScrollOptions(NULL, fConcurrency, crowKeyset, crowRowset);
	SPRINTF(buffer," SQLSetScrollOptions retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

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
