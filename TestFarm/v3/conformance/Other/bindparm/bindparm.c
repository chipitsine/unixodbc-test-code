/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : bindparm.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test von SQLBINDPARAMETER
**
** Date         : 09.12.94
** l.aenderung  : 13:14pm 03-14-1996 (Harun)
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
#include <tst.h>

/* ---------------------------------------------------------------------- */
#define PROGNAME "bindparm"
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
		  case(100) : break; \
		  default   : exit(16); \
			      break; } \
	}

 UCHAR sqlstate  [ 10 ];
 SDWORD esq_sql_code;
 UCHAR error_txt  [ 101 ] ;
 SWORD len_error_txt = 100;
 SWORD used_error_txt;
 UCHAR buffer[1024];
/* ----------------------------------------------------------------------- */
#define TESTNAME "SQLBindParameter"
#define TABLE "BINDTEST"

#define MAX_NAME_LEN 512
#define MAX_SCALE 2

/*
 --------------------------------------------------------------------------
| DoSQLBindParameter:
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

/* ------------------------------------------------------------------------ */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   tabname[MAX_NAME_LEN];
	UCHAR                   buf[MAX_NAME_LEN];

	UCHAR                   rgbValueChar[MAX_NAME_LEN+1];
	SWORD                   rgbValueSmallint;
	SDWORD                  rgbValueInteger;
	SFLOAT			rgbValueFloat;
	SDOUBLE			rgbValueDouble;
	DATE_STRUCT		rgbValueDate;
	TIME_STRUCT             rgbValueTime;

	UWORD			ipar;
	SWORD			fParamType;
	SWORD			fCType;
	SWORD			fSqlType;
	UDWORD			cbColDef;
	SWORD			ibScale;
	PTR			rgbValue;
	SQLLEN			cbValueMax;
	SQLLEN			pcbValue;

	SWORD                   cAbort=0;
/* ---------------------------------------------------------------------har- */

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

/* ---------------------------------------------------------------------har- */

	/* --- Create Table --------------------------------------------- */
	STRCPY(tabname,TABLE);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (\"TYP_CHAR\" CHAR(30), \"TYP_SMALLINT\" SMALLINT, \"TYP_INTEGER\" INTEGER,");
	STRCAT(create," \"TYP_FLOAT\" FLOAT, \"TYP_DOUBLE\" FIXED(13,2), \"TYP_DATE\" DATE, \"TYP_TIME\" TIME) ");
	SPRINTF(buffer,"Create Stmt = '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Insert Table --------------------------------------------- */
	/* --- 1. ---*/
	STRCPY(insert,"INSERT INTO ");
	STRCAT(insert,tabname);
	STRCAT(insert," (\"TYP_CHAR\", \"TYP_SMALLINT\", \"TYP_INTEGER\", \"TYP_FLOAT\", \"TYP_DOUBLE\", \"TYP_DATE\", \"TYP_TIME\" )");
	STRCAT(insert,"VALUES (?, ?, ?, ?, ?, ?, ?)");
	SPRINTF(buffer,"Insert Stmt = '%s'"CRLF,insert);
	PRINTF(buffer);

	rc = SQLPrepare(hstmt, insert, SQL_NTS);
	SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** SQLBindParameter -------------------------------------------- */
	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
			SQL_CHAR, MAX_NAME_LEN, 0, rgbValueChar, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_SMALLINT, 0, 0, &rgbValueSmallint, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG,
			SQL_INTEGER, 0, 0, &rgbValueInteger, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT,
			SQL_FLOAT, 0, 0, &rgbValueFloat, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_DOUBLE,
			SQL_DOUBLE, 0, MAX_SCALE, &rgbValueDouble, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_DATE,
			SQL_DATE, 0, 0, &rgbValueDate, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_TIME,
			SQL_TIME, 0, 0, &rgbValueTime, 0, &pcbValue);
	SPRINTF(buffer," SQLBindParameter retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* *** Set values -------------------------------------------------- */
	STRCPY(rgbValueChar,"Testname");
	rgbValueSmallint = 44;
	rgbValueInteger = 1234567890;
	rgbValueFloat    = 1234;
	rgbValueDouble   = 1234567890.12;
	rgbValueDate.year=1994;rgbValueDate.month=12;rgbValueDate.day=8;
	rgbValueTime.hour=11;rgbValueTime.minute=55;rgbValueTime.second=30;

	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	STRCPY(rgbValueChar,"---");
	rgbValueSmallint = 0;
	rgbValueInteger  = 0;
	rgbValueFloat    = 0;
	rgbValueDouble   = 0;
	rgbValueDate.year=0;rgbValueDate.month=0;rgbValueDate.day=0;
	rgbValueTime.hour=0;rgbValueTime.minute=0;rgbValueTime.second=0;

	/* --- Select Table --------------------------------------------- */
	/* --- 1. --- */
	STRCPY(select,"SELECT ");
	STRCAT(select,"\"TYP_CHAR\", \"TYP_SMALLINT\", \"TYP_INTEGER\", \"TYP_FLOAT\", \"TYP_DOUBLE\", \"TYP_DATE\", \"TYP_TIME\" ");
	STRCAT(select," FROM ");STRCAT(select,tabname);
	SPRINTF(buffer,"Select Stmt= '%s'"CRLF,select);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, select, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 1, SQL_C_CHAR, rgbValueChar, MAX_NAME_LEN, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 2, SQL_C_SHORT, &rgbValueSmallint, 0, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 3, SQL_C_LONG, &rgbValueInteger, 0, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 4, SQL_C_FLOAT, &rgbValueFloat, 0, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 5, SQL_C_DOUBLE, &rgbValueDouble, 0, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 6, SQL_C_DATE, &rgbValueDate, 0, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	pcbValue = SQL_NTS;
	rc = SQLBindCol(hstmt, 7, SQL_C_TIME, &rgbValueTime, 0, &pcbValue);
	SPRINTF(buffer," SQLBindCol retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	rc = SQLFetch(hstmt);
	SPRINTF(buffer," SQLFetch retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	SPRINTF(buffer,"Select Values --> "CRLF);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueChar         = '%s'"CRLF,rgbValueChar);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueSmallint     = '%d'"CRLF,rgbValueSmallint);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueInteger      = '%ld'"CRLF,rgbValueInteger);
	PRINTF(buffer);
	SPRINTF(buf,"%.0f",rgbValueFloat);
	SPRINTF(buffer,"rgbValueFloat        = '%s'"CRLF,buf);
	PRINTF(buffer);
	SPRINTF(buf,"%.2f",rgbValueDouble);
	SPRINTF(buffer,"rgbValueDouble       = '%s'"CRLF,buf);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueDate(Year)   = '%d'"CRLF,rgbValueDate.year);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueDate(Month)  = '%d'"CRLF,rgbValueDate.month);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueDate(Day)    = '%d'"CRLF,rgbValueDate.day);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueTime(Hour)   = '%d'"CRLF,rgbValueTime.hour);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueTime(Minute) = '%d'"CRLF,rgbValueTime.minute);
	PRINTF(buffer);
	SPRINTF(buffer,"rgbValueTime(second) = '%d'"CRLF,rgbValueTime.second);
	PRINTF(buffer);

	rc = SQLFreeStmt(hstmt, SQL_CLOSE);
	SPRINTF(buffer," SQLFreeStmt retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hstmt);

	/* --- Drop Table ----------------------------------------------- */
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt= '%s'"CRLF,drop);
	PRINTF(buffer);

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
