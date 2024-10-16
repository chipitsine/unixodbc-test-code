/*
*****************************************************************************
** SQL Datenbanksysteme GmbH
**
** Autor        : Harun Eren
**
** Program      : getfunc.c.OC
**
** Usage        : --
** Option       : --
**
** Purpose      : Test der Funktion SQLGetFunctions
**
** Date         : 12.10.95
** l.aenderung  : 14:56pm 03-14-1995 (Harun)
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
#define PROGNAME "getfunc"
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

/* ------------------------------------------------------------------------ */
#define TESTNAME "SQLGetFunctions"
#define TABLE ""

#define MAX_NAME_LEN 512

#define STR_LEN 128+1
#define REM_LEN 254+1

#define NULL_VALUE "<NULL>"

/* ----------------------------------------------------------------------- */
void Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer);

/*
 ----------------------------------------------------------------------------
| DoSQLGetFunctions:
|
|
| Returns:
|
 -----------------------------------------------------------------------------
*/
int main(int argc, argv_t argv)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------ */
	HENV henv   = SQL_NULL_HENV;
	HENV FAR * phenv;
	HDBC hdbc   = SQL_NULL_HDBC;
	HDBC FAR * phdbc;
	HSTMT hstmt = SQL_NULL_HSTMT;
	HSTMT FAR * phstmt;

/* ---------------------------------------------------------------------- */
	UWORD			fFunction;
	UWORD			pfExists[100];

	UCHAR                   buf[MAX_NAME_LEN+1];

	SWORD  			cAbort=0;
/* --------------------------------------------------------------------har- */

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

