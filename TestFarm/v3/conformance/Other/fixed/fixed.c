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
#define PROGNAME "FIXED_"
#include "tst.h"

#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFSIZE1 200

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   printf(" sqlstate: %s"CRLF, sqlstate);\
		  printf(" esq_sql_code: %d"CRLF, esq_sql_code);\
		  printf(" error_txt: %s"CRLF, error_txt);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (!STRCMP(sqlstate,"22005") &&\
			!STRCMP(sqlstate,"22003") &&\
			   !STRCMP(sqlstate,"22008") &&\
			      !STRCMP(sqlstate,"07006"))\
			      { exit(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ exit(16); }\
		     break;\
		  default   :\
			  exit(16);\
			  break; } }

#define INIT_CHAR '*'
UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ 302 ] ;
SWORD len_error_txt = 300;
SWORD used_error_txt;
HENV henv   = SQL_NULL_HENV;
HENV FAR * phenv;
HDBC hdbc   = SQL_NULL_HDBC;
HDBC FAR * phdbc;
HSTMT hstmt = SQL_NULL_HSTMT;
HSTMT FAR * phstmt;

UCHAR driverver [ 11 ];
UWORD col;
UCHAR szTestName[NAME_LEN+1];

UCHAR buffer [1024];

#define CHAR_LEN 120
#define BINARY_LEN 1000
#define LONGVARCHAR_LEN 10*CHAR_LEN
UCHAR szCHAR[CHAR_LEN+1]; SQLLEN cbCHAR=SQL_NTS;
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1]; SQLLEN cbLONGVARBINARY;
UCHAR szBINARY[BINARY_LEN+1]; SQLLEN cbBINARY;
SWORD sSHORT; SQLLEN cbSHORT;
SDWORD sLONG; SQLLEN cbLONG;
SFLOAT sFLOAT; SQLLEN cbFLOAT;
SDOUBLE sDOUBLE; SQLLEN cbDOUBLE;
DATE_STRUCT sDATE; SQLLEN cbDATE;
TIME_STRUCT sTIME; SQLLEN cbTIME;
TIMESTAMP_STRUCT sSTAMP; SQLLEN cbSTAMP;

/* default values for C datatypes */
UCHAR szDefCHAR[] = "SQL_C_CHAR"; SQLLEN cbszDefCHAR=sizeof(szDefCHAR)-1;
UCHAR szDefDATE[] = "1994-10-10"; SQLLEN cbszDefDATE=sizeof(szDefDATE)-1;
UCHAR szDefTIME[] = "15:20:23";   SQLLEN cbszDefTIME=sizeof(szDefTIME)-1;
UCHAR szDefSTAMP[] = "1994-10-10 15:20:23"; SQLLEN cbszDefSTAMP=sizeof(szDefSTAMP)-1;
UCHAR szDefNUM[] =  ".123"; SQLLEN cbszDefNUM=sizeof(szDefNUM);
UCHAR szDefBIT[] = "0"; UCHAR sDefBIT = '1'; SQLLEN cbDefBIT = 0;
UCHAR szDefTINY[] = "12"; SCHAR sDefTINY = SQL_C_TINYINT; SQLLEN cbDefTINY = 0;
UCHAR szDefSHORT[] = "3333"; SWORD sDefSHORT = SQL_C_SHORT; SQLLEN cbDefSHORT = 0;
UCHAR szDefLONG[] = "44444"; SDWORD sDefLONG = SQL_C_LONG; SQLLEN cbDefLONG = 0;
UCHAR szDefFLOAT[] = "5.5555"; SFLOAT sDefFLOAT = SQL_C_FLOAT; SQLLEN cbDefFLOAT = 0;
UCHAR szDefDOUBLE[] = "6.66666"; SDOUBLE sDefDOUBLE = 2222.222; SQLLEN cbDefDOUBLE = 0;
UCHAR szDefBINARY[] = "SQL_C_BINARY"; SQLLEN cbszDefBINARY = sizeof(szDefBINARY)-1;
UCHAR szDefHEX[] = "48656c6c6f20776f726C64"; SQLLEN cbszDefHEX = sizeof(szDefHEX)-1;
UCHAR szDefBINNUM[] = "9999"; SQLLEN cbszDefBINNUM = sizeof(szDefBINNUM);
SDOUBLE sDefBINNUM = -77.77; SQLLEN cbsDefBINNUM = sizeof(sDefBINNUM);
DATE_STRUCT sDefDATE[] = {1994, 9, 28}; SQLLEN cbDefDATE = 0;
TIME_STRUCT sDefTIME[] = {9, 35, 58}; SQLLEN cbDefTIME = 0;
TIMESTAMP_STRUCT sDefSTAMP[] = {1994, 9, 28, 9, 35, 58, 30}; SQLLEN cbDefSTAMP = 0;

