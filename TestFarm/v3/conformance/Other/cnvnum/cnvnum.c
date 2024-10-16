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
#define PROGNAME "CNVNUM"
#include <tst.h>

#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif
#define BUFSIZE 200

#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS) {\
			   SQLError(henv, hdbc, hstmtx,\
			      sqlstate, &esq_sql_code, error_txt,\
				    len_error_txt, &used_error_txt);\
			   SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
			      PRINTF(buffer);\
		  SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
		     PRINTF(buffer);\
		  SPRINTF(buffer, " error_txt: %s"CRLF, error_txt);\
		     PRINTF(buffer);\
                  switch (retcode) {\
		  case(100) :\
			  break;\
                  case(SQL_ERROR) : \
		     if (STRCMP(sqlstate,"22005"))\
			if (STRCMP(sqlstate,"07006"))\
			{ return(16); }\
		     break;\
                  case(SQL_SUCCESS_WITH_INFO) : \
		       if (STRCMP(sqlstate,"01004"))\
		          if (STRCMP(sqlstate,"22001"))\
			{ exit(16); }\
		     break;\
		  default   :\
			  return(16);\
			  break; } }

UCHAR sqlstate  [ 10 ];
SDWORD esq_sql_code;
UCHAR error_txt  [ 101 ] ;
SWORD len_error_txt = 100;
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
UCHAR szCHAR[CHAR_LEN+1];
UCHAR szLONGVARCHAR[LONGVARCHAR_LEN+1];
UCHAR szBINARY[BINARY_LEN+1];
SWORD sSMALLINT;

SDWORD *pcbDOUBLE, *pcbSMALLINT, *pcbCHAR, *pcbBINARY, *pcbLONGVARCHAR, *pcbDECIMAL;

/* default values for C datatypes */
UCHAR szDefCHAR[] = "SQL_C_CHAR";
UCHAR szDefDATE[] = "1994-10-10";
UCHAR szDefTIME[] = "15:20:23";
UCHAR szDefSTAMP[] = "1994-10-10 15:20:23";
UCHAR szDefNUM[] =  "12345";
UCHAR szDefBIT[] = "0"; UCHAR sDefBIT = '1'; SQLLEN cbBIT = 0;
UCHAR szDefTINY[] = "12"; SCHAR sDefTINY = SQL_C_TINYINT; SQLLEN cbTINY = 0;
UCHAR szDefSTINY[] = "-100"; SCHAR sDefSTINY = -100; SQLLEN cbSTINY = 0;
UCHAR szDefUTINY[] = "254"; unsigned char sDefUTINY = 254; SQLLEN cbUTINY = 0;
UCHAR szDefSHORT[] = "3333"; SWORD sDefSHORT = 5555; SQLLEN cbSHORT = 0;
UCHAR szDefSSHORT[] = "-30000"; SWORD sDefSSHORT = -30000; SQLLEN cbSSHORT = 0;
UCHAR szDefUSHORT[] = "60000"; UWORD sDefUSHORT = 60000; SQLLEN cbUSHORT = 0;
UCHAR szDefLONG[] = "44444"; SDWORD sDefLONG = 4444; SQLLEN cbLONG = 0;
UCHAR szDefSLONG[] = "-2000000"; SDWORD sDefSLONG = -2000000; SQLLEN cbSLONG = 0;
UCHAR szDefSLONGSMALL[] = "-20000"; SDWORD sDefSLONGSMALL = -20000;
UCHAR szDefULONG[] = "40000000"; UDWORD sDefULONG =  40000000; SQLLEN cbULONG = 0;
UCHAR szDefFLOAT[] = "5.5555"; SFLOAT sDefFLOAT = 5.5555F; SQLLEN cbFLOAT = 0;
UCHAR szDefDOUBLE[] = "6.66666"; SDOUBLE sDefDOUBLE = 2222.222; SQLLEN cbDOUBLE = 0;
UCHAR szDefBINARY[50] = "SQL_C_BINARY"; SQLLEN cbBINARY = sizeof(szDefBINARY);
UCHAR szDefBINNUM[50] = "9999"; SQLLEN cbBINNUM = sizeof(szDefBINNUM);
DATE_STRUCT sDefDATE[] = {1994, 9, 28}; SQLLEN cbDATE = 0;
TIME_STRUCT sDefTIME[] = {9, 35, 58}; SQLLEN cbTIME = 0;
TIMESTAMP_STRUCT sDefSTAMP[] = {1994, 9, 28, 9, 35, 58, 30}; SQLLEN cbSTAMP = 0;