/* ------------------------------------------------------------------------- */
/* Parameters :
   Core		:
		 1.SQL_API_SQLALLOCCONNECT
		 2.SQL_API_SQLALLOCENV
		 3.SQL_API_SQLALLOCSTMT
		 4.SQL_API_SQLBINDCOL
		 5.SQL_API_SQLCANCEL
		 6.SQL_API_SQLCOLATTRIBUTES
		 7.SQL_API_SQLCONNECT
		 8.SQL_API_SQLDESCRIBECOL
		 9.SQL_API_SQLDISCONNECT
		10.SQL_API_SQLERROR
		11.SQL_API_SQLEXECDIRECT
		12.SQL_API_SQLFETCH
		13.SQL_API_SQLFREECONNECT
		14.SQL_API_SQLFREEENV
		15.SQL_API_SQLFREESTMT
		16.SQL_API_SQLGETCURSORNAME
		17.SQL_API_SQLNUMRESULTCOLS
		18.SQL_API_SQLPREPARE
		19.SQL_API_SQLROWCOUNT
		20.SQL_API_SQLSETCURSORNAME
		21.SQL_API_SQLSETPARAM
		22.SQL_API_SQLTRANACT
   ---------------------------------------------------------------------------
   Level 1    	:
		 1.SQL_API_SQLBINDPARAMETER	(ODBC 2.0)
		 2.SQL_API_SQLCOLUMNS
		 3.SQL_API_SQLDRIVERCONNECT
		 4.SQL_API_SQLGETCONNECTOPTION
		 5.SQL_API_SQLGETDATA
		 6.SQL_API_SQLGETFUNCTIONS
		 7.SQL_API_SQLGETINFO
		 8.SQL_API_SQLGETSTMTOPTION
		 9.SQL_API_SQLGETTYPEINFO
		10.SQL_API_SQLPARAMDATA
		11.SQL_API_SQLPUTDATA
		12.SQL_API_SQLSETCONNECTOPTION
		13.SQL_API_SQLSETSTMTOPTION
		14.SQL_API_SQLSPECIALCOLUMNS
		15.SQL_API_SQLSTATISTICS
		16.SQL_API_SQLTABLES
   ---------------------------------------------------------------------------
   Level 2    	:
		 1.SQL_API_SQLBROWSECONNECT
		 2.SQL_API_SQLCOLUMNPRIVILEGES
		 3.SQL_API_SQLDATASOURCES
		 4.SQL_API_SQLDESCRIBEPARAM
		 5.SQL_API_SQLDRIVERS		(ODBC 2.0)
		 6.SQL_API_SQLEXTENDEDFETCH
		 7.SQL_API_SQLFOREIGNKEYS
		 8.SQL_API_SQLMORERESULTS
		 9.SQL_API_SQLNATIVESQL
		10.SQL_API_SQLNUMPARAMS
		11.SQL_API_SQLPARAMOPTIONS
		12.SQL_API_SQLPRIMARYKEYS
		13.SQL_API_SQLPROCEDURECOLUMNS
		14.SQL_API_SQLPROCEDURES
		15.SQL_API_SQLSETPOS
		16.SQL_API_SQLSETSCROLLOPTIONS
		17.SQL_API_SQLTABLEPRIVILEGES
*/
/* --- SQLGetFunctions --------------------------------------------------- */

        SPRINTF(buffer,"Conformance Core --> "CRLF);
	PRINTF(buffer);

	/* --- Core Conformance -------------------------------------------- */
	fFunction = SQL_API_SQLALLOCCONNECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLALLOCCONNECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLALLOCENV;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLALLOCENV = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLALLOCSTMT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLALLOCSTMT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLBINDCOL;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLBINDCOL = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLCANCEL;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLCANCEL = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLCOLATTRIBUTES;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLCOLATTRIBUTES = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLCONNECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLCONNECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLDESCRIBECOL;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLDESCRIBECOL = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLDISCONNECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLDISCONNECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLERROR;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLERROR = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLEXECDIRECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLEXECDIRECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLFETCH;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLFETCH = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLFREECONNECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLFREECONNECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLFREEENV;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLFREEENV = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLFREESTMT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLFREESTMT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETCURSORNAME;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETCURSORNAME = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLNUMRESULTCOLS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLNUMRESULTCOLS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPREPARE;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPREPARE = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLROWCOUNT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLROWCOUNT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSETCURSORNAME;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSETCURSORNAME = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSETPARAM;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSETPARAM = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLTRANSACT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLTRANSACT = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* ----------------------------------------------------------------- */
        SPRINTF(buffer,"Conformance Level 1. --> "CRLF);
	PRINTF(buffer);

	/* --- Level 1 Conformance ----------------------------------------- */
	/* ***** ODBC 2.0
	fFunction = SQL_API_SQLBINDPARAMETER;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLBINDPARAMETER = '%s'"CRLF,buf);
	PRINTF(buffer);
	*/
	fFunction = SQL_API_SQLCOLUMNS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLCOLUMNS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLDRIVERCONNECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
	SPRINTF(buffer,"SQL_API_SQLDRIVERCONNECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETCONNECTOPTION;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETCONNECTOPTION = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETDATA;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETDATA = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETFUNCTIONS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETFUNCTIONS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETINFO;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETINFO = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETSTMTOPTION;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETSTMTOPTION = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLGETTYPEINFO;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLGETTYPEINFO = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPARAMDATA;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPARAMDATA = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPUTDATA;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPUTDATA = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSETCONNECTOPTION;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSETCONNECTOPTION = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSETSTMTOPTION;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSETSTMTOPTION = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSPECIALCOLUMNS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSPECIALCOLUMNS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSTATISTICS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSTATISTICS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLTABLES;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLTABLES = '%s'"CRLF,buf);
	PRINTF(buffer);

	/* ----------------------------------------------------------------- */
        SPRINTF(buffer,"Conformance Level 2. --> "CRLF);
	PRINTF(buffer);

	/* --- Level 2 Conformance ----------------------------------------- */
	fFunction = SQL_API_SQLBROWSECONNECT;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLBROWSECONNECT = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLCOLUMNPRIVILEGES;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLCOLUMNPRIVILEGES = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLDATASOURCES;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLDATASOURCES = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLDESCRIBEPARAM;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLDESCRIBEPARAM = '%s'"CRLF,buf);
	PRINTF(buffer);
	/* **** ODBC 2.0
	fFunction = SQL_API_SQLDRIVERS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLDRIVERS = '%s'"CRLF,buf);
	PRINTF(buffer);
	*/
	fFunction = SQL_API_SQLEXTENDEDFETCH;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLEXTENDEDFETCH = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLFOREIGNKEYS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLFOREIGNKEYS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLMORERESULTS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLMORERESULTS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLNATIVESQL;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLNATIVESQL = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLNUMPARAMS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
	SPRINTF(buffer,"SQL_API_SQLNUMPARAMS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPARAMOPTIONS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPARAMOPTIONS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPRIMARYKEYS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPRIMARYKEYS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPROCEDURECOLUMNS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPROCEDURECOLUMNS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLPROCEDURES;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLPROCEDURES = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSETPOS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSETPOS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLSETSCROLLOPTIONS;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLSETSCROLLOPTIONS = '%s'"CRLF,buf);
	PRINTF(buffer);

	fFunction = SQL_API_SQLTABLEPRIVILEGES;
	rc = SQLGetFunctions(*phdbc, fFunction, pfExists);
        Get_BoolValue( (SWORD) *pfExists, buf);
        SPRINTF(buffer,"SQL_API_SQLTABLEPRIVILEGES = '%s'"CRLF,buf);
	PRINTF(buffer);

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
/*
 -----------------------------------------------------------------------------
| Get_BoolValue:
|
|
| Returns:
 -----------------------------------------------------------------------------
*/
void Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer)
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
}
/* ------------------------------------------------------------------------- */