SQLLEN cbVALUE, *pcbVALUE=&cbVALUE;
PTR psVALUE;
UDWORD cbDataSize;
SCHAR sBUFFER[1024];

typedef struct {
   SWORD  fCTypeIn;         /* C Datatype for SQLSetParam */
   PTR    rgbValueIn;	    /* pointer to Input Parameter */
   SQLLEN *cbValueIn;       /* parameter for SQLSetParam */
   SWORD  fCTypeOut;        /* C Datatype for SQLBinCol */
   PTR    rgbValueOut;      /* pointer to Output Parameter */
   SQLLEN cbSizeofValueOut; /* sizeof Output Parameter (for init with zeros) */
   SDWORD cbValueMax;       /* parameter for SQLBinCol */
   SQLLEN *cbValueOut;      /* parameter for SQLBinCol */
} TestTypesStruct;

static TestTypesStruct TestTypeChar[] = {
{SQL_C_CHAR,      szDefCHAR,  &cbszDefCHAR,   SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),   0, &cbCHAR},
{SQL_C_SHORT,     &sDefSHORT, &cbDefSHORT,    SQL_C_SHORT,     &sSHORT,    sizeof(sSHORT),   0, &cbSHORT},
{SQL_C_LONG,      &sDefLONG,  &cbDefLONG,     SQL_C_LONG,      &sLONG,     sizeof(sLONG),    0, &cbLONG},
{SQL_C_FLOAT,     &sDefFLOAT, &cbDefFLOAT,    SQL_C_FLOAT,     &sFLOAT,    sizeof(sFLOAT),   0, &cbFLOAT},
{SQL_C_DOUBLE,    &sDefDOUBLE,&cbDefDOUBLE,   SQL_C_DOUBLE,    &sDOUBLE,   sizeof(sDOUBLE),  0, &cbDOUBLE},
{SQL_C_BINARY,    szDefBINARY,&cbszDefBINARY, SQL_C_BINARY,    szBINARY,   sizeof(szBINARY), 0, &cbBINARY},
{SQL_C_DATE,      &sDefDATE,  &cbDefDATE,     SQL_C_DATE,      &sDATE,     sizeof(sDATE),    0, &cbDATE},
{SQL_C_TIME,      &sDefTIME,  &cbDefTIME,     SQL_C_TIME,      &sTIME,     sizeof(sTIME),    0, &cbTIME},
{SQL_C_TIMESTAMP, &sDefSTAMP, &cbDefSTAMP,    SQL_C_TIMESTAMP, &sSTAMP,    sizeof(sSTAMP),   0, &cbSTAMP},
{0, 0, 0, 0, 0, 0, 0}
};

static TestTypesStruct TestTypeByte[] = {
{SQL_C_CHAR,      szDefHEX,    &cbszDefHEX,    SQL_C_CHAR,     szCHAR,     sizeof(szCHAR),     0, &cbCHAR},
{SQL_C_BINARY,    szDefBINARY, &cbszDefBINARY, SQL_C_BINARY,   szBINARY,   sizeof(szBINARY),   0, &cbBINARY},
{0, 0, 0, 0, 0, 0, 0}
};

