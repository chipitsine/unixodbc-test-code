/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : getconop.c.OC
**
** Purpose      : Test von SQLGetConnectOption
**
** Date         : 11.11.94
** l.aenderung  : 14:19pm 03-14-1996 (Harun)
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
#define PROGNAME "getconop"
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
	   SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
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
/* ---------------------------------------------------------------------- */

#define TESTNAME "SQLGETCONNECTOPT"
#define TABLE TESTNAME

#define MAX_NAME_LEN 512
#define STRING_LEN 10
#define CHAR_LEN 120

/*
 --------------------------------------------------------------------------
| DoGetConnectOption:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 --------------------------------------------------------------------------
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

/* ----------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   buf[MAX_NAME_LEN+1];

	SWORD                   cAbort=0;


	UDWORD 			pAccessMode,
				pAutoCommit,
				pLoginTimeout,
				pOdbcCursors,
				pOptTrace,
				pPacketSize,
				pQuietMode,
				pTranslateOption,
				pTxnIsolation;

 	UCHAR			pCurrentQualifier[CHAR_LEN],
				pOptTraceFile[CHAR_LEN],
				pTranslateDLL[CHAR_LEN];

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

/* ---------------------------------------------------------------------- */
	/* --- GetConnectOption ----------------------------------------- */
	/* *** SQL_ACCESS_MODE --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_ACCESS_MODE, &pAccessMode);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_AUTOCOMMIT --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_AUTOCOMMIT, &pAutoCommit);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_CURRENT_QUALIFIER --------------- *** */
	/* *** ODBC 2.0 */
	rc = SQLGetConnectOption( hdbc, SQL_CURRENT_QUALIFIER, pCurrentQualifier);
	rc = SQL_SUCCESS;
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_LOGIN_TIMEOUT --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_LOGIN_TIMEOUT, &pLoginTimeout);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_ODBC_CURSORS --------------- *** */
   	/* *** ODBC 2.0 */
	rc = SQLGetConnectOption(hdbc, SQL_ODBC_CURSORS, &pOdbcCursors);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_OPT_TRACE --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_OPT_TRACE, &pOptTrace);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_TRACEFILE --------------- *** */
        pOptTraceFile[0] = '\0';
	rc = SQLGetConnectOption( hdbc, SQL_OPT_TRACEFILE, pOptTraceFile);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_PACKET_SIZE --------------- *** */
	/* *** ODBC 2.0 */
	rc = SQLGetConnectOption( hdbc, SQL_PACKET_SIZE, &pPacketSize);
	rc = SQL_SUCCESS;
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_QUIET_MODE --------------- *** */
	/* *** ODBC 2.0 */
	rc = SQLGetConnectOption( hdbc, SQL_QUIET_MODE, &pQuietMode);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_TRANSLATE_DLL --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_TRANSLATE_DLL, pTranslateDLL);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_TRANSLATE_OPTION --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_TRANSLATE_OPTION, &pTranslateOption);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* *** SQL_TXN_ISOLATION --------------- *** */
	rc = SQLGetConnectOption( hdbc, SQL_TXN_ISOLATION, &pTxnIsolation);
	SPRINTF(buffer," SQLGetConnectOption retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

	/* --- Output ConnectOptons ---------------------------------------- */

	/* *** SQL_ACCESS_MODE --------------- *** */
	switch (pAccessMode) {
	 case (SQL_MODE_READ_ONLY): STRCPY(buf,"SQL_MODE_READ_ONLY"); break;
	 case (SQL_MODE_READ_WRITE): STRCPY(buf,"SQL_MODE_READ_WRITE"); break;
        }
	SPRINTF(buffer," SQL_ACCESS_MODE	: '%s' "CRLF, buf);
	PRINTF(buffer);

	/* *** SQL_AUTOCOMMIT --------------- *** */
	switch (pAutoCommit) {
	 case (SQL_AUTOCOMMIT_ON): STRCPY(buf,"SQL_AUTOCOMMIT_ON"); break;
	 case (SQL_AUTOCOMMIT_OFF): STRCPY(buf,"SQL_AUTOCOMMIT_OFF"); break;
        }
	SPRINTF(buffer," SQL_AUTOCOMMIT		: '%s' "CRLF, buf);
	PRINTF(buffer);

	/* *** SQL_CURRENT_QUALIFIER --------------- *** */
	/* %s - pCurrentQualifier */
	SPRINTF(buffer," SQL_CURRENT_QUALIFIER	: '-' "CRLF);
	PRINTF(buffer);

	/* *** SQL_LOGIN_TIMEOUT --------------- *** */
	SPRINTF(buffer," SQL_LOGIN_TIMEOUT	: '%d' "CRLF, pLoginTimeout);
	PRINTF(buffer);

	/* *** SQL_ODBC_CURSORS --------------- *** */
	/* %d - pOdbcCursors */
	switch(pOdbcCursors) {
	 case (SQL_CUR_USE_IF_NEEDED): STRCPY(buf,"SQL_CUR_USE_IF_NEEDED");break;
	 case (SQL_CUR_USE_ODBC): STRCPY(buf,"SQL_CUR_USE_ODBC"); break;
	 case (SQL_CUR_USE_DRIVER) : STRCPY(buf,"SQL_CUR_USE_DRIVER"); break;
        }
	SPRINTF(buffer," SQL_ODBC_CURSORS	: '%s' "CRLF,buf);
	PRINTF(buffer);

	/* *** SQL_OPT_TRACE --------------- *** */
	switch (pOptTrace) {
	 case (SQL_OPT_TRACE_ON): STRCPY(buf,"SQL_OPT_TRACE_ON"); break;
	 case (SQL_OPT_TRACE_OFF): STRCPY(buf,"SQL_OPT_TRACE_OFF"); break;
        }
	SPRINTF(buffer," SQL_OPT_TRACE		: '%s' "CRLF, buf);
	PRINTF(buffer);

	/* *** SQL_TRACEFILE --------------- *** */
	SPRINTF(buffer," SQL_OPT_TRACEFILE	: GREPWEG('%s') "CRLF, pOptTraceFile);
	PRINTF(buffer);

	/* *** SQL_PACKET_SIZE --------------- *** */
	/* %d - pPacketSize */
	SPRINTF(buffer," SQL_PACKET_SIZE	: '-' "CRLF);
	PRINTF(buffer);

	/* *** SQL_QUIET_MODE --------------- *** */
	/* %d - pQuietMode, %p is different on 32, 64 Bit */
	SPRINTF(buffer," SQL_QUIET_MODE		: '%d' "CRLF,pQuietMode);
	PRINTF(buffer);

	/* *** SQL_TRANSLATE_DLL --------------- *** */
	/* %s - pTranslateDLL */
	SPRINTF(buffer," SQL_TRANSLATE_DLL	: '%s' "CRLF,pTranslateDLL);
	PRINTF(buffer);

	/* *** SQL_TRANSLATE_OPTION --------------- *** */
	/* %d - pTranslateOption */
	SPRINTF(buffer," SQL_TRANSLATE_OPTION	: '%d' "CRLF,pTranslateOption);
	PRINTF(buffer);

	/* *** SQL_TXN_ISOLATION --------------- *** */
	/* %d - pTxnIsolation */
	switch (pTxnIsolation) {
	 case (SQL_TXN_READ_UNCOMMITTED) : STRCPY(buf,"SQL_TXN_READ_UNCOMMITTED"); break;
	 case (SQL_TXN_READ_COMMITTED) : STRCPY(buf,"SQL_TXN_READ_COMMITTED"); break;
	 case (SQL_TXN_REPEATABLE_READ) : STRCPY(buf,"SQL_TXN_REPEATABLE_READ"); break;
	 case (SQL_TXN_SERIALIZABLE) : STRCPY(buf,"SQL_TXN_SERIALIZABLE"); break;
         /*case (SQL_TXN_VERSIONING) : STRCPY(buf,"SQL_TXN_VERSIONING"); break;*/
	}
	SPRINTF(buffer," SQL_TXN_ISOLATION	: '%s' "CRLF,buf);
	PRINTF(buffer);

	/* --- SQLSetConnectOption ----------------------------------------- */
	/*
	if (vAutoCommit == SQL_AUTOCOMMIT_ON) {
		vAutoCommit =  SQL_AUTOCOMMIT_OFF;  (o. SQL_AUTOCOMMIT_ON)
		rc = SQLSetConnectOption( hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
		SPRINTF(buffer," SQLSetConnectOption retcode: %d"CRLF, rc);
		PRINTF(buffer);
		ERROR_CHECK(hdbc);
	}
	*/
  /* --- Disconnect -------------------------------------------------- */
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
/* ************************************************************************* */