SQLLEN cbVALUE, *pcbVALUE=&cbVALUE;
PTR psVALUE;
UDWORD cbDataSize;
SCHAR sBUFFER[1024];

typedef struct {
   UCHAR *szNAME;
   SWORD cbType;
   PTR sVALUE;
   PTR sNUM;
   SQLLEN *pcbVALUE;
   UDWORD cbDataSize;
   UDWORD cbDataSizeN;
} DefTabStruct;

static DefTabStruct DefTab[] = {
{"SQL_C_CHAR",      SQL_C_CHAR,      szDefCHAR,   szDefNUM,	NULL,      sizeof(sBUFFER),	 sizeof(sBUFFER)},
{"SQL_C_BIT",       SQL_C_BIT,	     szDefBIT,    &sDefBIT,	&cbBIT,    sizeof(szDefBIT),	 sizeof(sDefBIT)},
{"SQL_C_TINYINT",   SQL_C_TINYINT,   szDefTINY,   &sDefTINY,	&cbTINY,   sizeof(szDefTINY),	 sizeof(sDefTINY)},
{"SQL_C_STINYINT",  SQL_C_STINYINT,  szDefSTINY,  &sDefSTINY,	&cbSTINY,  sizeof(szDefSTINY),	 sizeof(sDefSTINY)},
{"SQL_C_uTINYINT",  SQL_C_UTINYINT,  szDefUTINY,  &sDefUTINY,	&cbUTINY,  sizeof(szDefUTINY),	 sizeof(sDefUTINY)},
{"SQL_C_SHORT",     SQL_C_SHORT,     szDefSHORT,  &sDefSHORT,  	&cbSHORT,  sizeof(szDefSHORT),	 sizeof(sDefSHORT)},
{"SQL_C_SSHORT",    SQL_C_SSHORT,    szDefSSHORT, &sDefSSHORT, 	&cbSSHORT, sizeof(szDefSSHORT),	 sizeof(sDefSSHORT)},
{"SQL_C_USHORT",    SQL_C_USHORT,    szDefUSHORT, &sDefUSHORT, 	&cbUSHORT, sizeof(szDefUSHORT),	 sizeof(sDefUSHORT)},
{"SQL_C_LONG",      SQL_C_LONG,	     szDefLONG,   &sDefLONG,   	&cbLONG,   sizeof(szDefLONG),	 sizeof(sDefLONG)},
{"SQL_C_SLONG",     SQL_C_SLONG,     szDefSLONG,  &sDefSLONG,  	&cbSLONG,  sizeof(szDefSLONG),	 sizeof(sDefSLONG)},
{"SQL_C_SLONG",     SQL_C_SLONG,     szDefSLONGSMALL,  &sDefSLONGSMALL,&cbSLONG,  sizeof(szDefSLONGSMALL), sizeof(sDefSLONGSMALL)},
{"SQL_C_ULONG",     SQL_C_ULONG,     szDefULONG,  &sDefULONG,  	&cbULONG,  sizeof(szDefULONG),	 sizeof(sDefULONG)},
{"SQL_C_FLOAT",     SQL_REAL,        szDefFLOAT,  &sDefFLOAT,  	&cbFLOAT,  sizeof(szDefFLOAT),	 sizeof(sDefFLOAT)},
{"SQL_C_DOUBLE",    SQL_FLOAT,       szDefDOUBLE, &sDefDOUBLE, 	&cbDOUBLE, sizeof(szDefDOUBLE), sizeof(sDefDOUBLE)},
{"SQL_C_DOUBLE",    SQL_DOUBLE,      szDefDOUBLE, &sDefDOUBLE, 	&cbDOUBLE, sizeof(szDefDOUBLE), sizeof(sDefDOUBLE)},
{"SQL_C_BINARY",    SQL_C_BINARY,    szDefBINARY, szDefBINARY, 	&cbBINARY, sizeof(szDefBINARY), sizeof(szDefBINARY)},
{"SQL_C_DATE",      SQL_C_DATE,	     szDefDATE,   sDefDATE,   	&cbDATE,   sizeof(szDefDATE),  sizeof(sDefDATE)},
{"SQL_C_TIME",      SQL_C_TIME,	     szDefTIME,   sDefTIME,   	&cbTIME,   sizeof(szDefTIME),	 sizeof(sDefTIME)},
{"SQL_C_TIMESTAMP", SQL_C_TIMESTAMP, szDefSTAMP,  sDefSTAMP,  	&cbSTAMP,  sizeof(szDefSTAMP),  sizeof(sDefSTAMP)},
{"SQL_C_VARCHAR",   SQL_VARCHAR,     szDefCHAR,   szDefCHAR,   	NULL,      sizeof(sBUFFER),	 sizeof(sBUFFER)},
{"SQL_C_LONGVAR",   SQL_LONGVARCHAR, szDefCHAR,   szDefCHAR,   	NULL,      sizeof(sBUFFER),     sizeof(sBUFFER)},
{NULL, 0, NULL, NULL}};

