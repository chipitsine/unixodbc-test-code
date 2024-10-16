/*



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
#define PROGNAME "INS1"
#include "tst.h"
#include "stdlib.h"

#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFSIZE 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   RETCODE errret;\
			   errret = SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
			 PRINTF(buffer);\
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
			 PRINTF(buffer);\
		  SPRINTF(buffer, " error_txt: %s"CRLF, error_txt);\
			 PRINTF(buffer);\
		     if (errret == SQL_SUCCESS)\
                  switch (retcode) {\
		  case(SQL_NO_DATA_FOUND) :\
		  case(SQL_NEED_DATA) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (!STRCMP(sqlstate,"22005") &&\
			!STRCMP(sqlstate,"22003") &&\
			   !STRCMP(sqlstate,"22008") &&\
			      !STRCMP(sqlstate,"07006"))\
			      { return(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ return(16); }\
		     break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR driverver [ 11 ];
#define MAX_COL 30
SWORD col;
UCHAR szColName[MAX_COL][NAME_LEN+1];
SWORD pcbColName[MAX_COL], pfSqlType[MAX_COL],
pibScale[MAX_COL], pfNullable[MAX_COL];
SQLULEN pcbColDef[MAX_COL];

UCHAR buffer [BUFSIZE];
UCHAR szSTRING1[] = "ABCDEFGHIJKLMNOPQRSTUV";
UCHAR szSTRING2[] = "1234567";
UCHAR szSTRING3[] = "abcdefghijklmnopqrstuv";

#define CHAR_LEN 120
#define LONGVARCHAR_LEN 30000
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szCHAR1[CHAR_LEN+1];
UCHAR FAR *szLONGVARCHAR;
UCHAR FAR *szLONGVARCHAR2;
double  sDOUBLE;
double  sDECIMAL;
SQLLEN cbDOUBLE, cbCHAR, cbCHAR1, cbLONGVARCHAR, cbLONGVARCHAR2, cbDECIMAL;
#define BOUNDVAL 999

#define SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue) \
SQLSetParam (hstmt, ipar, fCType, fSqlType, cbColDef, ibScale, rgbValue, pcbValue)

void fetchout();

HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;
SQLLEN *pcbValue, cbValue;
SQLLEN *pcbValue1, cbValue1;
SQLLEN *pcbValue2, cbValue2;
PTR rgbValue;

int main(int argc, argv_t argv)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code=0;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UWORD ipar;
  SCHAR *ptr;

  RETCODE retcode;
  RETCODE retp;
  SWORD pccol;
  SQLLEN pToken;       /* must be as long as a pointer! */

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

  szLONGVARCHAR=(UCHAR FAR *)MALLOC(LONGVARCHAR_LEN+1);
  szLONGVARCHAR2=(UCHAR FAR *)MALLOC(LONGVARCHAR_LEN+1);
  if (!szLONGVARCHAR || !szLONGVARCHAR2) {
     SPRINTF(buffer, "malloc fails program terminated."CRLF);
     PRINTF(buffer);
     return(255);
  }

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
		       (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, phstmt);
  retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "create table "TABLE" (l1 long, c char(10), l2 long)", SQL_NTS);
  retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

  ipar = 1;
  cbValue = 0;
  pcbValue = &cbValue;
  retcode = SQLBindParameter (hstmt, ipar, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, 0, 0, (PTR*)ipar, 0, pcbValue);
  SPRINTF(buffer," SQLBindParameter (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 2;
  cbValue1 = 0;
  pcbValue1 = &cbValue1;
  retcode = SQLBindParameter (hstmt, ipar, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, (UDWORD) 10, 0, (PTR*)ipar, 0, pcbValue1);
  SPRINTF(buffer," SQLBindParameter (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  ipar = 3;
  cbValue2 = 0;
  pcbValue2 = &cbValue2;
  retcode = SQLBindParameter (hstmt, ipar, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, ipar, 0, (PTR*)ipar, 0, pcbValue2);
  SPRINTF(buffer," SQLBindParameter (%d) retcode: %s"CRLF, ipar, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  SPRINTF(buffer, "insert ... testlen"CRLF);
  PRINTF(buffer);
  *pcbValue = SQL_DATA_AT_EXEC;
  *pcbValue1 = SQL_DATA_AT_EXEC;
  *pcbValue2 = SQL_DATA_AT_EXEC;
  retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?, ?)", SQL_NTS);
  SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLParamData (hstmt, (PTR *)&pToken);
  SPRINTF(buffer, " SQLParamData(%d) retcode: %s"CRLF, pToken, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  while (retcode == SQL_NEED_DATA) {
     switch (pToken) {
	case 1: {
	   ptr = (SCHAR FAR *)szSTRING1;
	   retp = SQLPutData (hstmt, szSTRING1, SQL_NTS);
	   SPRINTF(buffer, " SQLPutData retcode: %s"CRLF, cnvret(retp));
	   PRINTF(buffer);
	   SPRINTF(buffer, " pcbValue = %ld"CRLF, *pcbValue);
	   PRINTF(buffer);
	   break;
	}
	case 2: {
	   retp = SQLPutData (hstmt, szSTRING2, SQL_NTS);
	   SPRINTF(buffer, " SQLPutData retcode: %s"CRLF, cnvret(retp));
	   PRINTF(buffer);
	   SPRINTF(buffer, " pcbValue = %ld"CRLF, *pcbValue);
	   PRINTF(buffer);
	   break;
	}
	case 3: {
	   retp = SQLPutData (hstmt, szSTRING3, SQL_NTS);
	   SPRINTF(buffer, " SQLPutData retcode: %s"CRLF, cnvret(retp));
	   PRINTF(buffer);
	   SPRINTF(buffer, " pcbValue = %ld"CRLF, *pcbValue);
	   PRINTF(buffer);
	   break;
	}
	default : {
	   SPRINTF(buffer, " error default"CRLF);
	   PRINTF(buffer);
	   break;
	}
     }
     retcode = SQLParamData (hstmt, (PTR *) &pToken);
     SPRINTF(buffer, " SQLParamData(%d) retcode: %s"CRLF, pToken, cnvret(retcode));
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
  }

  retcode = SQLPrepare (hstmt, "select l1, c, l2 from "TABLE, SQL_NTS);
  SPRINTF(buffer, " SQLPrepare retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);
  if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
      return 1;
  }; /* if */

  retcode = SQLNumResultCols (hstmt, &pccol);
  SPRINTF(buffer, " SQLNumResultCols retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  SPRINTF(buffer, " pccol : %d"CRLF, pccol);
  PRINTF(buffer);

  SPRINTF(buffer, " COLNAME LEN TYPE PRECISION SCALE NULLABLE"CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
  PRINTF(buffer);
  for (col=1; col <= pccol; col++ ) {
     retcode = SQLDescribeCol (hstmt, col, szColName[col], NAME_LEN+1,
	&pcbColName[col], &pfSqlType[col], &pcbColDef[col], &pibScale[col], &pfNullable[col]);
     SPRINTF(buffer, " %s %3d %4d %9ld %5d %8d "CRLF, szColName[col], pcbColName[col], pfSqlType[col], pcbColDef[col], pibScale[col], pfNullable[col]);
     PRINTF(buffer);
  }

  retcode = SQLExecute (hstmt);
  SPRINTF(buffer, " SQLExecute retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  fetchout();

  retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
  SPRINTF(buffer, " SQLFreeStmt retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(hstmt);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

void fetchout()
{
  RETCODE retcode=SQL_SUCCESS;
  UDWORD len, lbufs;
  int try;
  while (retcode == SQL_SUCCESS) {
     cbCHAR=SQL_NULL_DATA;
     cbLONGVARCHAR=SQL_NULL_DATA;
     MEMSET(szLONGVARCHAR, 0, LONGVARCHAR_LEN+1);
     cbLONGVARCHAR2=SQL_NULL_DATA;
     MEMSET(szLONGVARCHAR2, 0, LONGVARCHAR_LEN+1);
     retcode = SQLFetch (hstmt);
     if ( retcode != SQL_NO_DATA_FOUND ) {
	SPRINTF(buffer, " SQLFetch retcode: %s"CRLF, cnvret(retcode));
	PRINTF(buffer);
	for (try=0; retcode == SQL_SUCCESS && try < 3; try++) {
	   retcode = SQLGetData (hstmt, 1, SQL_C_CHAR, szLONGVARCHAR, LONGVARCHAR_LEN+1, &cbLONGVARCHAR);
	   SPRINTF(buffer, " SQLGetData(%ld) retcode: %s"CRLF, cbLONGVARCHAR, cnvret(retcode));
	   PRINTF(buffer);
	}
	retcode = SQL_SUCCESS;
	for (try=0; retcode == SQL_SUCCESS && try < 3; try++) {
	   retcode = SQLGetData (hstmt, 2, SQL_C_CHAR, szCHAR, sizeof(szCHAR), &cbCHAR);
	   SPRINTF(buffer, " SQLGetData(%ld) retcode: %s"CRLF, cbCHAR, cnvret(retcode));
	   PRINTF(buffer);
	}
	retcode = SQL_SUCCESS;
	for (try=0; retcode == SQL_SUCCESS && try < 3; try++) {
	   retcode = SQLGetData (hstmt, 3, SQL_C_CHAR, szLONGVARCHAR2, LONGVARCHAR_LEN+1, &cbLONGVARCHAR2);
	   SPRINTF(buffer, " SQLGetData(%ld) retcode: %s"CRLF, cbLONGVARCHAR2, cnvret(retcode));
	   PRINTF(buffer);
	}
	len = (cbLONGVARCHAR == SQL_NTS) ? STRLEN(szLONGVARCHAR) : cbLONGVARCHAR;
	SPRINTF(buffer, "cbLONGVARCHAR=%ld"CRLF, len);
	PRINTF(buffer);
	lbufs = BUFSIZE-20;
	if (len < lbufs) {
	   STRCPY(buffer, "'");
	   STRNCAT(buffer, szLONGVARCHAR, (size_t)len);
	   STRCAT(buffer, "'"CRLF);
	}
	else {
	   STRCPY(buffer, "'");
	   STRNCAT(buffer, szLONGVARCHAR, (size_t)lbufs/3);
	   STRCAT(buffer, CRLF"......"CRLF);
	   STRNCAT(buffer, szLONGVARCHAR+len-lbufs/3, (size_t)lbufs/3);
	   STRCAT(buffer, "'"CRLF);
	}
	PRINTF(buffer);

	SPRINTF(buffer, "szCHAR(%ld) = '%s'"CRLF, cbCHAR, szCHAR);
	PRINTF(buffer);

	len = (cbLONGVARCHAR2 == SQL_NTS) ? STRLEN(szLONGVARCHAR2) : cbLONGVARCHAR2;
	SPRINTF(buffer, "cbLONGVARCHAR2=%ld"CRLF, len);
	PRINTF(buffer);
	if (len < lbufs) {
	   STRCPY(buffer, "'");
	   STRNCAT(buffer, szLONGVARCHAR2, (size_t)len);
	   STRCAT(buffer, "'"CRLF);
	}
	else {
	   STRCPY(buffer, "'");
	   STRNCAT(buffer, szLONGVARCHAR2, (size_t)lbufs/3);
	   STRCAT(buffer, CRLF"......"CRLF);
	   STRNCAT(buffer, szLONGVARCHAR2+len-lbufs/3, (size_t)lbufs/3);
	   STRCAT(buffer, "'"CRLF);
	}
	PRINTF(buffer);
     }
  }
}