static TestTypesStruct TestTypeNum2_2[] = {
{SQL_C_CHAR,      "+.1",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      ".1",     NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-.1",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+0.1",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0.1",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0.1",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+.12",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      ".12",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-.12",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+0.12",  NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0.12",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0.12",  NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+.123",  NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      ".123",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-.123",  NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+0.123", NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0.123",  NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0.123", NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{0, 0, 0, 0, 0, 0, 0}
};

static TestTypesStruct TestTypeNull2_2[] = {
{SQL_C_CHAR,      "+0",     NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0",      NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0",     NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+.0",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-.0",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+0.",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0.",     NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0.",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "+0.0",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0.0",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0.0",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{0, 0, 0, 0, 0, 0, 0}
};

static TestTypesStruct TestTypeNum10_5[] = {
{SQL_C_CHAR,      "12345.12345",     NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-12345.12345",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "0.123456",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "123456.12345",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-0.123456",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-123456.12345",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{0, 0, 0, 0, 0, 0, 0}
};

static TestTypesStruct TestTypeNum15_0[] = {
{SQL_C_CHAR,      ".1",     NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "1.1",    NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "123456789012345",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{SQL_C_CHAR,      "-123456789012345",   NULL,    SQL_C_CHAR,      szCHAR,     sizeof(szCHAR),     sizeof(szCHAR), &cbCHAR},
{0, 0, 0, 0, 0, 0, 0}
};

typedef struct {
   SWORD fSqlType;
   UCHAR *sTestName;
   UCHAR *sCreateString;
   UDWORD cbColDef;
   SWORD  ibScale;
   TestTypesStruct *pTestTypes;
} TestTabStruct;

static TestTabStruct TestTab[] = {
{SQL_NUMERIC, "FIXED2_2", "create table "TABLE" (n integer, col fixed(2,2))", 100, 0, TestTypeNum2_2 },
{SQL_NUMERIC, "ZERO2_2", "create table "TABLE" (n integer, col fixed(2,2))", 100, 0, TestTypeNull2_2 },
{SQL_NUMERIC, "FIXED10_5", "create table "TABLE" (n integer, col fixed(10,5))", 100, 0, TestTypeNum10_5 },
{SQL_NUMERIC, "FIXED15_0", "create table "TABLE" (n integer, col fixed(15,0))", 100, 0, TestTypeNum15_0 },
{0, NULL, NULL}
};

RETCODE process(SWORD sNUM, SWORD fCTypeIn, SWORD fSqlTypeIn, UDWORD cbColDefIn,
SWORD ibScaleIn, PTR rgbValueIn, SQLLEN *pcbValueIn, SWORD fCTypeOut,
PTR rgbValueOut, SDWORD cbSizeofValueOut, SDWORD cbValueMaxOut, SQLLEN *pcbValueOut);
VOID rgbValueShow(SWORD fCType, PTR rgbValue, SQLLEN *pcbValue);
BOOL IsNumber(SWORD fSqlType);
BOOL IsChar(SWORD fSqlType);
BOOL IsBinary(SWORD fSqlType);

int main(int argc, argv_t argv)
{
  UWORD ipar;
  RETCODE retcode;
  SWORD pccol;
  SWORD nr;
  TestTypesStruct *p;
  TestTabStruct *pTestTab;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);
  (*szTestName) = 0;
  phenv = (HENV FAR *) &henv;
  phdbc = (HDBC FAR *) &hdbc;
  phstmt = (HSTMT FAR *) &hstmt;

  retcode = SQLAllocEnv(phenv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocConnect(henv, phdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
     (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer, " SQLConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  for(pTestTab = TestTab; pTestTab->sTestName != NULL; pTestTab++) {
     if ((*szTestName) != 0) {
	if (STRCMP(szTestName, pTestTab->sTestName)) {
	   continue;
	}
     }
     SPRINTF(buffer, "**** START TEST GROUP %s ****"CRLF, pTestTab->sTestName);
     PRINTF(buffer);
     p = pTestTab->pTestTypes;
     retcode = SQLAllocStmt (hdbc, phstmt);
     retcode = SQLExecDirect (hstmt, "DROP TABLE "TABLE, SQL_NTS);
     STRCPY(buffer, pTestTab->sCreateString);
     retcode = SQLExecDirect (hstmt, buffer, SQL_NTS);
     SPRINTF(buffer, "%s"CRLF, buffer);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
     for(nr = 0; p->fCTypeIn != 0; p++, nr++) {
	retcode = process(nr, p->fCTypeIn, pTestTab->fSqlType, pTestTab->cbColDef,
	   pTestTab->ibScale, p->rgbValueIn, p->cbValueIn,
	   p->fCTypeOut, p->rgbValueOut, p->cbSizeofValueOut, p->cbValueMax, p->cbValueOut);
     }
     retcode = SQLFreeStmt(hstmt, SQL_DROP);
     SPRINTF(buffer, " SQLFreeStmt retcode: %d"CRLF, retcode);
     PRINTF(buffer);
     ERROR_CHECK(hstmt);
     SPRINTF(buffer, "**** END TEST GROUP %s ****"CRLF, pTestTab->sTestName);
     PRINTF(buffer);
  }

  retcode = SQLTransact(henv, hdbc, SQL_ROLLBACK);
  SPRINTF(buffer, " SQLTransact retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

RETCODE process(
SWORD sNUM,
SWORD fCTypeIn,
SWORD fSqlTypeIn,
UDWORD cbColDefIn,
SWORD ibScaleIn,
PTR rgbValueIn,
SQLLEN *pcbValueIn,
SWORD fCTypeOut,
PTR rgbValueOut,
SDWORD cbSizeofValueOut,
SDWORD cbValueMaxOut,
SQLLEN *pcbValueOut)
{
   RETCODE retcode;
   SQLLEN cbNUM = 0;
   SDWORD cbValue;
   /* storage for initialvalues */
   /* store this value(s) and   */
   /* and restore back after executing the fetch */
   cbValue = *pcbValueOut;

   SPRINTF(buffer, "**** START TEST (%d) ****"CRLF, sNUM);
   PRINTF(buffer);
   SPRINTF(buffer, "fCTypeIn = %d, fSqlTypeIn = %d, cbColDefIn = %lu, ibScaleIn = %d"CRLF,
      fCTypeIn, fSqlTypeIn, cbColDefIn, ibScaleIn);
   PRINTF(buffer);
   rgbValueShow(fCTypeIn, rgbValueIn, pcbValueIn);

   retcode = SQLSetParam (hstmt, 1, SQL_C_SHORT, SQL_SMALLINT, (UDWORD) sizeof(sNUM), 0, &sNUM, &cbNUM);
   ERROR_CHECK(hstmt);

   retcode = SQLSetParam (hstmt, 2, fCTypeIn, fSqlTypeIn, cbColDefIn, ibScaleIn, rgbValueIn, pcbValueIn);
   SPRINTF(buffer, " SQLSetParam retcode: %d"CRLF, retcode);
   PRINTF(buffer);
   ERROR_CHECK(hstmt);
   if (retcode == SQL_SUCCESS) {

      retcode = SQLExecDirect (hstmt, "insert "TABLE" values (?, ?)", SQL_NTS);
      SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
      PRINTF(buffer);
      ERROR_CHECK(hstmt);
      retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
      ERROR_CHECK(hstmt);

      retcode = SQLExecDirect (hstmt, "select col from "TABLE" where n = ?", SQL_NTS);
      SPRINTF(buffer, " SQLExecDirect retcode: %d"CRLF, retcode);
      PRINTF(buffer);
      ERROR_CHECK(hstmt);

      MEMSET(rgbValueOut, INIT_CHAR, cbSizeofValueOut);
      retcode = SQLBindCol (hstmt, 1, fCTypeOut, rgbValueOut, cbValueMaxOut, pcbValueOut);
      SPRINTF(buffer, " SQLBindCol retcode: %d"CRLF, retcode);
      PRINTF(buffer);
      ERROR_CHECK(hstmt);
      if (retcode == SQL_SUCCESS) {
	 retcode = SQLFetch (hstmt);
	 SPRINTF(buffer, " SQLFetch retcode: %d"CRLF, retcode);
	 PRINTF(buffer);
	 ERROR_CHECK(hstmt);

	 SPRINTF(buffer, "fCTypeOut = %d, cbValueMaxOut = %lu"CRLF, fCTypeOut, cbValueMaxOut);
	 PRINTF(buffer);
	 rgbValueShow(fCTypeOut, rgbValueOut, pcbValueOut);
	 *pcbValueOut = cbValue;

      }
   }
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
   ERROR_CHECK(hstmt);

   return (retcode);
}

VOID rgbValueShow(SWORD fCType, PTR rgbValue, SQLLEN *pcbValue)
{
   UCHAR localbuf[BUFSIZE1];
   SDWORD len;
   switch (fCType) {
      case (SQL_C_CHAR): {
	 len = (pcbValue == NULL) ? STRLEN(rgbValue) : (*pcbValue == SQL_NTS) ? STRLEN(rgbValue) : *pcbValue;
         if (len < 0)
	    SPRINTF(localbuf, "SQL_C_CHAR : SQL_NO_DATA");
	 else
	 if (len < BUFSIZE1) {
	    SPRINTF(localbuf, "SQL_C_CHAR = '%.*s'", len, rgbValue);
	 }
	 else {
	    SPRINTF(localbuf, "SQL_C_CHAR = '%.*s"CRLF"........"CRLF"%.*s'",
	       BUFSIZE1/3, rgbValue, BUFSIZE1/3, ((char *)rgbValue+len-BUFSIZE1/3));
	 }
	 break;
      }
      case (SQL_C_LONG): {
	 SPRINTF(localbuf, "SQL_C_LONG = %ld", * (SDWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_SHORT): {
	 SPRINTF(localbuf, "SQL_C_SHORT = %d", * (SWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_DOUBLE): {
	 SPRINTF(localbuf, "SQL_C_DOUBLE = %.18f", * (SDOUBLE FAR *)rgbValue);
	 break;
      }
      case (SQL_C_FLOAT): {
	 SPRINTF(localbuf, "SQL_C_FLOAT = %.18f", * (SFLOAT FAR *)rgbValue);
	 break;
      }
      case (SQL_C_TINYINT): {
	 SWORD sword;
	 sword = (SWORD)* (SCHAR FAR *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_TINYINT = %d", sword);
	 break;
      }
      case (SQL_C_DATE): {
	 DATE_STRUCT *sD;
	 sD = (DATE_STRUCT *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_DATE = %d,%d,%d", sD->year, sD->month, sD->day);
	 break;
      }
      case (SQL_C_TIME): {
	 TIME_STRUCT *sT;
	 sT = (TIME_STRUCT *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_TIME = %d,%d,%d", sT->hour, sT->minute, sT->second );
	 break;
      }
      case (SQL_C_TIMESTAMP): {
	 TIMESTAMP_STRUCT *sTS;
	 sTS = (TIMESTAMP_STRUCT *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_TIMESTAMP = %d,%d,%d,%d,%d,%d,%ld", sTS->year, sTS->month, sTS->day,
	    sTS->hour, sTS->minute, sTS->second, sTS->fraction );
	 break;
      }
      case (SQL_C_BINARY): {
	 UDWORD i;
	 UDWORD bufsize;
	 UCHAR hchar[4];
	 UCHAR FAR *p;
	 p = (UCHAR FAR *)rgbValue;
	 bufsize = BUFSIZE1 / 3;
	 STRCPY(localbuf, "SQL_C_BINARY = ");
	 len = (pcbValue == NULL) ? STRLEN(rgbValue) : *pcbValue;
         if (len < 0)
	    SPRINTF(localbuf, "SQL_C_BINARY : SQL_NO_DATA");
	 else
	 if (len < bufsize) {
	    for ( i=1; i <= len; i++) {
	       SPRINTF(hchar, "%3x", *p++);
	       STRCAT(localbuf, hchar);
	    }
	 }
	 else {
	    for ( i=1; i <= bufsize/3; i++) {
	       SPRINTF(hchar, "%3x", *p++);
	       STRCAT(localbuf, hchar);
	    }
	    STRCAT(localbuf, CRLF"........"CRLF);
	    for ( i=len-bufsize/3; i <= len; i++) {
	       SPRINTF(hchar, "%3x", *p++);
	       STRCAT(localbuf, hchar);
	    }
	 }
	 break;
      }
      case (SQL_C_BIT): {
	 SPRINTF(localbuf, "SQL_C_BIT = *** can't display ***");
	 break;
      }
      case (SQL_C_DEFAULT): {
	 SPRINTF(localbuf, "SQL_C_DEFAULT = *** can't display ***");
	 break;
      }
   }
   STRCAT(localbuf, CRLF);
   PRINTF(localbuf);
   if (pcbValue == NULL)
      SPRINTF(buffer, "pcbValue is NULL"CRLF);
   else
      SPRINTF(buffer, "*pcbValue = %ld"CRLF, *pcbValue);
   PRINTF(buffer);
   return;
}

BOOL IsNumber(SWORD fSqlType)
{
   BOOL ret = FALSE;
   switch (fSqlType) {
      case (SQL_SMALLINT):{}
      case (SQL_INTEGER):{}
      case (SQL_NUMERIC):{}
      case (SQL_DECIMAL):{}
      case (SQL_REAL):{}
      case (SQL_FLOAT):{}
      case (SQL_DOUBLE):{
	 ret = TRUE;
	 break;
      }
   }
   return(ret);
}

BOOL IsChar(SWORD fSqlType)
{
   BOOL ret = FALSE;
   switch (fSqlType) {
      case (SQL_CHAR):{}
      case (SQL_VARCHAR):{}
      case (SQL_LONGVARCHAR):{
	 ret = TRUE;
	 break;
      }
   }
   return(ret);
}

BOOL IsBinary(SWORD fSqlType)
{
   BOOL ret = FALSE;
   switch (fSqlType) {
      case (SQL_BINARY):{}
      case (SQL_VARBINARY):{}
      case (SQL_LONGVARBINARY):{
	 ret = TRUE;
	 break;
      }
   }
   return(ret);
}