typedef struct {
   SWORD fCTypeIn;
   SWORD fCTypeOut;
   SWORD fSqlType;
} TestTypesStruct;

static TestTypesStruct TestTypeNum[] = {
{SQL_C_CHAR,      SQL_C_CHAR,      SQL_CHAR},
{SQL_C_TINYINT,     SQL_C_TINYINT,     SQL_SMALLINT},
{SQL_C_STINYINT,    SQL_C_STINYINT,    SQL_SMALLINT},
{SQL_C_UTINYINT,    SQL_C_UTINYINT,    SQL_SMALLINT},
{SQL_C_SHORT,     SQL_C_SHORT,     SQL_SMALLINT},
{SQL_C_SSHORT,    SQL_C_SSHORT,    SQL_SMALLINT},
{SQL_C_USHORT,    SQL_C_USHORT,    SQL_SMALLINT},
{SQL_C_LONG,      SQL_C_LONG,      SQL_INTEGER},
{SQL_C_SLONG,     SQL_C_SLONG,     SQL_INTEGER},
{SQL_C_ULONG,     SQL_C_ULONG,     SQL_INTEGER},
{SQL_C_FLOAT,     SQL_C_FLOAT,     SQL_REAL},
{SQL_C_DOUBLE,    SQL_C_DOUBLE,    SQL_DOUBLE},
{SQL_C_BINARY,    SQL_C_BINARY,    SQL_BINARY},
#ifdef EMPTY
{SQL_C_, SQL_C_, SQL_},
#endif
{0, 0, 0}
};

typedef struct {
   SWORD fSqlType;
   UCHAR *sTestName;
   UCHAR *sCreateString;
   TestTypesStruct *pTestTypes;
} TestTabStruct;

static TestTabStruct TestTab[] = {
{SQL_SMALLINT, "SQL_SMALLINT", "create table "TABLE" (n integer, col smallint)", TestTypeNum},
{SQL_INTEGER, "SQL_INTEGER", "create table "TABLE" (n integer, col integer)", TestTypeNum},
{SQL_REAL, "SQL_REAL", "create table "TABLE" (n integer, col real(7))", TestTypeNum},
{SQL_FLOAT, "SQL_FLOAT", "create table "TABLE" (n integer, col float)", TestTypeNum},
{SQL_DOUBLE, "SQL_DOUBLE", "create table "TABLE" (n integer, col double precision)", TestTypeNum},
{0, NULL, NULL}
};

RETCODE process(SWORD fCTypeIn, SWORD fSqlTypeIn, UDWORD cbColDefIn,
SWORD ibScaleIn, PTR rgbValueIn, SQLLEN *pcbValueIn, SWORD fCTypeOut,
PTR rgbValueOut, SDWORD cbValueMaxOut, SQLLEN *pcbValueOut);
VOID rgbValueShow(SWORD fCType, PTR rgbValue, SQLLEN *pcbValue, PTR rgbValueOld, SQLLEN *pcbValueOld);
PTR GetDefVALUE(SWORD fCType, SWORD fSqlType, SQLLEN *cbVALUE, UDWORD *cbDataSize);
BOOL IsNumber(SWORD fSqlType);
BOOL IsChar(SWORD fSqlType);
BOOL IsBinary(SWORD fSqlType);

