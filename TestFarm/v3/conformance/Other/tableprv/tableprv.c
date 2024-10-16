/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : tableprv.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test der Funktion SQLTablePrivileges
**
** Date         : 22.06.95
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
#define PROGNAME "tablesprv"
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

#define TESTNAME "SQLTABLEPRIVILEGES"
#define TABLE "TABSQLTABPRIV"

#define USER1 "PUBLIC"
#define USER2 "SYSODBC"

#define MAX_NAME_LEN 		512
#define MAX_COL_NAME_LEN 	18

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

/* ---------------------------------------------------------------------- */
RETCODE lstTablePrivilegesInfo(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE lst_ColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt, SWORD outcol);

/*
 -------------------------------------------------------------------------
| SQLTablePrivileges:
|
|
| Returns:
|
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
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   grant[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];

	/* Declare storage locations for result set data */
	UCHAR   szProcQualifier[STR_LEN],
		szProcOwner[STR_LEN],
	        szProcName[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SDWORD cbProcQualifier, cbProcOwner, cbProcName;

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
/* ---------------------------------------------------------------------- */
	/* --- Create Table  -------------------------------------------- */
	STRCPY(tabname, TABLE);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (CUST_ID INTEGER, CUST_NAME CHAR(30) )");
	SPRINTF(buffer,"Create Stmt (Table: "TABLE")= '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Grant Table 1. (USER1) ------------------------------------- */
	STRCPY(tabname, TABLE);
	STRCPY(grant,"GRANT SELECT, INSERT, UPDATE, ALTER ON ");
	STRCAT(grant,tabname);
	STRCAT(grant," TO "USER1);
	SPRINTF(buffer,"Grant Stmt 1.(Table: "TABLE")= '%s'"CRLF,grant);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, grant, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Grant Table 2. (USER2) ------------------------------------- */
	STRCPY(tabname, TABLE);
	STRCPY(grant,"GRANT ALL ON ");
	STRCAT(grant,tabname);
	STRCAT(grant," TO "USER2" WITH GRANT OPTION");
	SPRINTF(buffer,"Grant Stmt 2.(Table: "TABLE")= '%s'"CRLF,grant);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, grant, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

/* ----------------------------------------------------------------------- */
	/* *************************************************************** */
	/* *** I. SQLTablePrivileges ************************************* */
	/* *************************************************************** */
	SPRINTF(buffer,"I.) SQLTablePrivileges -> \
		(TableOwner: %s - TableName: "TABLE" )"CRLF, ODBCUSER);
	PRINTF(buffer);

	rc = SQLTablePrivileges(hstmt,
			NULL, 0, 			/* Table qualifier */
			ODBCUSER, SQL_NTS, 		/* Table owner     */
			TABLE, SQL_NTS);		/* Table name      */
	SPRINTF(buffer," SQLTablePrivileges retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	if (rc == SQL_SUCCESS)
		lstTablePrivilegesInfo(henv, hdbc, hstmt);

	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Drop Table --------------------------------------------- */

	/* --- Drop Table 1. ------------------------------------------ */
	STRCPY(tabname, TABLE);
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt (Table:"TABLE")= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);
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
/* ------------------------------------------------------------------------ */
RETCODE lstTablePrivilegesInfo(HENV henv, HDBC hdbc, HSTMT hstmt)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szTableQualifier[STR_LEN],
		szTableOwner[STR_LEN],
	        szTableName[STR_LEN],
		szGrantor[STR_LEN],
		szGrantee[STR_LEN],
		szPrivilege[STR_LEN],
		szIsGrantable[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SQLLEN 	cbTableQualifier, cbTableOwner, cbTableName,
		cbGrantor, cbGrantee, cbPrivilege, cbIsGrantable;

	SWORD  count=0;
/* ------------------------------------------------------------------har- */
	/* Bind columns in result set to storage locations */
	SQLBindCol(hstmt, 1, SQL_C_CHAR, szTableQualifier,
				STR_LEN,&cbTableQualifier);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, szTableOwner,
				STR_LEN, &cbTableOwner);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, szGrantor, STR_LEN,&cbGrantor);
	SQLBindCol(hstmt, 5, SQL_C_CHAR, szGrantee, STR_LEN,&cbGrantee);
	SQLBindCol(hstmt, 6, SQL_C_CHAR, szPrivilege, STR_LEN,&cbPrivilege);
	SQLBindCol(hstmt, 7, SQL_C_CHAR, szIsGrantable,
				STR_LEN,&cbIsGrantable);

	rc = lst_ColumnNames(henv, hdbc, hstmt, 7);

	while(TRUE) {
		count++;

		rc = SQLFetch(hstmt);
		/* ERROR_CHECK(hstmt);  */

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbTableQualifier == SQL_NULL_DATA)
				STRCPY(szTableQualifier, NULL_VALUE);
			if (cbTableOwner == SQL_NULL_DATA)
				STRCPY(szTableOwner, NULL_VALUE);
			if (cbTableName == SQL_NULL_DATA)
				STRCPY(szTableName, NULL_VALUE);
			if (cbGrantor == SQL_NULL_DATA)
				STRCPY(szGrantor, NULL_VALUE);
			if (cbGrantee == SQL_NULL_DATA)
				STRCPY(szGrantee, NULL_VALUE);
			if (cbPrivilege == SQL_NULL_DATA)
				STRCPY(szPrivilege, NULL_VALUE);
			if (cbIsGrantable == SQL_NULL_DATA)
				STRCPY(szIsGrantable, NULL_VALUE);

			SPRINTF(buffer,"Table %d : \
				'%s','%s','%s','%s','%s','%s','%s'"CRLF,
			  count, szTableQualifier, szTableOwner, szTableName,
			  szGrantor, szGrantee, szPrivilege, szIsGrantable);
			PRINTF(buffer);
		} else {
			/* ERROR_CHECK(hstmt);  */
			break;
		}
	    }
	return TRUE;
}
/* ----------------------------------------------------------------------- */

RETCODE lst_ColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt, SWORD outcol)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UWORD			icol;
	UCHAR                   szColName[MAX_NAME_LEN];
	SWORD                   cbColNameMax;
	SWORD                   pcbColName;
	SWORD			pfSqlType;
	SQLULEN			pcbColDef;
	SWORD			pibScale;
	SWORD			pfNullable;
/* ------------------------------------------------------------------------- */
	STRCPY(buffer,"Columns->|");

	icol = 1; cbColNameMax = MAX_COL_NAME_LEN;
	while (icol <= outcol) {
		rc = SQLDescribeCol(hstmt, icol, szColName, cbColNameMax,
					&pcbColName, &pfSqlType, &pcbColDef,
					&pibScale, &pfNullable);
		STRCAT(buffer, szColName); STRCAT(buffer,"|");
		icol++;
	}
	STRCAT(buffer, CRLF);
	PRINTF(buffer);

return(rc);
}
/* ------------------------------------------------------------------------- */
