/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : foreigk1.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test der Funktion SQLForeignKeys
**
** Date         : 27.06.95
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
#define PROGNAME "foreigk1"
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
/* ---------------------------------------------------------------------- */

#define TESTNAME "SQLFOREIGNKEYS"

#define TABLE1 "FR_CUSTOMER"
#define TABLE2 "FR_EMPLOYEE"
#define TABLE3 "FR_SALES_ORDER"
#define TABLE4 "FR_SALES_LINE"

#define MAX_NAME_LEN 		512
#define MAX_COL_NAME_LEN 	18

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

/* ---------------------------------------------------------------------- */
RETCODE lstForeignKeysInfo(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE lst_ColumnNames(HENV henv, HDBC hdbc, HSTMT hstmt, SWORD outcol);

/*
 ----------------------------------------------------------------------------
| DoSQLForeignKeys:
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
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   colname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buffer[MAX_NAME_LEN+1];

	/* Declare storage locations for result set data */
	UCHAR   szPkQualifier[STR_LEN],
		szPkOwner[STR_LEN],
		szPkTableName[STR_LEN];
	UCHAR   szFkQualifier[STR_LEN],
		szFkOwner[STR_LEN],
		szFkTableName[STR_LEN];

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

	/* --- Create Table 1. ------------------------------------------ */
	STRCPY(tabname, TABLE1);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (CUSTOMER_ID INTEGER, CUST_NAME CHAR(30), \
		ADRESS CHAR(60), PHONE CHAR(15), PRIMARY KEY (CUSTOMER_ID))");
	SPRINTF(buffer,"Create Stmt (Table1: CUSTOMER)= '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Create Table 2. ------------------------------------------ */
	STRCPY(tabname, TABLE2);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (EMPLOYEE_ID INTEGER, NAME CHAR(30),AGE INTEGER, \
				BIRTHDAY DATE, PRIMARY KEY (EMPLOYEE_ID)) ");
	SPRINTF(buffer,"Create Stmt (Table2: EMPLOYEE)= '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Create Table 3. ------------------------------------------ */
	STRCPY(tabname, TABLE3);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (SALES_ID INTEGER, CUSTOMER_ID INTEGER, \
			   EMPLOYEE_ID INTEGER, TOTAL_PRICE FIXED(6,2), ");
	STRCAT(create,"PRIMARY KEY (SALES_ID), ");
	STRCAT(create,"FOREIGN KEY (CUSTOMER_ID) REFERENCES "TABLE1" \
							(CUSTOMER_ID), ");
	STRCAT(create,"FOREIGN KEY (EMPLOYEE_ID) REFERENCES "TABLE2" \
							(EMPLOYEE_ID)) ");
	SPRINTF(buffer,"Create Stmt (Table3: SALES_ORDER)= '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Create Table 4. ------------------------------------------ */
	STRCPY(tabname, TABLE4);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (SALES_ID INTEGER, LINE_NUMBER INTEGER, \
		     PART_ID INTEGER, QUANTITY INTEGER, PRICE FIXED(6,2), ");
	STRCAT(create,"FOREIGN KEY (SALES_ID) REFERENCES "TABLE3" \
			(SALES_ID)) ");
	SPRINTF(buffer,"Create Stmt (Table4: SALES_LINE)= '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

/* ----------------------------------------------------------------------- */
	/* *************************************************************** */
	/* *** I. SQLForeignKeys (PkTableName) *************************** */
	/* *************************************************************** */
	STRCPY(szPkTableName,TABLE3);

	SPRINTF(buffer,"I.) SQLForeignKeys -> \
			with primary key tablename: %s'"CRLF, szPkTableName);
	PRINTF(buffer);

	/* Get all the foreign keys that refer to TABLE3 primary key. */
	/* Wenn Ownername -> "lpSrvr->szValidLogin0, SQL_NTS,"*/
	rc = SQLForeignKeys(hstmt,
			NULL, 0,                     /* Primary qualifier */
			NULL, 0,                     /* Primary owner     */
			szPkTableName, SQL_NTS,      /* Primary table     */
			NULL, 0,                     /* Foreign qualifier */
			NULL, 0,                     /* Foreign owner     */
			NULL, 0);                    /* Foreign table     */
	SPRINTF(buffer," SQLForeignKeys retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	if (rc == SQL_SUCCESS)
		lstForeignKeysInfo(henv, hdbc, hstmt);

	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* *************************************************************** */
	/* *** II. SQLForeignKeys (FkTableName) ************************** */
	/* *************************************************************** */
	STRCPY(szFkTableName,TABLE3);

	SPRINTF(buffer,"II.) SQLForeignKeys -> \
			with foreign key tablename: %s'"CRLF, szFkTableName);
	PRINTF(buffer);

	/* Get all the foreign keys in the TABLE3 table. */
	/* Wenn Ownername -> "lpSrvr->szValidLogin0, SQL_NTS,"*/
	rc = SQLForeignKeys(hstmt,
			NULL, 0,                      /* Primary qualifier */
			NULL, 0,                      /* Primary owner     */
			NULL, 0,                      /* Primary table     */
			NULL, 0,                      /* Foreign qualifier */
			NULL, 0,                      /* Foreign owner     */
			szFkTableName, SQL_NTS);      /* Foreign table     */
	SPRINTF(buffer," SQLForeignKeys retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	if (rc == SQL_SUCCESS)
		lstForeignKeysInfo(henv, hdbc, hstmt);

	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* *************************************************************** */
	/* *** III. SQLForeignKeys (PkTableName and FkTableName) ********* */
	/* *************************************************************** */
	STRCPY(szPkTableName,TABLE3);
	STRCPY(szFkTableName,TABLE4);

	SPRINTF(buffer,"III.) SQLForeignKeys -> with primary key tablename:\
	  			'%s' and foreign key tablename: '%s'"CRLF,
				szPkTableName, szFkTableName);
	PRINTF(buffer);

	/*Get all the foreign keys in the TABLE3 table,refer to primary key.*/
	/* Wenn Ownername -> "lpSrvr->szValidLogin0, SQL_NTS,"*/
	rc = SQLForeignKeys(hstmt,
			NULL, 0,                      /* Primary qualifier */
			NULL, 0,                      /* Primary owner     */
			szPkTableName, SQL_NTS,       /* Primary table     */
			NULL, 0,                      /* Foreign qualifier */
			NULL, 0,                      /* Foreign owner     */
			szFkTableName, SQL_NTS);      /* Foreign table     */
	SPRINTF(buffer," SQLForeignKeys retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	if (rc == SQL_SUCCESS)
		lstForeignKeysInfo(henv, hdbc, hstmt);

	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Drop Table ----------------------------------------------- */
	/* --- Drop Table 1. ------------------------------------------ */
	STRCPY(tabname, TABLE1);
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt (Table1:"TABLE1")= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Drop Table 2. ------------------------------------------ */
	STRCPY(tabname, TABLE2);
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt (Table2:"TABLE2")= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Drop Table 3. ------------------------------------------ */
	STRCPY(tabname, TABLE3);
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt (Table3:"TABLE3")= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Drop Table 4. ------------------------------------------ */
	STRCPY(tabname, TABLE4);
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt (Table4:"TABLE4")= '%s'"CRLF,drop);
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
RETCODE lstForeignKeysInfo(HENV henv, HDBC hdbc, HSTMT hstmt)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ----------------------------------------------------------------------- */
	/* Declare storage locations for result set data */
	UCHAR   szPkQualifier[STR_LEN],
		szPkOwner[STR_LEN],
		szPkTableName[STR_LEN],
		szPkColumnName[STR_LEN];
	UCHAR   szFkQualifier[STR_LEN],
		szFkOwner[STR_LEN],
		szFkTableName[STR_LEN],
		szFkColumnName[STR_LEN];
	UCHAR   szFkName[STR_LEN],
		szPkName[STR_LEN];
	SWORD   KeySeq,
		UpdateRule,
		DeleteRule;
	UCHAR   szUpdateRule[STR_LEN],
		szDeleteRule[STR_LEN];

	/* Declare storage locations for bytes available to return */
	SQLLEN cbPkQualifier, cbPkOwner, cbPkTableName, cbPkColumnName;
	SQLLEN cbFkQualifier, cbFkOwner, cbFkTableName, cbFkColumnName;
	SQLLEN cbKeySeq, cbUpdateRule, cbDeleteRule, cbFkName, cbPkName;

	SWORD  count=0;
/* -------------------------------------------------------------------har- */

	/* Bind columns in result set to storage locations */
	SQLBindCol(hstmt, 1, SQL_C_CHAR, szPkQualifier,
					STR_LEN,&cbPkQualifier);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, szPkOwner,
					STR_LEN, &cbPkOwner);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, szPkTableName,
					STR_LEN,&cbPkTableName);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, szPkColumnName,
					STR_LEN,&cbPkColumnName);
	SQLBindCol(hstmt, 5, SQL_C_CHAR, szFkQualifier,
					STR_LEN,&cbFkQualifier);
	SQLBindCol(hstmt, 6, SQL_C_CHAR, szFkOwner,
					STR_LEN, &cbFkOwner);
	SQLBindCol(hstmt, 7, SQL_C_CHAR, szFkTableName,
					STR_LEN,&cbFkTableName);
	SQLBindCol(hstmt, 8, SQL_C_CHAR, szFkColumnName,
					STR_LEN,&cbFkColumnName);
	SQLBindCol(hstmt, 9, SQL_C_SHORT, &KeySeq, 0, &cbKeySeq);
	SQLBindCol(hstmt,10, SQL_C_SHORT, &UpdateRule, 0, &cbUpdateRule);
	SQLBindCol(hstmt,11, SQL_C_SHORT, &DeleteRule, 0, &cbDeleteRule);
	SQLBindCol(hstmt,12, SQL_C_CHAR, szFkName, STR_LEN,&cbFkName);
	SQLBindCol(hstmt,13, SQL_C_CHAR, szPkName, STR_LEN,&cbPkName);

	rc = lst_ColumnNames(henv, hdbc, hstmt, 13);

	while(TRUE) {
		count++;

		rc = SQLFetch(hstmt);
		/* ERROR_CHECK(hstmt); */

		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){
			/* Process fetched data */
			if (cbPkQualifier == SQL_NULL_DATA)
				STRCPY(szPkQualifier, NULL_VALUE);
			if (cbPkOwner == SQL_NULL_DATA)
				STRCPY(szPkOwner, NULL_VALUE);
			if (cbPkTableName == SQL_NULL_DATA)
				STRCPY(szPkTableName, NULL_VALUE);
			if (cbPkColumnName == SQL_NULL_DATA)
				STRCPY(szPkColumnName, NULL_VALUE);
			if (cbFkQualifier == SQL_NULL_DATA)
				STRCPY(szFkQualifier, NULL_VALUE);
			if (cbFkOwner == SQL_NULL_DATA)
				STRCPY(szFkOwner, NULL_VALUE);
			if (cbFkTableName == SQL_NULL_DATA)
				STRCPY(szFkTableName, NULL_VALUE);
			if (cbFkColumnName == SQL_NULL_DATA)
				STRCPY(szFkColumnName, NULL_VALUE);
			if (cbKeySeq == SQL_NULL_DATA) KeySeq = 0;
			if (cbUpdateRule == SQL_NULL_DATA)
			        UpdateRule = SQL_SET_NULL;
			/*	STRCPY(szUpdateRule, NULL_VALUE); */
			if (cbDeleteRule == SQL_NULL_DATA)
			        DeleteRule = SQL_SET_NULL;
				/* STRCPY(szDeleteRule, NULL_VALUE); */
			if (cbDeleteRule == SQL_NULL_DATA)
				DeleteRule = 0;
			if (cbFkName == SQL_NULL_DATA)
				STRCPY(szFkName, NULL_VALUE);
			if (cbPkName == SQL_NULL_DATA)
				STRCPY(szPkName, NULL_VALUE);

			/* UpdateRule information */
			switch (UpdateRule) {
			  case SQL_CASCADE  :
			    STRCPY(szUpdateRule,"SQL_CASCADE"); break;
			  case SQL_RESTRICT :
			    STRCPY(szUpdateRule,"SQL_RESTRICT"); break;
			  case SQL_SET_NULL :
			    STRCPY(szUpdateRule,"SQL_SET_NULL"); break;
			}
			/* DeleteRule information */
			switch (DeleteRule) {
			  case SQL_CASCADE        :
				STRCPY(szDeleteRule,"SQL_CASCADE"); break;
			  case SQL_RESTRICT       :
				STRCPY(szDeleteRule,"SQL_RESTRICT"); break;
			  case SQL_SET_NULL       :
				STRCPY(szDeleteRule,"SQL_SET_NULL"); break;
			}
			SPRINTF(buffer, "Column %d : \
			   '%s','%s','%s','%s','%s','%s','%s','%s','%d','%s',\
			   '%s','%s','%s' "CRLF,
			  count, szPkQualifier, szPkOwner, szPkTableName, \
			  szPkColumnName, szFkQualifier, szFkOwner, \
			  szFkTableName, szFkColumnName, KeySeq,szUpdateRule,\
			  szDeleteRule, szFkName, szPkName);
			PRINTF(buffer);
		} else {
			/* ERROR_CHECK(hstmt) */
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
/* ----------------------------------------------------------------------- */
