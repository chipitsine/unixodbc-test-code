/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : getinfo.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test von SQLGETINFO
**
** Date         : 28.10.94
** l.aenderung  : 14:59pm 03-14-1996 (Harun)
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
#define PROGNAME "getinfo1"
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

#define TESTNAME "SQLGetInfo"
#define TABLE ""

#define MAX_RGB_VALUE 512
#define MAX_NAME_LEN  512

#define GETINFO_ALL
/* #define GETINFO_OPT1 */
/* #define GETINFO_OPT2 */
/* #define GETINFO_OPT3 */
/* #define GETINFO_OPT4 */
/* #define GETINFO_OPT5 */
/* #define GETINFO_OPT6 */
/* #define GETINFO_OPT7 */

/* *****************************  Test Cases  ****************************** */
/*  The following functions implement the tests                              */
/* ************************************************************************* */

RETCODE PutDriverInformation(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE PutDBMSProductInformation(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE PutDataSourceInformation(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE PutSupportedSQL(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE PutSQLLimits(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE PutScalarFunctionInformation(HENV henv, HDBC hdbc, HSTMT hstmt);
RETCODE PutConversionInformation(HENV henv, HDBC hdbc, HSTMT hstmt);
void    CheckConversion(UDWORD rgbInfoValue, UCHAR buffer[MAX_NAME_LEN]);

/*
 ------------------------------------------------------------------------
| DoSQLGetInfo:
|     This test case will test the SQLGetInfo function.
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

/* ----------------------------------------------------------------------- */

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
/* ***** 1.) Driver Information */
/* ---------------------------------------------------------------------- */
PutDriverInformation(henv, hdbc, hstmt);

/* ---------------------------------------------------------------------- */
/* ***** 2.) DBMS Product Information */
/* ---------------------------------------------------------------------- */
PutDBMSProductInformation(henv, hdbc, hstmt);

/* ---------------------------------------------------------------------- */
/* ***** 3.) Data Source Information */
/* ---------------------------------------------------------------------- */
PutDataSourceInformation(henv, hdbc, hstmt);

/* ---------------------------------------------------------------------- */
/* ***** 4.) Supported SQL */
/* ---------------------------------------------------------------------- */
PutSupportedSQL(henv, hdbc, hstmt);

/* ---------------------------------------------------------------------- */
/* ***** 5.) SQL Limits */
/* ---------------------------------------------------------------------- */
PutSQLLimits(henv, hdbc, hstmt);

/* ---------------------------------------------------------------------- */
/* ***** 6.) Scalar Function Information */
/* ---------------------------------------------------------------------- */
PutScalarFunctionInformation(henv, hdbc, hstmt);

/* ---------------------------------------------------------------------- */
/* ***** 7.) Conversion Information */
/* ---------------------------------------------------------------------- */
PutConversionInformation(henv, hdbc, hstmt);

/* ------------------------------------------------------------------har- */
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
/* *********************************************************************** */
/*
 -------------------------------------------------------------------------
| 1.PutDriverInformation:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutDriverInformation(HENV henv, HDBC hdbc, HSTMT hstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*5];
        UCHAR                   buf[MAX_NAME_LEN*5];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
        SQLSMALLINT             rgbInfoSmallValue;
        SQLUSMALLINT            rgbInfoSmallUValue;
        UDWORD                  rgbInfoValue;
        UWORD                   rgbInfoValueSh;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;

/* ---------------------------------------------------------------------- */
/* ***** 1.) Driver Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT1)
        SPRINTF(buffer, " 1.) Driver Information -->"CRLF);
	PRINTF(buffer);

        /* *** 1. SQL_ACTIVE_CONNECTIONS !*/

        SPRINTF(buffer, "1.1.SQL_ACTIVE_CONNECTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ACTIVE_CONNECTIONS, (PTR) &rgbInfoValueSh,
                                sizeof(rgbInfoValueSh), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValueSh);
	PRINTF(buffer);

	/* *** 2. SQL_ACTIVE_STATEMENTS !*/

        SPRINTF(buffer, "1.2.SQL_ACTIVE_STATEMENTS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ACTIVE_STATEMENTS, (PTR) &rgbInfoValueSh,
                                sizeof(rgbInfoValueSh), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValueSh);
	PRINTF(buffer);

        /* *** 3. SQL_DATA_SOURCE_NAME !*/
        SPRINTF(buffer, "1.3.SQL_DATA_SOURCE_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 4. SQL_DRIVER_HDBC !*/
        rgbInfoValueHandle = (ULONG *) malloc(sizeof(ULONG));
        MEMCPY(rgbInfoValueHandle, &hdbc, sizeof(HDBC));

        SPRINTF(buffer, "1.4.SQL_DRIVER_HDBC : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DRIVER_HDBC, (PTR) rgbInfoValueHandle,
                                sizeof(ULONG), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF,rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buf,"%p", *rgbInfoValueHandle);
        SPRINTF(buffer, "Value = GREPWEG('%s')"CRLF,buf);
	PRINTF(buffer);

        free(rgbInfoValueHandle);

        /* *** 5. SQL_DRIVER_HENV !*/
        rgbInfoValueHandle = (ULONG *) malloc(sizeof(ULONG));
        MEMCPY(rgbInfoValueHandle, &henv, sizeof(HENV));

        SPRINTF(buffer, "1.5.SQL_DRIVER_HENV : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DRIVER_HENV, (PTR) rgbInfoValueHandle,
                                sizeof(ULONG), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF,rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buf,"%p",*rgbInfoValueHandle);
        SPRINTF(buffer, "Value = GREPWEG('%s')"CRLF,buf);
	PRINTF(buffer);

        free(rgbInfoValueHandle);

        /* *** 6. SQL_DRIVER_HLIB       (ODBC 2.0) !*/
        rgbInfoValueHandle = (ULONG *) malloc(sizeof(ULONG));
        MEMCPY(rgbInfoValueHandle, &hdbc, sizeof(HDBC));

        SPRINTF(buffer, "1.6.SQL_DRIVER_HLIB (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
	/*
        rc = SQLGetInfo(hdbc, SQL_DRIVER_HLIB, (PTR) rgbInfoValueHandle,
                                sizeof(ULONG), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: "CRLF);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
	*/
        SPRINTF(buf,"%p",*rgbInfoValueHandle);
        SPRINTF(buffer, "Value = (WINDOWS only)"CRLF);
	PRINTF(buffer);

        free(rgbInfoValueHandle);

        /* *** 7. SQL_DRIVER_HSTMT ! */
        rgbInfoValueHandle = (ULONG *) malloc(sizeof(ULONG));
        MEMCPY(rgbInfoValueHandle, &hstmt, sizeof(HSTMT));

        SPRINTF(buffer, "1.7.SQL_DRIVER_HSTMT : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DRIVER_HSTMT, (PTR) rgbInfoValueHandle,
                                sizeof(ULONG), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF,rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buf,"%p", *rgbInfoValueHandle);
        SPRINTF(buffer, "Value = GREPWEG('%s')"CRLF,buf);
	PRINTF(buffer);

        free(rgbInfoValueHandle);

        /* *** 8. SQL_DRIVER_NAME !*/
        SPRINTF(buffer, "1.8.SQL_DRIVER_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DRIVER_NAME, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = GREPWEG('%s')"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 9. SQL_DRIVER_ODBC_VER   (ODBC 2.0) */
        SPRINTF(buffer, "1.9.SQL_DRIVER_ODBC_VER (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DRIVER_ODBC_VER, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 10. SQL_DRIVER_VER */
        SPRINTF(buffer, "1.10.SQL_DRIVER_VER : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DRIVER_VER, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        rgbInfoValueChar[8] = '\0';

        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 11.SQL_FETCH_DIRECTION !*/
        SPRINTF(buffer, "1.11.SQL_FETCH_DIRECTION : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_FETCH_DIRECTION, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_FD_FETCH_NEXT)     STRCAT(buf,"SQL_FD_FETCH_NEXT | ");
        if (rgbInfoValue & SQL_FD_FETCH_FIRST)    STRCAT(buf,"SQL_FD_FETCH_FIRST |");
        if (rgbInfoValue & SQL_FD_FETCH_LAST)     STRCAT(buf,"SQL_FD_FETCH_LAST |");
        if (rgbInfoValue & SQL_FD_FETCH_PRIOR)    STRCAT(buf,"SQL_FD_FETCH_PRIOR |");
        if (rgbInfoValue & SQL_FD_FETCH_ABSOLUTE) STRCAT(buf,"SQL_FD_FETCH_ABSOLUTE |");
        if (rgbInfoValue & SQL_FD_FETCH_RELATIVE) STRCAT(buf,"SQL_FD_FETCH_RELATIVE |");
        if (rgbInfoValue & SQL_FD_FETCH_BOOKMARK) STRCAT(buf,"SQL_FD_FETCH_BOOKMARK");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 12.SQL_FILE_USAGE        (ODBC 2.0) !*/
        SPRINTF(buffer, "1.12.SQL_FILE_USAGE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_FILE_USAGE, (PTR) &rgbInfoSmallUValue,
                                sizeof(rgbInfoSmallUValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallUValue) {
         case SQL_FILE_NOT_SUPPORTED : STRCPY(buf,"SQL_FILE_NOT_SUPPORTED"); break;
         case SQL_FILE_TABLE         : STRCPY(buf,"SQL_FILE_TABLE"); break;
         case SQL_FILE_QUALIFIER     : STRCPY(buf,"SQL_FILE_QUALIFIER"); break;
         default                     : SPRINTF(buf,"%d",rgbInfoSmallUValue);
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 13.SQL_GETDATA_EXTENSIONS(ODBC 2.0) !*/
        SPRINTF(buffer, "1.13.SQL_GETDATA_EXTENSIONS  (ODBC 2.0): "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_GETDATA_EXTENSIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_GD_ANY_COLUMN)STRCAT(buf,"SQL_GD_ANY_COLUMN |");
        if (rgbInfoValue & SQL_GD_ANY_ORDER) STRCAT(buf,"SQL_GD_ANY_ORDER |");
        if (rgbInfoValue & SQL_GD_BLOCK)     STRCAT(buf,"SQL_GD_BLOCK |");
        if (rgbInfoValue & SQL_GD_BOUND)     STRCAT(buf,"SQL_GD_BOUND");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 14.SQL_LOCK_TYPES        (ODBC 2.0) !*/
        SPRINTF(buffer, "1.14.SQL_LOCK_TYPES (ODBC 2.0): "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_LOCK_TYPES, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_LCK_NO_CHANGE)   STRCAT(buf,"SQL_LCK_NO_CHANGE |");
        if (rgbInfoValue & SQL_LCK_EXCLUSIVE)   STRCAT(buf,"SQL_LCK_EXCLUSIVE |");
        if (rgbInfoValue & SQL_LCK_UNLOCK)      STRCAT(buf,"SQL_LCK_UNLOCK");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 15.SQL_ODBC_API_CONFORMANCE !*/
        SPRINTF(buffer, "1.15.SQL_ODBC_API_CONFORMANCE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ODBC_API_CONFORMANCE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_OAC_NONE   : STRCPY(buf,"SQL_OAC_NONE"); break;
         case SQL_OAC_LEVEL1 : STRCPY(buf,"SQL_OAC_LEVEL1"); break;
         case SQL_OAC_LEVEL2 : STRCPY(buf,"SQL_OAC_LEVEL2"); break;
         default             : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 16.SQL_ODBC_SAG_CLI_CONFORMANCE !*/
        SPRINTF(buffer, "1.16.SQL_ODBC_SAG_CLI_CONFORMANCE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ODBC_SAG_CLI_CONFORMANCE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_OSCC_NOT_COMPLIANT : STRCPY(buf,"SQL_OSSCC_NOT_COMPLIANT"); break;
         case SQL_OSCC_COMPLIANT     : STRCPY(buf,"SQL_OSSCC_COMPLIANT"); break;
         default                     : STRCPY(buf,"!!!");
        }
	SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 17.SQL_ODBC_VER */
        SPRINTF(buffer, "1.17.SQL_ODBC_VER : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ODBC_VER, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF,rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        /* this value comes from the driver manager if available */
        /* SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar); */
	/* PRINTF(buffer); */

        /* *** 18.SQL_POS_OPERATIONS (ODBC 2.0) !*/
        SPRINTF(buffer, "1.18.SQL_POS_OPERATIONS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_POS_OPERATIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_POS_POSITION)  STRCAT(buf,"SQL_POS_POSITION |");
        if (rgbInfoValue & SQL_POS_REFRESH)   STRCAT(buf,"SQL_POS_REFRESH |");
        if (rgbInfoValue & SQL_POS_UPDATE)    STRCAT(buf,"SQL_POS_UPDATE |");
        if (rgbInfoValue & SQL_POS_DELETE)    STRCAT(buf,"SQL_POS_DELETE |");
        if (rgbInfoValue & SQL_POS_ADD)       STRCAT(buf,"SQL_POS_ADD");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 19.SQL_ROW_UPDATES !*/
        SPRINTF(buffer, "1.19.SQL_ROW_UPDATES : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ROW_UPDATES, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 20.SQL_SEARCH_PATTERN_ESCAPE !*/
        SPRINTF(buffer, "1.20.SQL_SEARCH_PATTERN_ESCAPE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SEARCH_PATTERN_ESCAPE, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 21.SQL_SERVER_NAME !*/
        SPRINTF(buffer, "1.21.SQL_SERVER_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SERVER_NAME, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = GREPWEG('%s')"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 2.PutDBMSProductInformation:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutDBMSProductInformation(HENV henv, HDBC hdbc, HSTMT hstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*5];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
        UDWORD                  rgbInfoValue;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 2.) DBMS Product Information */
/* ------------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT2)
        SPRINTF(buffer, " 2.) DBMS Product Information -->"CRLF);
	PRINTF(buffer);

        /* *** 1.SQL_DATABASE_NAME !*/
        SPRINTF(buffer, "2.1.SQL_DATABASE_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DATABASE_NAME, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = GREPWEG('%s')"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 2.SQL_DBMS_NAME !*/
        SPRINTF(buffer, "2.2.SQL_DBMS_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DBMS_NAME, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 3.SQL_DBMS_VER !*/
        SPRINTF(buffer, "2.3.SQL_DBMS_VER : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DBMS_VER, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);
DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 3.PutDataSourceInformation:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutDataSourceInformation(HENV henv, HDBC hdbc, HSTMT phstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*5];
        UCHAR                   buf[MAX_NAME_LEN*5];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
        SQLSMALLINT             rgbInfoSmallValue;
        UDWORD                  rgbInfoValue;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 3.) Data Source Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT3)
        SPRINTF(buffer, " 3.) Data Source Information -->"CRLF);
	PRINTF(buffer);

        /* *** 1.SQL_ACCESSIBLE_PROCEDURES */
        SPRINTF(buffer, "3.1.SQL_ACCESSIBLE_PROCEDURES : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ACCESSIBLE_PROCEDURES, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 2.SQL_ACCESSIBLE_TABLES !*/
        SPRINTF(buffer, "3.2.SQL_ACCESSIBLE_TABLES : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ACCESSIBLE_TABLES, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 3.SQL_BOOKMARK_PERSISTENCE (ODBC 2.0) !*/
        SPRINTF(buffer, "3.3.SQL_BOOKMARK_PERSISTENCE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_BOOKMARK_PERSISTENCE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_BP_CLOSE)        STRCAT(buf,"SQL_BP_CLOSE |");
        if (rgbInfoValue & SQL_BP_DELETE)       STRCAT(buf,"SQL_BP_DELETE |");
        if (rgbInfoValue & SQL_BP_DROP)         STRCAT(buf,"SQL_BP_DROP |");
        if (rgbInfoValue & SQL_BP_SCROLL)       STRCAT(buf,"SQL_BP_SCROLL |");
        if (rgbInfoValue & SQL_BP_TRANSACTION)  STRCAT(buf,"SQL_BP_TRANSACTION |");
        if (rgbInfoValue & SQL_BP_UPDATE)       STRCAT(buf,"SQL_BP_UPDATE |");
        if (rgbInfoValue & SQL_BP_OTHER_HSTMT)  STRCAT(buf,"SQL_BP_OTHER_HSTMT");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 4.SQL_CONCAT_NULL_BEHAVIOR! */
        SPRINTF(buffer, "3.4.SQL_CONCAT_NULL_BEHAVIOR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONCAT_NULL_BEHAVIOR, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_CB_NULL       : STRCPY(buf,"SQL_CB_NULL"); break;
         case SQL_CB_NON_NULL   : STRCPY(buf,"SQL_CB_NON_NULL"); break;
         default                : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 5.SQL_CURCOR_COMMIT_BEHAVIOR !*/
        SPRINTF(buffer, "3.5.SQL_CURSOR_COMMIT_BEHAVIOR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CURSOR_COMMIT_BEHAVIOR, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_CB_DELETE   : STRCPY(buf,"SQL_CB_DELETE"); break;
         case SQL_CB_CLOSE    : STRCPY(buf,"SQL_CB_CLOSE"); break;
         case SQL_CB_PRESERVE : STRCPY(buf,"SQL_CB_PRESERVE"); break;
         default              : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 6.SQL_CURSOR_ROLLBACK_BEHAVIOR !*/
        SPRINTF(buffer, "3.6.SQL_CURSOR_ROLLBACK_BEHAVIOR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CURSOR_ROLLBACK_BEHAVIOR, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_CB_DELETE   : STRCPY(buf,"SQL_CB_DELETE"); break;
         case SQL_CB_CLOSE    : STRCPY(buf,"SQL_CB_CLOSE"); break;
         case SQL_CB_PRESERVE : STRCPY(buf,"SQL_CB_PRESERVE"); break;
         default              : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 7.SQL_DATA_SOURCE_READ_ONLY !*/
        SPRINTF(buffer, "3.7.SQL_DATA_SOURCE_READ_ONLY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DATA_SOURCE_READ_ONLY, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 8.SQL_DEFAULT_TXN_ISOLATION !*/
        SPRINTF(buffer, "3.8.SQL_DEFAULT_TXN_ISOLATION : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_DEFAULT_TXN_ISOLATION, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_TXN_READ_UNCOMMITTED) STRCAT(buf,"SQL_TXN_READ_UNCOMMITTED |");
        if (rgbInfoValue & SQL_TXN_READ_COMMITTED)   STRCAT(buf,"SQL_TXN_READ_COMMITTED |");
        if (rgbInfoValue & SQL_TXN_REPEATABLE_READ)  STRCAT(buf,"SQL_TXN_REPEATABLE_READ |");
        if (rgbInfoValue & SQL_TXN_SERIALIZABLE)     STRCAT(buf,"SQL_TXN_SERIALIZABLE |");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 9.SQL_MULT_RESULT_SETS !*/
        SPRINTF(buffer, "3.9.SQL_MULT_RESULT_SETS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MULT_RESULT_SETS, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 10.SQL_MULTIPLE_ACTIVE_TXN !*/
        SPRINTF(buffer, "3.10.SQL_MULTIPLE_ACTIVE_TXN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MULTIPLE_ACTIVE_TXN, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 11.SQL_NEED_LONG_DATA_LEN (ODBC 2.0) !*/
        SPRINTF(buffer, "3.11.SQL_NEED_LONG_DATA_LEN (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_NEED_LONG_DATA_LEN, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 12.SQL_NULL_COLLATION (ODBC 2.0) !*/
        SPRINTF(buffer, "3.12.SQL_NULL_COLLATION (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_NULL_COLLATION, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_NC_END      : STRCPY(buf,"SQL_NC_END"); break;
         case SQL_NC_HIGH     : STRCPY(buf,"SQL_NC_HIGH"); break;
         case SQL_NC_LOW      : STRCPY(buf,"SQL_NC_LOW"); break;
         case SQL_NC_START    : STRCPY(buf,"SQL_NC_START"); break;
         default              : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 13.SQL_OWNER_TERM !*/
        SPRINTF(buffer, "3.13.SQL_OWNER_TERM : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_OWNER_TERM, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 14.SQL_PROCEDURE_TERM */
        SPRINTF(buffer, "3.14.SQL_PROCEDURE_TERM : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_PROCEDURE_TERM, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 15.SQL_QUALIFIER_TERM !*/
        SPRINTF(buffer, "3.15.SQL_QUALIFIER_TERM : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_QUALIFIER_TERM, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 16.SQL_SCROLL_CONCURRENCY !*/
        SPRINTF(buffer, "3.16.SQL_SCROLL_CONCURRENCY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SCROLL_CONCURRENCY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_SCCO_READ_ONLY)   STRCAT(buf,"SQL_SCCO_READ_ONLY |");
        if (rgbInfoValue & SQL_SCCO_LOCK)        STRCAT(buf,"SQL_SCCO_LOCK |");
        if (rgbInfoValue & SQL_SCCO_OPT_ROWVER)  STRCAT(buf,"SQL_SCCO_OPT_ROWVER |");
        if (rgbInfoValue & SQL_SCCO_OPT_VALUES)  STRCAT(buf,"SQL_SCCO_OPT_VALUES |");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 17.SQL_SCROLL_OPTIONS !*/
        SPRINTF(buffer, "3.17.SQL_SCROLL_OPTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SCROLL_OPTIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_SO_FORWARD_ONLY)   STRCAT(buf,"SQL_SCCO_FORWARD_ONLY |");
        if (rgbInfoValue & SQL_SO_STATIC)         STRCAT(buf,"SQL_SO_FORWARD_ONLY |");
        if (rgbInfoValue & SQL_SO_KEYSET_DRIVEN)  STRCAT(buf,"SQL_SO_KEYSET_DRIVEN |");
        if (rgbInfoValue & SQL_SO_DYNAMIC)        STRCAT(buf,"SQL_SO_DYNAMIC |");
        if (rgbInfoValue & SQL_SO_MIXED)          STRCAT(buf,"SQL_SO_MIXED");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 18.SQL_STATIC_SENSITIVITY (ODBC 2.0) !*/
        SPRINTF(buffer, "3.18.SQL_STATIC_SENSITIVITY (ODBC 2.0): "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_STATIC_SENSITIVITY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_SS_ADDITIONS) STRCAT(buf,"SQL_ADDITIONS |");
        if (rgbInfoValue & SQL_SS_DELETIONS) STRCAT(buf,"SQL_DELETIONS |");
        if (rgbInfoValue & SQL_SS_UPDATES)   STRCAT(buf,"SQL_UPDATES");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 19.SQL_TABLE_TERM !*/
        SPRINTF(buffer, "3.19.SQL_TABLE_TERM : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_TABLE_TERM, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 20.SQL_TXN_CAPABLE !*/
        SPRINTF(buffer, "3.20.SQL_TXN_CAPABLE: "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_TXN_CAPABLE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_TC_NONE       : STRCPY(buf,"SQL_ADDITIONS"); break;
         case SQL_TC_DML        : STRCPY(buf,"SQL_TC_DML"); break;
         case SQL_TC_DDL_COMMIT : STRCPY(buf,"SQL_TC_DDL_COMMIT"); break;
         case SQL_TC_DDL_IGNORE : STRCPY(buf,"SQL_TC_DLL_IGNORE"); break;
         case SQL_TC_ALL        : STRCPY(buf,"SQL_TC_ALL"); break;
         default                : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 21.SQL_TXN_ISOLATION_OPTION !*/
        SPRINTF(buffer, "3.21.SQL_TXN_ISOLATION_OPTION : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_TXN_ISOLATION_OPTION, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_TXN_READ_UNCOMMITTED) STRCAT(buf,"SQL_TXN_READ_UNCOMMITTED |");
        if (rgbInfoValue & SQL_TXN_READ_COMMITTED)   STRCAT(buf,"SQL_TXN_READ_COMMITTED |");
        if (rgbInfoValue & SQL_TXN_REPEATABLE_READ)  STRCAT(buf,"SQL_TXN_REPEATABLE_READ |");
        if (rgbInfoValue & SQL_TXN_SERIALIZABLE)     STRCAT(buf,"SQL_TXN_SERIALIZABLE |");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 22.SQL_USER_NAME !*/
        SPRINTF(buffer, "3.22.SQL_USER_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_USER_NAME, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 4.PutSuppotedSQL:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutSupportedSQL(HENV henv, HDBC hdbc, HSTMT hstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_RGB_VALUE*5];
        UCHAR                   buf[MAX_NAME_LEN*5];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE*3];
        SQLSMALLINT             rgbInfoSmallValue;
        UDWORD                  rgbInfoValue;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 4.) Supported SQL */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT4)
        SPRINTF(buffer, " 4.) Supported SQL -->"CRLF);
	PRINTF(buffer);

        /* *** 1.SQL_ALTER_TABLE (ODBC 2.0) !*/
        SPRINTF(buffer, "4.1.SQL_ALTER_TABLE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ALTER_TABLE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_AT_ADD_COLUMN) STRCAT(buf,"SQL_AT_ADD_COLUMN |");
        if (rgbInfoValue & SQL_AT_DROP_COLUMN) STRCAT(buf,"SQL_AT_DROP_COLUMN");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 2.SQL_COLUMN_ALIAS (ODBC 2.0) !*/
        SPRINTF(buffer, "4.2.SQL_COLUMN_ALIAS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_COLUMN_ALIAS, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);


        /* *** 3.SQL_CORRELATION_NAME !*/
        SPRINTF(buffer, "4.3.SQL_CORRELATION_NAME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CORRELATION_NAME, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_CN_NONE      : STRCPY(buf,"SQL_CN_NONE"); break;
         case SQL_CN_DIFFERENT : STRCPY(buf,"SQL_CN_DIFFERENT"); break;
         case SQL_CN_ANY       : STRCPY(buf,"SQL_CN_ANY"); break;
         default               : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 4.SQL_EXPRESSIONS_IN_ORDERBY !*/
        SPRINTF(buffer, "4.4.SQL_EXPRESSIONS_IN_ORDERBY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_EXPRESSIONS_IN_ORDERBY, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 5.SQL_GROUP_BY   (ODBC 2.0) !*/
        SPRINTF(buffer, "4.5.SQL_GROUP_BY (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_GROUP_BY, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_GB_NOT_SUPPORTED            : STRCPY(buf,"SQL_GB_NOT_SUPPORTED"); break;
         case SQL_GB_GROUP_BY_EQUALS_SELECT   : STRCPY(buf,"SQL_GB_GROUP_BY_EQUALS_SELECT"); break;
         case SQL_GB_GROUP_BY_CONTAINS_SELECT : STRCPY(buf,"SQL_GB_GROUP_BY_CONTAINS_SELECT"); break;
         case SQL_GB_NO_RELATION              : STRCPY(buf,"SQL_GB_NO_RELATION"); break;
         default                              : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 6.SQL_IDENTIFIER_CASE !*/
        SPRINTF(buffer, "4.6.SQL_IDENTIFIER_CASE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_IDENTIFIER_CASE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_IC_UPPER     : STRCPY(buf,"SQL_IC_UPPER"); break;
         case SQL_IC_LOWER     : STRCPY(buf,"SQL_IC_LOWER"); break;
         case SQL_IC_SENSITIVE : STRCPY(buf,"SQL_IC_SENSITIVE"); break;
         case SQL_IC_MIXED     : STRCPY(buf,"SQL_IC_MIXED"); break;
         default               : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 7.SQL_IDENTIFIER_QUOTE_CHAR !*/
        SPRINTF(buffer, "4.7.SQL_IDENTIFIER_QUOTE_CHAR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_IDENTIFIER_QUOTE_CHAR, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 8.SQL_KEYWORDS (ODBC 2.0) !*/
        SPRINTF(buffer, "4.8.SQL_KEYWORDS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_KEYWORDS, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 9.SQL_LIKE_ESCAPE_CLAUSE  (ODBC 2.0) !*/
        SPRINTF(buffer, "4.9.SQL_LIKE_ESCAPE_CLAUSE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_LIKE_ESCAPE_CLAUSE, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 10.SQL_NON_NULLABLE_COLUMNS !*/
        SPRINTF(buffer, "4.10.SQL_NON_NULLABLE_COLUMNS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_NON_NULLABLE_COLUMNS, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_NNC_NULL     : STRCPY(buf,"SQL_NNC_NULL"); break;
         case SQL_NNC_NON_NULL : STRCPY(buf,"SQL_NNC_NON_NULL"); break;
         default               : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 11.SQL_ODBC_SQL_CONFORMANCE !*/
        SPRINTF(buffer, "4.11.SQL_ODBC_SQL_CONFORMANCE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ODBC_SQL_CONFORMANCE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_OSC_MINIMUM  : STRCPY(buf,"SQL_OSC_MINIMUM"); break;
         case SQL_OSC_CORE     : STRCPY(buf,"SQL_OSC_CORE"); break;
         case SQL_OSC_EXTENDED : STRCPY(buf,"SQL_OSC_EXTENDED"); break;
         default               : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 12.SQL_ODBC_SQL_OPT_IEF !*/
        SPRINTF(buffer, "4.12.SQL_ODBC_SQL_OPT_IEF : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ODBC_SQL_OPT_IEF, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 13.SQL_ORDER_BY_COLUMNS_IN_SELECT (ODBC 2.0) !*/
        SPRINTF(buffer, "4.13.SQL_ORDER_BY_COLUMNS_IN_SELECT (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_ORDER_BY_COLUMNS_IN_SELECT, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 14.SQL_OUTER_JOINS !*/
        SPRINTF(buffer, "4.14.SQL_OUTER_JOINS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_OUTER_JOINS, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 15.SQL_OWNER_USAGE (ODBC 2.0) !*/
        SPRINTF(buffer, "4.15.SQL_OWNER_USAGE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_OWNER_USAGE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_OU_DML_STATEMENTS       ) STRCAT(buf,"SQL_OU_DML_STATEMENTS |");
        if (rgbInfoValue & SQL_OU_PROCEDURE_INVOCATION ) STRCAT(buf,"SQL_OU_PRECEDURE_INVOCATION |");
        if (rgbInfoValue & SQL_OU_TABLE_DEFINITION     ) STRCAT(buf,"SQL_OU_TABLE_DEFINITION |");
        if (rgbInfoValue & SQL_OU_INDEX_DEFINITION     ) STRCAT(buf,"SQL_OU_INDEX_DEFINITION |");
        if (rgbInfoValue & SQL_OU_PRIVILEGE_DEFINITION ) STRCAT(buf,"SQL_OU_PRIVILEGE_DEFINITION");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 16.SQL_POSITIONED_STATEMENTS (ODBC 2.0) !*/
        SPRINTF(buffer, "4.16.SQL_POSITIONED_STATEMENTS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_POSITIONED_STATEMENTS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_PS_POSITIONED_DELETE ) STRCAT(buf,"SQL_PS_POSITIONED_DELETE |");
        if (rgbInfoValue & SQL_PS_POSITIONED_UPDATE ) STRCAT(buf,"SQL_PS_POSITIONED_UPDATE |");
        if (rgbInfoValue & SQL_PS_SELECT_FOR_UPDATE ) STRCAT(buf,"SQL_PS_SELECT_FOR_UPDATE");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 17.SQL_PROCEDURES */
        SPRINTF(buffer, "4.17.SQL_PROCEDURES : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_PROCEDURES, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 18.SQL_QUALIFIER_LOCATION (ODBC 2.0) !*/
        SPRINTF(buffer, "4.18.SQL_QUALIFIER_LOCATION (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_QUALIFIER_LOCATION, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_QL_START : STRCPY(buf,"SQL_QL_START"); break;
         case SQL_QL_END   : STRCPY(buf,"SQL_QL_END"); break;
         default           : SPRINTF(buf,"%d", rgbInfoValue);
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 19.SQL_QUALIFIER_NAME_SEPARATOR !*/
        SPRINTF(buffer, "4.19.SQL_QUALIFIER_NAME_SEPARATOR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_QUALIFIER_NAME_SEPARATOR, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 20.SQL_QUALIFIER_USAGE (ODBC 2.0) !*/
        SPRINTF(buffer, "4.20.SQL_QUALIFIER_USAGE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_QUALIFIER_USAGE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_QU_DML_STATEMENTS)        STRCAT(buf,"SQL_QU_DML_STATEMENTS |");
        if (rgbInfoValue & SQL_QU_PROCEDURE_INVOCATION)  STRCAT(buf,"SQL_QU_PROCEDURE_INVOCATION |");
        if (rgbInfoValue & SQL_QU_TABLE_DEFINITION)      STRCAT(buf,"SQL_QU_TABLE_DEFINITION |");
        if (rgbInfoValue & SQL_QU_INDEX_DEFINITION)      STRCAT(buf,"SQL_QU_INDEX_DEFINITION |");
        if (rgbInfoValue & SQL_QU_PRIVILEGE_DEFINITION)  STRCAT(buf,"SQL_QU_PRIVILEGE_DEFINITION");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 21.SQL_QUOTED_IDENTIFIER_CASE                 (ODBC 2.0) */
        SPRINTF(buffer, "4.21.SQL_QUOTED_IDENTIFIER_CASE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_QUOTED_IDENTIFIER_CASE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        switch (rgbInfoSmallValue) {
         case SQL_IC_UPPER     : STRCPY(buf,"SQL_IC_UPPER"); break;
         case SQL_IC_LOWER     : STRCPY(buf,"SQL_IC_LOWER"); break;
         case SQL_IC_SENSITIVE : STRCPY(buf,"SQL_IC_SENSITIVE"); break;
         case SQL_IC_MIXED     : STRCPY(buf,"SQL_IC_MIXED"); break;
         default               : STRCPY(buf,"!!!");
        }
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 22.SQL_SPECIAL_CHARACTERS (ODBC 2.0) !*/
        SPRINTF(buffer, "4.22.SQL_SPECIAL_CHARACTERS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SPECIAL_CHARACTERS, (PTR) rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 23.SQL_SUBQUERIES (ODBC 2.0) !*/
        SPRINTF(buffer, "4.23.SQL_SUBQUERIES (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SUBQUERIES, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_SQ_CORRELATED_SUBQUERIES)  STRCAT(buf,"SQL_SQ_CORRELATED_SUBQUERIES |");
        if (rgbInfoValue & SQL_SQ_COMPARISON)             STRCAT(buf,"SQL_SQ_COMPARISON |");
        if (rgbInfoValue & SQL_SQ_EXISTS)                 STRCAT(buf,"SQL_SQ_EXISTS |");
        if (rgbInfoValue & SQL_SQ_IN)                     STRCAT(buf,"SQL_SQ_IN |");
        if (rgbInfoValue & SQL_SQ_QUANTIFIED)             STRCAT(buf,"SQL_SQ_QUANTIFIED");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 24.SQL_UNION (ODBC 2.0) !*/
        SPRINTF(buffer, "4.24.SQL_UNION (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_UNION, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_U_UNION)     STRCAT(buf,"SQL_U_UNION |");
        if (rgbInfoValue & SQL_U_UNION_ALL) STRCAT(buf,"SQL_U_UNION_ALL");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);
DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 5.PutSQLLimits:
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutSQLLimits(HENV henv, HDBC hdbc, HSTMT hstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*5];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
        SQLSMALLINT             rgbInfoSmallValue;
        UDWORD                  rgbInfoValue;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 5.) SQL Limits */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT5)
        SPRINTF(buffer, " 5.) SQL Limits -->"CRLF);
	PRINTF(buffer);

        /* *** 1.SQL_MAX_BINARY_LITERAL_LEN (ODBC 2.0) !*/
        SPRINTF(buffer, "5.1.SQL_MAX_BINARY_LITERAL_LEN (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_BINARY_LITERAL_LEN, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValue);
	PRINTF(buffer);

        /* *** 2.SQL_MAX_CHAR_LITERAL_LEN (ODBC 2.0) !*/
        SPRINTF(buffer, "5.2.SQL_MAX_CHAR_LITERAL_LEN (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_CHAR_LITERAL_LEN, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValue);
	PRINTF(buffer);

        /* *** 3.SQL_MAX_COLUMN_NAME_LEN !*/
        SPRINTF(buffer, "5.3.SQL_MAX_COLUMN_NAME_LEN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_COLUMN_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 4.SQL_MAX_COLUMNS_IN_GROUP_BY (ODBC 2.0) !*/
        SPRINTF(buffer, "5.4.SQL_MAX_COLUMNS_IN_GROUP_BY (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_BINARY_LITERAL_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 5.SQL_MAX_COLUMNS_IN_ORDER_BY (ODBC 2.0) !*/
        SPRINTF(buffer, "5.5.SQL_MAX_COLUMNS_IN_ORDER_BY (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_COLUMNS_IN_ORDER_BY, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 6.SQL_MAX_COLUMNS_IN_INDEX (ODBC 2.0) !*/
        SPRINTF(buffer, "5.6.SQL_MAX_COLUMNS_IN_INDEX (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_COLUMNS_IN_INDEX, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 7.SQL_MAX_COLUMNS_IN_SELECT (ODBC 2.0) !*/
        SPRINTF(buffer, "5.7.SQL_MAX_COLUMNS_IN_SELECT (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_COLUMNS_IN_SELECT, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 8.SQL_MAX_COLUMNS_IN_TABLE (ODBC 2.0) !*/
        SPRINTF(buffer, "5.8.SQL_MAX_COLUMNS_IN_TABLE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_COLUMNS_IN_TABLE, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 9.SQL_MAX_CURSOR_NAME_LEN !*/
        SPRINTF(buffer, "5.9.SQL_MAX_CURSOR_NAME_LEN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_CURSOR_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 10.SQL_MAX_INDEX_SIZE (OBDC 2.0) !*/
        SPRINTF(buffer, "5.10.SQL_MAX_INDEX_SIZE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_INDEX_SIZE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValue);
	PRINTF(buffer);

        /* *** 11.SQL_MAX_OWNER_NAME_LEN !*/
        SPRINTF(buffer, "5.11.SQL_MAX_OWNER_NAME_LEN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_OWNER_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 12.SQL_MAX_PROCEDURE_NAME_LEN */
        SPRINTF(buffer, "5.12.SQL_MAX_PROCEDURE_NAME_LEN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_PROCEDURE_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 13.SQL_MAX_QUALIFIER_NAME_LEN !*/
        SPRINTF(buffer, "5.13.SQL_MAX_QUALIFIER_NAME_LEN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_QUALIFIER_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 14.SQL_MAX_ROW_SIZE (ODBC 2.0) !*/
        SPRINTF(buffer, "5.14.SQL_MAX_ROW_SIZE (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_ROW_SIZE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValue);
	PRINTF(buffer);

        /* *** 15.SQL_MAX_ROW_SIZE_INCLUDES_LONG (ODBC 2.0) !*/
        SPRINTF(buffer, "5.15.SQL_MAX_ROW_SIZE_INCLUDES_LONG (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_ROW_SIZE_INCLUDES_LONG, (PTR) &rgbInfoValueChar,
                                sizeof(rgbInfoValueChar), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%s'"CRLF,rgbInfoValueChar);
	PRINTF(buffer);

        /* *** 16.SQL_MAX_STATEMENT_LEN (OBDC 2.0) !*/
        SPRINTF(buffer, "5.16.SQL_MAX_STATEMENT_LEN (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_STATEMENT_LEN, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoValue);
	PRINTF(buffer);

        /* *** 17.SQL_MAX_TABLE_NAME_LEN !*/
        SPRINTF(buffer, "5.17.SQL_MAX_TABLE_NAME_LEN : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_TABLE_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 18.SQL_MAX_TABLES_IN_SELECT (ODBC 2.0) !*/
        SPRINTF(buffer, "5.18.SQL_MAX_TABLES_IN_SELECT (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_TABLES_IN_SELECT, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);

        /* *** 19.SQL_MAX_USER_NAME_LEN                 (ODBC 2.0) */
        SPRINTF(buffer, "5.19.SQL_MAX_USER_NAME_LEN (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_MAX_USER_NAME_LEN, (PTR) &rgbInfoSmallValue,
                                sizeof(rgbInfoSmallValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        SPRINTF(buffer, "Value = '%d'"CRLF,rgbInfoSmallValue);
	PRINTF(buffer);
DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 6.PutScalarFunctionInformation
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutScalarFunctionInformation(HENV henv, HDBC hdbc, HSTMT hstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*10];
        UCHAR                   buf[MAX_NAME_LEN*10];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
        UDWORD                  rgbInfoValue;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 6.) Scalar Function Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT6)
        SPRINTF(buffer, " 6.) Scalar Function Information -->"CRLF);
	PRINTF(buffer);

        /* *** 1.SQL_CONVERT_FUNCTIONS !*/
        SPRINTF(buffer, "6.1.SQL_CONVERT_FUNCTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_FUNCTIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_FN_CVT_CONVERT) STRCAT(buf," SQL_FN_CVT_CONVERT ");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 2.SQL_NUMERIC_FUNCTIONS !*/
        SPRINTF(buffer, "6.2.SQL_NUMERIC_FUNCTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_NUMERIC_FUNCTIONS, (PTR) &rgbInfoValue,
                         sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue &  SQL_FN_NUM_ABS      ) STRCAT(buf,"SQL_FN_NUM_ABS |");
        if (rgbInfoValue &  SQL_FN_NUM_ACOS     ) STRCAT(buf," SQL_FN_NUM_ACOS |");
        if (rgbInfoValue &  SQL_FN_NUM_ASIN     ) STRCAT(buf," SQL_FN_NUM_ASIN |");
        if (rgbInfoValue &  SQL_FN_NUM_ATAN     ) STRCAT(buf," SQL_FN_NUM_ATAN |");
        if (rgbInfoValue &  SQL_FN_NUM_ATAN2    ) STRCAT(buf," SQL_FN_NUM_ATAN2 |");
        if (rgbInfoValue &  SQL_FN_NUM_CEILING  ) STRCAT(buf," SQL_FN_NUM_CEILING |");
        if (rgbInfoValue &  SQL_FN_NUM_COS      ) STRCAT(buf," SQL_FN_NUM_COS |");
        if (rgbInfoValue &  SQL_FN_NUM_COT      ) STRCAT(buf," SQL_FN_NUM_COT |");
        if (rgbInfoValue &  SQL_FN_NUM_DEGREES  ) STRCAT(buf," SQL_FN_NUM_DEGREES |");
        if (rgbInfoValue &  SQL_FN_NUM_EXP      ) STRCAT(buf," SQL_FN_NUM_EXP |");
        if (rgbInfoValue &  SQL_FN_NUM_FLOOR    ) STRCAT(buf," SQL_FN_NUM_FLOOR |");
        if (rgbInfoValue &  SQL_FN_NUM_LOG      ) STRCAT(buf," SQL_FN_NUM_LOG |");
        if (rgbInfoValue &  SQL_FN_NUM_LOG10    ) STRCAT(buf," SQL_FN_NUM_LOG10 |");
        if (rgbInfoValue &  SQL_FN_NUM_MOD      ) STRCAT(buf," SQL_FN_NUM_MOD |");
        if (rgbInfoValue &  SQL_FN_NUM_PI       ) STRCAT(buf," SQL_FN_NUM_PI |");
        if (rgbInfoValue &  SQL_FN_NUM_POWER    ) STRCAT(buf," SQL_FN_NUM_POWER |");
        if (rgbInfoValue &  SQL_FN_NUM_RADIANS  ) STRCAT(buf," SQL_FN_NUM_RADIANS |");
        if (rgbInfoValue &  SQL_FN_NUM_RAND     ) STRCAT(buf," SQL_FN_NUM_RAND |");
        if (rgbInfoValue &  SQL_FN_NUM_ROUND    ) STRCAT(buf," SQL_FN_NUM_ROUND |");
        if (rgbInfoValue &  SQL_FN_NUM_SIGN     ) STRCAT(buf," SQL_FN_NUM_SIGN |");
        if (rgbInfoValue &  SQL_FN_NUM_SQRT     ) STRCAT(buf," SQL_FN_NUM_SQRT |");
        if (rgbInfoValue &  SQL_FN_NUM_TAN      ) STRCAT(buf," SQL_FN_NUM_TAN |");
        if (rgbInfoValue &  SQL_FN_NUM_TRUNCATE ) STRCAT(buf," SQL_FN_NUM_TRUNCATE");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 3.SQL_STRING_FUNCTIONS !*/
        SPRINTF(buffer, "6.3.SQL_STRING_FUNCTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_STRING_FUNCTIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue & SQL_FN_STR_ASCII) STRCAT(buf," SQL_FN_STR_ASCII |");
        if (rgbInfoValue & SQL_FN_STR_CHAR) STRCAT(buf," SQL_FN_STR_CHAR |");
        if (rgbInfoValue & SQL_FN_STR_CONCAT) STRCAT(buf," SQL_FN_STR_CONCAT |");
        if (rgbInfoValue & SQL_FN_STR_DIFFERENCE) STRCAT(buf," SQL_FN_STR_DIFFERENCE |");
        if (rgbInfoValue & SQL_FN_STR_INSERT) STRCAT(buf," SQL_FN_STR_INSERT |");
        if (rgbInfoValue & SQL_FN_STR_LCASE) STRCAT(buf," SQL_FN_STR_LCASE |");
        if (rgbInfoValue & SQL_FN_STR_LEFT) STRCAT(buf," SQL_FN_STR_LEFT |");
        if (rgbInfoValue & SQL_FN_STR_LENGTH) STRCAT(buf," SQL_FN_STR_LENGTH |");
        if (rgbInfoValue & SQL_FN_STR_LOCATE) STRCAT(buf," SQL_FN_STR_LOCATE |");
        if (rgbInfoValue & SQL_FN_STR_LOCATE_2) STRCAT(buf," SQL_FN_STR_LOCATE_2 |");
        if (rgbInfoValue & SQL_FN_STR_LTRIM) STRCAT(buf," SQL_FN_STR_LTRIM |");
        if (rgbInfoValue & SQL_FN_STR_REPEAT) STRCAT(buf," SQL_FN_STR_REPEAT |");
        if (rgbInfoValue & SQL_FN_STR_RIGHT) STRCAT(buf," SQL_FN_STR_RIGHT |");
        if (rgbInfoValue & SQL_FN_STR_RTRIM) STRCAT(buf," SQL_FN_STR_RTRIM |");
        if (rgbInfoValue & SQL_FN_STR_SOUNDEX) STRCAT(buf," SQL_FN_STR_SOUNDEX |");
        if (rgbInfoValue & SQL_FN_STR_SPACE) STRCAT(buf," SQL_FN_STR_SPACE |");
        if (rgbInfoValue & SQL_FN_STR_SUBSTRING) STRCAT(buf," SQL_FN_STR_SUBSTRING |");
        if (rgbInfoValue & SQL_FN_STR_UCASE) STRCAT(buf," SQL_FN_STR_UCASE");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 4.SQL_SYSTEM_FUNCTIONS !*/
        SPRINTF(buffer, "6.4.SQL_SYSTEM_FUNCTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_SYSTEM_FUNCTIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue &  SQL_FN_SYS_DBNAME   ) STRCAT(buf," SQL_FN_SYS_DBNAME |");
        if (rgbInfoValue &  SQL_FN_SYS_IFNULL   ) STRCAT(buf," SQL_FN_SYS_IFNULL |");
        if (rgbInfoValue &  SQL_FN_SYS_USERNAME ) STRCAT(buf," SQL_FN_SYS_USERNAME ");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 5.SQL_TIMEDATE_ADD_INTERVALS (ODBC 2.0) !*/
        SPRINTF(buffer, "6.5.SQL_TIMEDATE_ADD_INTERVALS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_TIMEDATE_ADD_INTERVALS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue &  SQL_FN_TSI_FRAC_SECOND ) STRCAT(buf," SQL_FN_TSI_FRAC_SECOND |");
        if (rgbInfoValue &  SQL_FN_TSI_SECOND      ) STRCAT(buf," SQL_FN_TSI_SECOND |");
        if (rgbInfoValue &  SQL_FN_TSI_MINUTE      ) STRCAT(buf," SQL_FN_TSI_MINUTE |");
        if (rgbInfoValue &  SQL_FN_TSI_HOUR        ) STRCAT(buf," SQL_FN_TSI_HOUR |");
        if (rgbInfoValue &  SQL_FN_TSI_DAY         ) STRCAT(buf," SQL_FN_TSI_DAY |");
        if (rgbInfoValue &  SQL_FN_TSI_WEEK        ) STRCAT(buf," SQL_FN_TSI_WEEK |");
        if (rgbInfoValue &  SQL_FN_TSI_MONTH       ) STRCAT(buf," SQL_FN_TSI_MONTH |");
        if (rgbInfoValue &  SQL_FN_TSI_QUARTER     ) STRCAT(buf," SQL_FN_TSI_QUALIFIER |");
        if (rgbInfoValue &  SQL_FN_TSI_YEAR        ) STRCAT(buf," SQL_FN_TSI_YEAR ");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 6.SQL_TIMEDATE_DIFF_INTERVALS (ODBC 2.0) !*/
        SPRINTF(buffer, "6.6.SQL_TIMEDATE_DIFF_INTERVALS (ODBC 2.0) : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_TIMEDATE_DIFF_INTERVALS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue &  SQL_FN_TSI_FRAC_SECOND ) STRCAT(buf," SQL_FN_TSI_FRAC_SECOND |");
        if (rgbInfoValue &  SQL_FN_TSI_SECOND      ) STRCAT(buf," SQL_FN_TSI_SECOND |");
        if (rgbInfoValue &  SQL_FN_TSI_MINUTE      ) STRCAT(buf," SQL_FN_TSI_MINUTE |");
        if (rgbInfoValue &  SQL_FN_TSI_HOUR        ) STRCAT(buf," SQL_FN_TSI_HOUR |");
        if (rgbInfoValue &  SQL_FN_TSI_DAY         ) STRCAT(buf," SQL_FN_TSI_DAY |");
        if (rgbInfoValue &  SQL_FN_TSI_WEEK        ) STRCAT(buf," SQL_FN_TSI_WEEK |");
        if (rgbInfoValue &  SQL_FN_TSI_MONTH       ) STRCAT(buf," SQL_FN_TSI_MONTH |");
        if (rgbInfoValue &  SQL_FN_TSI_QUARTER     ) STRCAT(buf," SQL_FN_TSI_QUALIFIER |");
        if (rgbInfoValue &  SQL_FN_TSI_YEAR        ) STRCAT(buf," SQL_FN_TSI_YEAR ");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 7.SQL_TIMEDATE_FUNCTIONS !*/
        SPRINTF(buffer, "6.7.SQL_TIMEDATE_FUNCTIONS : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_TIMEDATE_FUNCTIONS, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);
        STRCPY(buf," ");
        if (rgbInfoValue &  SQL_FN_TD_CURDATE       ) STRCAT(buf," SQL_FN_TD_CURDATE |");
        if (rgbInfoValue &  SQL_FN_TD_CURTIME       ) STRCAT(buf," SQL_FN_TD_CURTIME |");
        if (rgbInfoValue &  SQL_FN_TD_DAYNAME       ) STRCAT(buf," SQL_FN_TD_DAYNAME |");
        if (rgbInfoValue &  SQL_FN_TD_DAYOFMONTH    ) STRCAT(buf," SQL_FN_TD_DAYOFMONTH |");
        if (rgbInfoValue &  SQL_FN_TD_DAYOFWEEK     ) STRCAT(buf," SQL_FN_TD_DAYOFWEEK |");
        if (rgbInfoValue &  SQL_FN_TD_DAYOFYEAR     ) STRCAT(buf," SQL_FN_TD_DAYOFYEAR |");
        if (rgbInfoValue &  SQL_FN_TD_HOUR          ) STRCAT(buf," SQL_FN_TD_HOUR |");
        if (rgbInfoValue &  SQL_FN_TD_MINUTE        ) STRCAT(buf," SQL_FN_TD_MINUTE |");
        if (rgbInfoValue &  SQL_FN_TD_MONTH         ) STRCAT(buf," SQL_FN_TD_MONTH |");
        if (rgbInfoValue &  SQL_FN_TD_MONTHNAME     ) STRCAT(buf," SQL_FN_TD_MONTHNAME |");
        if (rgbInfoValue &  SQL_FN_TD_NOW           ) STRCAT(buf," SQL_FN_TD_NOW |");
        if (rgbInfoValue &  SQL_FN_TD_QUARTER       ) STRCAT(buf," SQL_FN_TD_QUARTER |");
        if (rgbInfoValue &  SQL_FN_TD_SECOND        ) STRCAT(buf," SQL_FN_TD_SECOND |");
        if (rgbInfoValue &  SQL_FN_TD_TIMESTAMPADD  ) STRCAT(buf," SQL_FN_TD_TIMESTAMPADD |");
        if (rgbInfoValue &  SQL_FN_TD_TIMESTAMPDIFF ) STRCAT(buf," SQL_FN_TD_TIMESTAMPDIFF |");
        if (rgbInfoValue &  SQL_FN_TD_WEEK          ) STRCAT(buf," SQL_FN_TD_WEEK |");
        if (rgbInfoValue &  SQL_FN_TD_YEAR          ) STRCAT(buf," SQL_FN_TD_YEAR");
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

DoAbort:
        return(0);
#endif
}
/*
 -------------------------------------------------------------------------
| 7.PutConversionInformation
|
|
| Returns:
|
 -------------------------------------------------------------------------
*/
RETCODE PutConversionInformation(HENV henv, HDBC hdbc, HSTMT hstmt)
{
        RETCODE                 rc;
        UCHAR                   buffer[MAX_NAME_LEN*5];
        UCHAR                   buf[MAX_NAME_LEN*5];

        UCHAR                   rgbInfoValueChar[MAX_RGB_VALUE];
        UDWORD                  rgbInfoValue;
        ULONG                   *rgbInfoValueHandle;

        UWORD                   fInfoType;
        SWORD                   cbInfoValueMax;
        SWORD                   pcbInfoValue;
/* ---------------------------------------------------------------------- */
/* ***** 7.Conversion Information */
/* ---------------------------------------------------------------------- */
#if defined(GETINFO_ALL) || defined(GETINFO_OPT7)
        SPRINTF(buffer, " 7.) Conversion Information -->"CRLF);
	PRINTF(buffer);

        /* *** 1.SQL_CONVERT_BIGINT */
        SPRINTF(buffer, "7.1.SQL_COVERT_BIGINT : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_BIGINT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 2.SQL_CONVERT_BINARY */
        SPRINTF(buffer, "7.2.SQL_COVERT_BINARY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_BINARY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 3.SQL_CONVERT_BIT  */
        SPRINTF(buffer, "7.3.SQL_COVERT_BIT : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_BIT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 4.SQL_CONVERT_CHAR */
        SPRINTF(buffer, "7.4.SQL_COVERT_CHAR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_CHAR, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 5.SQL_CONVERT_DATE */
        SPRINTF(buffer, "7.5.SQL_COVERT_DATE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_DATE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 6.SQL_CONVERT_DECIMAL */
        SPRINTF(buffer, "7.6.SQL_COVERT_DECIMAL : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_DECIMAL, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 7.SQL_CONVERT_DOUBLE */
        SPRINTF(buffer, "7.7.SQL_COVERT_DOUBLE : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_DOUBLE, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 8.SQL_CONVERT_FLOAT */
        SPRINTF(buffer, "7.8.SQL_COVERT_FLOAT : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_FLOAT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 9.SQL_CONVERT_INTEGER */
        SPRINTF(buffer, "7.9.SQL_COVERT_INTEGER : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_INTEGER, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 10.SQL_CONVERT_LONGVARBINARY */
        SPRINTF(buffer, "7.10.SQL_COVERT_LONGVARBINARY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_LONGVARBINARY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 11.SQL_CONVERT_LONGVARCHAR */
        SPRINTF(buffer, "7.11.SQL_COVERT_LONGVARCHAR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_LONGVARCHAR, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 12.SQL_CONVERT_NUMERIC */
        SPRINTF(buffer, "7.12.SQL_COVERT_NUMERIC : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_NUMERIC, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 13.SQL_CONVERT_REAL */
        SPRINTF(buffer, "7.13.SQL_COVERT_REAL : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_REAL, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 14.SQL_CONVERT_SMALLINT */
        SPRINTF(buffer, "7.14.SQL_COVERT_SMALLINT : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_SMALLINT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 15.SQL_CONVERT_TIME */
        SPRINTF(buffer, "7.15.SQL_COVERT_TIME : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_TIME, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 16.SQL_CONVERT_TIMESTAMP */
        SPRINTF(buffer, "7.16.SQL_COVERT_TIMESTAMP : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_TIMESTAMP, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 17.SQL_CONVERT_TINYINT */
        SPRINTF(buffer, "7.17.SQL_COVERT_TINYINT : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_TINYINT, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 18.SQL_CONVERT_VARBINARY */
        SPRINTF(buffer, "7.18.SQL_COVERT_VARBINARY : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_VARBINARY, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buffer);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);

        /* *** 19.SQL_CONVERT_VARCHAR */
        SPRINTF(buffer, "7.19.SQL_COVERT_VARCHAR : "CRLF);
	PRINTF(buffer);
        rc = SQLGetInfo(hdbc, SQL_CONVERT_VARCHAR, (PTR) &rgbInfoValue,
                                sizeof(rgbInfoValue), NULL);
	SPRINTF(buffer," SQLGetInfo retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ERROR_CHECK(hdbc);

        CheckConversion(rgbInfoValue, buf);
        SPRINTF(buffer, "Value = '%s'"CRLF,buf);
	PRINTF(buffer);
DoAbort:
        return(0);
#endif
}
/*
 --------------------------------------------------------------------------------
| CheckConversion:
|
|
| Returns:
|
 --------------------------------------------------------------------------------
*/
void CheckConversion(UDWORD rgbInfoValue, UCHAR buffer[MAX_NAME_LEN])
{
 STRCPY(buffer," ");
 if (rgbInfoValue &  SQL_CVT_BIGINT) STRCAT(buffer,"SQL_CVT_BIGINT");
 if (rgbInfoValue & SQL_CVT_BINARY) STRCAT(buffer,"SQL_CVT_BINARY");
 if (rgbInfoValue & SQL_CVT_BIT) STRCAT(buffer,"SQL_CVT_BIT");
 if (rgbInfoValue & SQL_CVT_CHAR) STRCAT(buffer,"SQL_CVT_CHAR");
 if (rgbInfoValue & SQL_CVT_DATE) STRCAT(buffer,"SQL_CVT_DATE");
 if (rgbInfoValue & SQL_CVT_DECIMAL) STRCAT(buffer,"SQL_CVT_DECIMAL");
 if (rgbInfoValue & SQL_CVT_DOUBLE) STRCAT(buffer,"SQL_CVT_DOUBLE");
 if (rgbInfoValue & SQL_CVT_FLOAT) STRCAT(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_INTEGER) STRCAT(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_LONGVARBINARY) STRCAT(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_LONGVARCHAR) STRCAT(buffer,"SQL_CVT_LONGVARBINARY");
 if (rgbInfoValue & SQL_CVT_NUMERIC) STRCAT(buffer,"SQL_CVT_NUMERIC");
 if (rgbInfoValue & SQL_CVT_REAL) STRCAT(buffer,"SQL_CVT_REAL");
 if (rgbInfoValue & SQL_CVT_SMALLINT) STRCAT(buffer,"SQL_CVT_SMALLINT");
 if (rgbInfoValue & SQL_CVT_TIME) STRCAT(buffer,"SQL_CVT_TIME");
 if (rgbInfoValue & SQL_CVT_TIMESTAMP) STRCAT(buffer,"SQL_CVT_TIMESTAMP");
 if (rgbInfoValue & SQL_CVT_TINYINT) STRCAT(buffer,"SQL_CVT_TINYINT");
 if (rgbInfoValue & SQL_CVT_VARBINARY) STRCAT(buffer,"SQL_CVT_VARBINARY");
 if (rgbInfoValue & SQL_CVT_VARCHAR) STRCAT(buffer,"SQL_CVT_VARCHAR");
}
/* ************************************************************************* */