int main(int argc, argv_t argv)
{
  RETCODE retcode;
  TestTypesStruct *p;
  TestTabStruct *pTestTab;

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);
  (*szTestName) = 0;
#ifndef WIN
  if ( argc > 1 )
  {
     sscanf(argv[1], "%s", szTestName);
  }
#endif
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
     for(; p->fCTypeIn != 0 && p->fCTypeOut != 0 && p->fSqlType; p++) {
	if (psVALUE = GetDefVALUE(p->fCTypeIn, pTestTab->fSqlType, pcbVALUE, &cbDataSize)) {
	   retcode = process(p->fCTypeIn, pTestTab->fSqlType, (UDWORD) 100, 0, psVALUE, pcbVALUE,
	      p->fCTypeOut, sBUFFER, (UDWORD) cbDataSize, &cbVALUE);
	}
	else
	   break;
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
SWORD fCTypeIn,
SWORD fSqlTypeIn,
UDWORD cbColDefIn,
SWORD ibScaleIn,
PTR rgbValueIn,
SQLLEN *pcbValueIn,
SWORD fCTypeOut,
PTR rgbValueOut,
SDWORD cbValueMaxOut,
SQLLEN *pcbValueOut)
{
   RETCODE retcode;
   SWORD sNUM;
   SQLLEN cbNUM = 0;
   sNUM = fCTypeIn;

   SPRINTF(buffer, "**** START TEST ****"CRLF);
   PRINTF(buffer);
   SPRINTF(buffer, "fCTypeIn = %d, fSqlTypeIn = %d, cbColDefIn = %lu, ibScaleIn = %d"CRLF,
      fCTypeIn, fSqlTypeIn, cbColDefIn, ibScaleIn);
   PRINTF(buffer);
   rgbValueShow(fCTypeIn, rgbValueIn, pcbValueIn, rgbValueIn, pcbValueIn);

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

      MEMSET(rgbValueOut, '*', cbValueMaxOut);
      retcode = SQLBindCol (hstmt, 1, fCTypeOut, rgbValueOut, cbValueMaxOut, pcbValueOut);
      SPRINTF(buffer, " SQLBindCol retcode: %d"CRLF, retcode);
      PRINTF(buffer);
      ERROR_CHECK(hstmt);
      if (retcode == SQL_SUCCESS) {

	 if (pcbValueOut != NULL)
	    *pcbValueOut = 99;

	 retcode = SQLFetch (hstmt);
	 SPRINTF(buffer, " SQLFetch retcode: %d"CRLF, retcode);
	 PRINTF(buffer);
	 ERROR_CHECK(hstmt);

	 SPRINTF(buffer, "fCTypeOut = %d, cbValueMaxOut = %lu"CRLF, fCTypeOut, cbValueMaxOut);
	 PRINTF(buffer);
	 rgbValueShow(fCTypeOut, rgbValueOut, pcbValueOut, rgbValueIn, pcbValueIn);
      }
   }
   retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
   ERROR_CHECK(hstmt);

   return (retcode);
}

