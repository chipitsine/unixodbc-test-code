/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : floatnmd.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test von DBS-FLOAT Datentype
**
** Date         : 01.09.94
** l.aenderung  : 14:06pm 03-14-1996 (Harun)
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
#define PROGNAME "floatnmd"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define ERROR_CHECK(hstmtx)      if (rc != SQL_SUCCESS) {\
			   SQLError(henv,\
				    hdbc,\
				    hstmtx,\
				    sqlstate,\
				    &esq_sql_code,\
				    error_txt,\
				    len_error_txt,\
				    &used_error_txt);\
			        SPRINTF(buffer, " sqlstate: %s\n", sqlstate);\
		                PRINTF(buffer);\
				SPRINTF(buffer," esq_sql_code: %ld\n", esq_sql_code);\
		                PRINTF(buffer);\
                  switch (rc) {\
		  case(100) :\
			  break;\
		  default   :\
		          if (esq_sql_code != -6000) \
			  exit(16);\
			  break; } }

 UCHAR sqlstate  [ 10 ];
 SDWORD esq_sql_code;
 UCHAR error_txt  [ 101 ] ;
 SWORD len_error_txt = 100;
 SWORD used_error_txt;
 UCHAR buffer[1024];
/* ----------------------------------------------------------------------- */

#define TESTNAME "FLOATNUMBERS"

#define TABLE	"FLOATNUM"
/* ------------------------------------------------------------------------ */

#define MAX_NAME_LEN 512
#define ERROR_TEXT_LEN 511

#define STRING_LEN 10

/*
 ----------------------------------------------------------------------------
| DoFloatNumbers:
|     This test case will test the fixed numbers write and read from the
|     database.
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 ----------------------------------------------------------------------------
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
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN+1];

	SDOUBLE                 num;
	SQLLEN                  cbnum=0;
	UCHAR                   buf[MAX_NAME_LEN+1];
	int                     i;
/* -------------------------------------------------------------------har- */

 ProcessCmdArguments(argc,argv);

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

 rc = SQLConnect(hdbc, DBNODE, SQL_NTS, ODBCUSER, SQL_NTS, ODBCPASSW, SQL_NTS );
 SPRINTF(buffer," SQLConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocStmt (hdbc, phstmt);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

/* -------------------------------------------------------------------har- */

	/* --- Create Table --------------------------------------------- */
	STRCPY(tabname,TABLE);
	STRCPY(create,"CREATE TABLE "); STRCAT(create,tabname);
	STRCAT(create," ( \"NUMBERS\" FLOAT(15) )");
	SPRINTF(buffer,"Create Stmt = '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	STRCPY(insert,"INSERT INTO ");STRCAT(insert,tabname);
	STRCAT(insert," VALUES (?)");
	SPRINTF(buffer,"Insert Stmt = '%s'"CRLF,insert);
	PRINTF(buffer);

	rc = SQLPrepare(hstmt, insert, SQL_NTS);
	SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE,
				SQL_DOUBLE, 0, 0, &num, 0, &cbnum);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	num=0;i=0;
	while (i < 32) {
		SPRINTF(buf,"%.0f",num);
		SPRINTF(buffer,"Insert Value %d.: ->'%s'"CRLF,i+1,buf);
		PRINTF(buffer);

		cbnum = sizeof(num);

		rc = SQLExecute(hstmt);
		SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
		PRINTF(buffer);
		ERROR_CHECK(hstmt);

		if (i < 16)  num = (num * 10) + 1;
			else num = (num * 10) - 1;
		i++;
		if (i == 16) num = 0;
	}

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */

	STRCPY(select,"SELECT \"NUMBERS\" FROM ");
	STRCAT(select,tabname);
	SPRINTF(buffer,"Select Stmt= '%s'"CRLF,select);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, select, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	rc = SQLBindCol(hstmt, 1, SQL_C_DOUBLE, &num, 0, NULL);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	i=0;
	while (1) {
		rc = SQLFetch(hstmt);
		SPRINTF(buffer," SQLFetch retcode: %d"CRLF, rc);
		PRINTF(buffer);
		ERROR_CHECK(hstmt);

		if (rc == SQL_NO_DATA_FOUND) break;

		SPRINTF(buf,"%.0f",num);
		SPRINTF(buffer,"Select Values %d.: ->'%s'"CRLF,i+1,buf);
		PRINTF(buffer);
		i++;
	}
	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Drop Table ----------------------------------------------- */
	STRCPY(drop,"DROP TABLE ");STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	rc = SQLExecDirect(hstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
/* ---------------------------------------------------------------------har- */
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

/* ---------------------------------------------------------------------har- */
