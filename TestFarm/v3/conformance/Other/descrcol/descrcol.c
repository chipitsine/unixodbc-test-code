/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : descrcol.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLDescribeCol
**
** Date         : 27.11.95
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

/* ----------------------------------------------------------------------- */
#define PROGNAME "descrcol"
#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"

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

#define TESTNAME "SQLDESCRIBECOL"

#define TABLE "DESCRIBECOL"

#define MAX_NAME_LEN 		512
#define MAX_COL_NAME_LEN 	18

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

/* ---------------------------------------------------------------------- */
RETCODE lst_ColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt, SWORD outcol);
/*
 ----------------------------------------------------------------------------
| DoSQLDescribeCol:
|
|
| Returns:
|
 ---------------------------------------------------------------------------
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
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   select[(MAX_NAME_LEN+1)*2];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	SWORD  cAbort=0;
/* ------------------------------------------------------------------har- */

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

	/* --- Create Table  ------------------------------------------ */
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,TABLE);
	STRCAT(create," (\"COL1 TEST\" CHAR(20), \"COL2    TEST\" CHAR(20), ");
	STRCAT(create,"\"COL3   POS   TEST\" CHAR(20), ");
	STRCAT(create,"\"C\" CHAR(10), \"COL5\" CHAR(10)) ");
	SPRINTF(buffer,"Create Stmt (Table: "TABLE")= '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Select Table  ------------------------------------------ */
	STRCPY(select,"SELECT * FROM ");
	STRCAT(select,TABLE);
	SPRINTF(buffer,"Select Stmt (Table: "TABLE")= '%s'"CRLF,select);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, select, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

/* ----------------------------------------------------------------------- */
	/* *************************************************************** */
	/* *** I. SQLDescribeCol ***************************************** */
	/* *************************************************************** */
	STRCPY(buffer,"Describe Columns (Table: "TABLE") : "CRLF);
	PRINTF(buffer);

	rc = lst_ColumnNames(henv, hdbc, hstmt, 5);

	/* --- Drop Table ----------------------------------------------- */
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,TABLE);
	SPRINTF(buffer,"Drop Stmt (Table:"TABLE")= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

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

  SPRINTF(buffer,""CRLF);
  PRINTF(buffer);
  SPRINTF(buffer," %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);

}
/* ---------------------------------------------------------------------- */
RETCODE lst_ColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt, SWORD outcol)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	UWORD			icol;
	UCHAR                   szColName[MAX_NAME_LEN];
	SWORD                   cbColNameMax;
	SWORD                   pcbColName;
	SWORD			pfSqlType;
	SQLULEN			pcbColDef;
	SWORD			pibScale;
	SWORD			pfNullable;
/* ----------------------------------------------------------------------- */
	STRCPY(buffer,"Columns -> : "CRLF);
	PRINTF(buffer);

	icol = 1; cbColNameMax = MAX_COL_NAME_LEN;
	while (icol <= outcol) {
		rc = SQLDescribeCol(hstmt, icol, szColName, cbColNameMax,
					&pcbColName, &pfSqlType, &pcbColDef,
					&pibScale, &pfNullable);
		SPRINTF(buffer, "%d. (%d) :	%s"CRLF,
			 icol, pcbColName, szColName);
		PRINTF(buffer);
		icol++;
	}

	return(rc);
}
/* ----------------------------------------------------------------------- */