VOID rgbValueShow(SWORD fCType, PTR rgbValue, SQLLEN *pcbValue, PTR rgbValueOld, SQLLEN *pcbValueOld)
{
   UCHAR localbuf[BUFSIZE];
   SDWORD len;
   switch (fCType) {
      case (SQL_C_CHAR): {
	 len = (pcbValue == NULL) ? STRLEN(rgbValue) : *pcbValue;
         if (len < 0)
	    SPRINTF(localbuf, "SQL_C_CHAR : SQL_NO_DATA");
	 else
	 if (len < BUFSIZE)
	    SPRINTF(localbuf, "SQL_C_CHAR = '%.*s'", len, rgbValue);
	 else
	    SPRINTF(localbuf, "SQL_C_CHAR = '%.*s"CRLF"........"CRLF"%.*s'",
	       BUFSIZE/3, rgbValue, BUFSIZE/3, ((char *)rgbValue+len-BUFSIZE/3));
	 break;
      }
      case (SQL_C_SLONG): {
	 SPRINTF(localbuf, "SQL_C_SLONG = %d", * (SDWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_ULONG): {
	 SPRINTF(localbuf, "SQL_C_SLONG = %u", * (UDWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_LONG): {
	 SPRINTF(localbuf, "SQL_C_LONG = %d", * (SDWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_SSHORT): {
	 SPRINTF(localbuf, "SQL_C_SSHORT = %d", * (SWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_SHORT): {
	 SPRINTF(localbuf, "SQL_C_SHORT = %d", * (SWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_USHORT): {
	 SPRINTF(localbuf, "SQL_C_USHORT = %u", * (UWORD FAR *)rgbValue);
	 break;
      }
      case (SQL_C_DOUBLE): {
	 SPRINTF(localbuf, "SQL_C_DOUBLE = %.13f", * (SDOUBLE FAR *)rgbValue);
	 break;
      }
      case (SQL_C_FLOAT): {
	 SPRINTF(localbuf, "SQL_C_FLOAT = %.13f", * (SFLOAT FAR *)rgbValue);
	 break;
      }
      case (SQL_C_STINYINT): {
	 SWORD sword;
	 sword = (SCHAR)* (SCHAR FAR *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_STINYINT = %d", sword);
	 break;
      }
      case (SQL_C_TINYINT): {
	 SWORD sword;
	 sword = (SCHAR)* (SCHAR FAR *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_TINYINT = %d", sword);
	 break;
      }
      case (SQL_C_UTINYINT): {
	 unsigned char uchar;
	 uchar = (UCHAR)* (UCHAR FAR *)rgbValue;
	 SPRINTF(localbuf, "SQL_C_UTINYINT = %u", (UWORD)uchar);
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
	 char *p;
	 char *p1;
	 p = (char *)rgbValue;
	 p1 = (char *)rgbValueOld;
	 STRCPY(localbuf, "SQL_C_BINARY = OK");
	 len = (pcbValue == NULL) ? STRLEN(rgbValue) : *pcbValue;
         if (len < 0)
	    SPRINTF(localbuf, "SQL_C_BINARY : SQL_NO_DATA");
	 else
	 for ( i=1; i <= len; i++) {
	   if (*p != *p1) {
	     STRCPY(localbuf, "SQL_C_BINARY = NOT OK");
	     break;
	   }
	   p++;p1++;
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

PTR GetDefVALUE(SWORD fCType, SWORD fSqlType, SQLLEN *cbVALUE, UDWORD *cbDataSize)
{
   DefTabStruct *p;
   for(p=DefTab; p->szNAME != NULL; p++) {
      if (p->cbType == fCType) {
	 switch (fCType) {
	    case (SQL_C_BINARY): {
	       for (p=DefTab; p->szNAME != NULL; p++) {
		  if (p->cbType == fSqlType) {
		     pcbVALUE = p->pcbVALUE;
		     *cbVALUE  = *pcbVALUE;
		     *cbDataSize = p->cbDataSizeN;
		     if (pcbVALUE != NULL)
			*pcbVALUE=*cbDataSize;
		     return (p->sNUM);
		     break;
		  }
	       }
	       break;
	    }
	    case (SQL_C_CHAR): {
	       for (p=DefTab; p->szNAME != NULL; p++) {
		  if (p->cbType == fSqlType) {
		     pcbVALUE = p->pcbVALUE;
		     *cbVALUE  = *pcbVALUE;
		     *cbDataSize = p->cbDataSize;
		     if (pcbVALUE != NULL)
			*pcbVALUE=*cbDataSize;
		     return (p->sVALUE);
		     break;
		  }
	       }
	       break;
	    }
	    default : {
	       pcbVALUE = p->pcbVALUE;
	       *cbDataSize = p->cbDataSizeN;
	       return (p->sNUM);
	       break;
	    }
	 } /* end switch */
      } /* end if */
   } /* end for */
   return (NULL);
}
