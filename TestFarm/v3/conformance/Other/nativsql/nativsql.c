/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : natisql.c.OC
**
** Usage	: --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLNativeSql
**
** Date 	: 30.06.95
** l.aenderung  : 13:27pm 03-14-1996 (Harun)
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
#define PROGNAME "nativsql"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)  \
    if (rc != SQL_SUCCESS) {\
	   SQLError(henv, hdbc, hstmtx, sqlstate, &esq_sql_code, error_txt,\
		    len_error_txt, &used_error_txt);\
	   SPRINTF(buffer," sqlstate: %s"CRLF, sqlstate);\
           PRINTF(buffer);\
	   SPRINTF(buffer," esq_sql_code: %ld"CRLF, esq_sql_code);\
           PRINTF(buffer);\
           switch (rc) {\
	   	  case (1) : \
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

#define TESTNAME "SQLNativeSql"

#define MAX_NAME_LEN 512

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

/* ---------------------------------------------------------------------- */

/*
  -----------------------------------------------------------------------
 | DoSQLNativeSql:
 |
 |
 | Returns:
 |
  -----------------------------------------------------------------------
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
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR 		        colname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

		/* Declare storage locations for result set data */
	UCHAR 	szSqlStrIn[STR_LEN],
		szSqlStr[STR_LEN];

	SDWORD 	pcbSqlStr;

	SDWORD 	cbSqlStrIn,
		cbSqlStrMax;

	SWORD   cAbort=0;
/* -------------------------------------------------------------------har- */

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

	/* *************************************************************** */
	/* *** I. SQLNativSql ******************************************** */
	/* *************************************************************** */
	SPRINTF(buffer,"I.) SQLNativeSql -> "CRLF);
	PRINTF(buffer);

	STRCPY(szSqlStrIn, "CREATE TABLE TEST (KEY INTEGER, NAME CHAR(30))");
	cbSqlStrMax = STRLEN(szSqlStrIn) + 1;
	cbSqlStrIn = SQL_NTS;
	SPRINTF(buffer,"IN=> SqlStrIn   : '%s'"CRLF,szSqlStrIn);
	PRINTF(buffer);
	SPRINTF(buffer,"     cbSqlStrIn : %d "CRLF,cbSqlStrIn);
	PRINTF(buffer);
	SPRINTF(buffer,"     cbSqlStrMax: %d "CRLF, cbSqlStrMax);
	PRINTF(buffer);

	rc = SQLNativeSql(hdbc, szSqlStrIn, cbSqlStrIn, szSqlStr,
				cbSqlStrMax, &pcbSqlStr);
	SPRINTF(buffer," SQLNativeSql retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"OUT => SqlStr    : '%s'"CRLF,szSqlStr);
	PRINTF(buffer);
	SPRINTF(buffer,"       pcbSqlStr : %d '"CRLF,pcbSqlStr);
	PRINTF(buffer);

	/* *************************************************************** */
	/* *** II. SQLNativSql ******************************************* */
	/* *************************************************************** */
	SPRINTF(buffer,"II.) SQLNativeSql -> \
					(Data trucated)'"CRLF);
	PRINTF(buffer);

	STRCPY(szSqlStrIn, "CREATE TABLE TEST (KEY INTEGER, NAME CHAR(30))");
	cbSqlStrMax = STRLEN(szSqlStrIn) - 5;
	cbSqlStrIn = SQL_NTS;
	SPRINTF(buffer,"IN=> SqlStrIn   : %s'"CRLF,szSqlStrIn);
	PRINTF(buffer);
	SPRINTF(buffer,"     cbSqlStrIn : %d "CRLF,cbSqlStrIn);
	PRINTF(buffer);
	SPRINTF(buffer,"     cbSqlStrMax: %d "CRLF, cbSqlStrMax);
	PRINTF(buffer);

	rc = SQLNativeSql(hdbc, szSqlStrIn, cbSqlStrIn, szSqlStr,
				cbSqlStrMax, &pcbSqlStr);
	SPRINTF(buffer," SQLNativeSql retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"OUT => SqlStr    : '%s'"CRLF,szSqlStr);
	PRINTF(buffer);
	SPRINTF(buffer,"       pcbSqlStr : %d '"CRLF,pcbSqlStr);
	PRINTF(buffer);

	/* *************************************************************** */
	/* *** III. SQLNativSql ****************************************** */
	/* *************************************************************** */
	SPRINTF(buffer,"III.) SQLNativeSql -> (Error)'"CRLF);
	PRINTF(buffer);

	STRCPY(szSqlStrIn, "CREATE TABLE TEST (KEY INTEGER, NAME CHAR(30))");
	cbSqlStrMax = SQL_NTS;
	cbSqlStrIn = SQL_NTS;
	SPRINTF(buffer,"IN=> SqlStrIn   : %s'"CRLF,szSqlStrIn);
	PRINTF(buffer);
	SPRINTF(buffer,"     cbSqlStrIn : %d "CRLF,cbSqlStrIn);
	PRINTF(buffer);
	SPRINTF(buffer,"     cbSqlStrMax: %d "CRLF, cbSqlStrMax);
	PRINTF(buffer);

	rc = SQLNativeSql(hdbc, szSqlStrIn, cbSqlStrIn, szSqlStr,
				cbSqlStrMax, &pcbSqlStr);
	SPRINTF(buffer," SQLNativeSql retcode: %d"CRLF, rc);
	PRINTF(buffer);
/* -------------------------------------------------------------------har- */
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

  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}
/* ----------------------------------------------------------------------- */
