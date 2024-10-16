/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : colattrb.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test der Funktion SQLCOLATTRIBUTES
**
** Date         : 14.07.95
** l.aenderung  : 16:02pm 03-14-1996 (Harun)
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
#define PROGNAME "colattrb"
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

#define TESTNAME "SQLColAttributes"
#define TABLE "COLATTRB"
#define MAX_NAME_LEN 512

/* --------------------------------------------------------------------- */
RETCODE Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE Get_pfNullable(SWORD pfNullable, UCHAR FAR * buffer);
RETCODE Get_Searchable(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE Get_Updatable(SWORD pfDesc, UCHAR FAR * buffer);

/*
 -------------------------------------------------------------------------
| DoSQLColAttributes:
|
|
| Returns:
|
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
	UCHAR                   tabname[MAX_NAME_LEN+1];
	UCHAR                   create[(MAX_NAME_LEN+1)*2];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   buf[MAX_NAME_LEN+1];

	UWORD			icol;
	UWORD                   fDescType;
	PTR                     rgbDesc;
	UCHAR                   rgbDescChar[MAX_NAME_LEN];
	SWORD			cbDescMax;
	SWORD			pcbDesc;
	SQLLEN			pfDesc;
	SWORD			pfDescSic;

	SWORD                   cAbort=0;
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

 rc = SQLConnect(hdbc, DBNODE, SQL_NTS, ODBCUSER, SQL_NTS, ODBCPASSW, SQL_NTS );
 SPRINTF(buffer," SQLConnect retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);

 rc = SQLAllocStmt (hdbc, phstmt);
 SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
 PRINTF(buffer);
 ERROR_CHECK(NULL);
/* ---------------------------------------------------------------------- */

	/* --- Create Table --------------------------------------------- */
	STRCPY(tabname, TABLE);
	STRCPY(create,"CREATE TABLE ");
	STRCAT(create,tabname);
	STRCAT(create," (ID INTEGER, NAME VARCHAR(80), AGE SMALLINT)");
	SPRINTF(buffer,"Create Stmt = '%s'"CRLF,create);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Insert Table --------------------------------------------- */
	/* 1. */
	STRCPY(tabname,TABLE);
	STRCPY(insert,"INSERT INTO ");
	STRCAT(insert,tabname);
	STRCAT(insert," VALUES (10, 'TestName', 40)");
	SPRINTF(buffer,"Insert Stmt= '%s'"CRLF,insert);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- Select Table --------------------------------------------- */
	/* 1. */
	STRCPY(tabname, TABLE);
	STRCPY(select,"SELECT * FROM ");
	STRCAT(select,tabname);
	SPRINTF(buffer,"Select Stmt= '%s'"CRLF,select);
	PRINTF(buffer);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	/* --- SQLColAttributes ---------------------------------------- */
	icol=1;

	SPRINTF(buffer,"SQLColAttributes Column: %d -> (ouput) "CRLF,icol);
	PRINTF(buffer);

	/* ***** 1. SQL_COLUMN_AUTO_INCREMNT */
	icol = 1; fDescType = SQL_COLUMN_AUTO_INCREMENT;cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_BoolValue( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"1.SQL_COLUMN_AUTO_INCREMT: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* ***** 2. SQL_COLUMN_CASE_SENSITIVE */
	icol = 1; fDescType = SQL_COLUMN_CASE_SENSITIVE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_BoolValue( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"2.SQL_COLUMN_CASE_SENSITIVE: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* ***** 3. SQL_COLUMN_COUNT */
	icol = 1; fDescType = SQL_COLUMN_COUNT; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"3.SQL_COLUMN_COUNT: '%d'"CRLF,pfDesc);
	PRINTF(buffer);

	/* ***** 4. SQL_COLUMN_DISPLAY_SIZE */
	icol = 1; fDescType = SQL_COLUMN_DISPLAY_SIZE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"4.SQL_COLUMN_DISPLAY_SIZE: '%d'"CRLF,pfDesc);
	PRINTF(buffer);

	/* ***** 5. SQL_COLUMN_LABEL (ODBC 2.0)*/
	icol = 1; fDescType = SQL_COLUMN_LABEL; cbDescMax=MAX_NAME_LEN;STRCPY(rgbDescChar,"\0");
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"5.SQL_COLUMN_LABEL (ODBC 2.0): '%s'"CRLF,rgbDescChar);
	PRINTF(buffer);

	/* ***** 6. SQL_COLUMN_LENGTH */
	icol = 1; fDescType = SQL_COLUMN_LENGTH; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"6.SQL_COLUMN_LENGTH: '%d'"CRLF,pfDesc);
	PRINTF(buffer);

	/* ***** 7. SQL_COLUMN_MONEY */
	icol = 1; fDescType = SQL_COLUMN_MONEY; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_BoolValue( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"7.SQL_COLUMN_MONEY: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* ***** 8. SQL_COLUMN_NAME */
	icol = 1; fDescType = SQL_COLUMN_NAME; cbDescMax=MAX_NAME_LEN;STRCPY(rgbDescChar,"\0");
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"8.SQL_COLUMN_NAME: '%s'"CRLF,rgbDescChar);
	PRINTF(buffer);

	/* ***** 9. SQL_COLUMN_NULLABLE */
	icol = 1; fDescType = SQL_COLUMN_NULLABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_pfNullable( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"9.SQL_COLUMN_NULLABLE: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *****10. SQL_COLUMN_OWNER_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_OWNER_NAME; cbDescMax=MAX_NAME_LEN;STRCPY(rgbDescChar,"\0");
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"10.SQL_COLUMN_OWNER_NAME (ODBC 2.0): '%s'"CRLF,rgbDescChar);
	PRINTF(buffer);

	/* *****11. SQL_COLUMN_PRECISION */
	icol = 1; fDescType = SQL_COLUMN_PRECISION; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"11.SQL_COLUMN_PRECISION: '%d'"CRLF,pfDesc);
	PRINTF(buffer);

	/* *****12. SQL_COLUMN_QUALIFIER_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_QUALIFIER_NAME; cbDescMax=MAX_NAME_LEN;STRCPY(rgbDescChar,"\0");
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"12.SQL_COLUMN_QUALIFIER_NAME (ODBC 2.0): '%s'"CRLF,rgbDescChar);
	PRINTF(buffer);

	/* *****13. SQL_COLUMN_SCALE */
	icol = 1; fDescType = SQL_COLUMN_SCALE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"13.SQL_COLUMN_SCALE: '%d'"CRLF,pfDesc);
	PRINTF(buffer);

	/* *****14. SQL_COLUMN_SEARCHABLE */
	icol = 1; fDescType = SQL_COLUMN_SEARCHABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_Searchable( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"14.SQL_COLUMN_SEARCHABLE: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *****15. SQL_COLUMN_TABLE_NAME (ODBC 2.0) */
	icol = 1; fDescType = SQL_COLUMN_TABLE_NAME; cbDescMax=MAX_NAME_LEN;STRCPY(rgbDescChar,"\0");
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"15.SQL_COLUMN_TABLE_NAME (ODBC 2.0): '%s'"CRLF,rgbDescChar);
	PRINTF(buffer);

	/* *****16. SQL_COLUMN_TYPE */
	icol = 1; fDescType = SQL_COLUMN_TYPE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"16.SQL_COLUMN_TYPE: '%d'"CRLF,pfDesc);
	PRINTF(buffer);

	/* *****17. SQL_COLUMN_TYPE_NAME */
	icol = 1; fDescType = SQL_COLUMN_TYPE_NAME; cbDescMax=MAX_NAME_LEN;STRCPY(rgbDescChar,"\0");
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDescChar, cbDescMax,
				 &pcbDesc, &pfDesc);

	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	SPRINTF(buffer,"17.SQL_COLUMN_TYPE_NAME: '%s'"CRLF,rgbDescChar);
	PRINTF(buffer);

	/* *****18. SQL_COLUMN_UNSIGNED */
	icol = 1; fDescType = SQL_COLUMN_UNSIGNED;cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_BoolValue( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"18.SQL_COLUMN_UNSIGNED: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* *****19. SQL_COLUMN_UPDATABLE */
	icol = 1; fDescType = SQL_COLUMN_UPDATABLE; cbDescMax=MAX_NAME_LEN;
	rc = SQLColAttributes(hstmt, icol, fDescType,(PTR)&rgbDesc, cbDescMax,
				 &pcbDesc, &pfDesc);
	SPRINTF(buffer," SQLColAttributes retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(NULL);

	Get_Updatable( (SWORD) pfDesc, buf);
	SPRINTF(buffer,"19.SQL_COLUMN_UPDATABLE: '%s'"CRLF,buf);
	PRINTF(buffer);

	/* --- Drop Table ----------------------------------------------- */
	STRCPY(tabname, TABLE);
	STRCPY(drop,"DROP TABLE ");
	STRCAT(drop,tabname);
	SPRINTF(buffer,"Drop Stmt= '%s'"CRLF,drop);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, drop, SQL_NTS);
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

/*
 -----------------------------------------------------------------------------
| Get_BoolValue:
|
|
| Returns:
 -----------------------------------------------------------------------------
*/
RETCODE Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer)
{
 switch (pfDesc) {
	case TRUE :
			STRCPY(buffer,"TRUE");
			break;
	case FALSE :
			STRCPY(buffer,"FALSE");
			break;
	default    :
			STRCPY(buffer,"<NULL>");
			break;
	}
  return 1;
}
/*
 -----------------------------------------------------------------------------
| Get_pfNullable:
|
|
| Returns:
 -----------------------------------------------------------------------------
*/
RETCODE Get_pfNullable(SWORD pfNullable, UCHAR FAR * buffer)
{
 switch (pfNullable) {
	case SQL_NO_NULLS :
			STRCPY(buffer,"SQL_NO_NULLS");
			break;
	case SQL_NULLABLE :
			STRCPY(buffer,"SQL_NULLABLE");
			break;
	case SQL_NULLABLE_UNKNOWN :
			STRCPY(buffer,"SQL_NULL_UNKNOWN");
			break;
	}
  return 1;
}
/*
 -----------------------------------------------------------------------------
| Get_Searchable:
|
|
| Returns:
 -----------------------------------------------------------------------------
*/
RETCODE Get_Searchable(SWORD pfDesc, UCHAR FAR * buffer)
{
 switch (pfDesc) {
	case SQL_UNSEARCHABLE :
			STRCPY(buffer,"SQL_UNSEARCHABLE");
			break;
	case SQL_LIKE_ONLY :
			STRCPY(buffer,"SQL_LIKE_ONLY");
			break;
	case SQL_ALL_EXCEPT_LIKE :
			STRCPY(buffer,"SQL_ALL_EXCEPT_LIKE");
			break;
	case SQL_SEARCHABLE :
			STRCPY(buffer,"SQL_SEARCHABLE");
			break;
	default    :
			STRCPY(buffer,"<NULL>");
			break;
	}
  return 1;
}
/*
 ----------------------------------------------------------------------------
| Get_Updatable:
|
|
| Returns:
 ----------------------------------------------------------------------------
*/
RETCODE Get_Updatable(SWORD pfDesc, UCHAR FAR * buffer)
{
 switch (pfDesc) {
	case SQL_ATTR_READONLY :
			STRCPY(buffer,"SQL_ATTR_READONLY");
			break;
	case SQL_ATTR_WRITE :
			STRCPY(buffer,"SQL_ATTR_WRITE");
			break;
	case SQL_ATTR_READWRITE_UNKNOWN :
			STRCPY(buffer,"SQL_ATTR_READWRITE_UNKNOWN");
			break;
	default    :
			STRCPY(buffer,"<NULL>");
			break;
	}
  return 1;
}
/* ------------------------------------------------------------------------- */
