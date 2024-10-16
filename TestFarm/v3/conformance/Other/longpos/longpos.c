/*!**********************************************************************

  module: longpos.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Positional reading of long data from table with two long
               columns

  -----------------------------------------------------------------------





    ========== licence begin  GPL
    Copyright (c) 1998-2004 SAP AG

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



************************************************************************/

#define PROGNAME "longpos"

#define NUM_VALUES 1
#define CHAR_LEN 150000

#include "tst.h"

#define O3UTIL_TEXT_LEN  1024
#define O3UTIL_TEXT_BUFFER_LEN  2*O3UTIL_TEXT_LEN
#define O3UTIL_SQLSTATE_LEN 10
#define O3UTIL_STMT_LEN 1024

char longBuf[CHAR_LEN];

/* Insert long values */
RETCODE InsertLong( HSTMT hstmt, UDWORD longLen1, UDWORD longLen2 );

/* show table contents */
RETCODE ShowTable( HSTMT hstmt, UDWORD longPos1, UDWORD longPos2 );

#define BUFSIZE 1000
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code=0;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
UCHAR buffer [BUFSIZE];

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

HENV henv   = SQL_NULL_HENV;
HDBC hdbc   = SQL_NULL_HDBC;
HSTMT hstmt = SQL_NULL_HSTMT;

int main(int argc, argv_t argv) {
    RETCODE retcode;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    InitLongBuf( longBuf, CHAR_LEN );

    retcode = SQLAllocEnv(&henv);
    SPRINTF(buffer, " SQLAllocEnv retcode: %s"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLAllocConnect(henv, &hdbc);
    SPRINTF(buffer, " SQLAllocConnect retcode: %s"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLConnect( hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER,
                          (SWORD) SQL_NTS, ODBCPASSW, (SWORD) SQL_NTS );
    SPRINTF(buffer, " SQLConnect retcode: %s"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(NULL);

    retcode = SQLAllocStmt( hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocStmt \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */
    retcode = SQLExecDirect( hstmt,
                             "create table longpos ( a long ascii, b long ascii, c int )", SQL_NTS );
    SPRINTF(buffer, " SQLExecDirect retcode: %s"CRLF, cnvret(retcode));
    PRINTF(buffer);
    ERROR_CHECK(hstmt);

    retcode = SQLFreeStmt( hstmt, SQL_DROP );
    retcode = SQLAllocStmt( hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocStmt \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */
    retcode = InsertLong( hstmt, 100000, 100000 );
    if (retcode != SQL_SUCCESS) {
        goto exit;
    }; /* if */

    retcode = ShowTable( hstmt, 10000, 0 );
    retcode = ShowTable( hstmt, 32666, 0 );
    retcode = ShowTable( hstmt, 32667, 0 );
    retcode = ShowTable( hstmt, 38000, 0 );
    retcode = ShowTable( hstmt, 68000, 0 );
    retcode = ShowTable( hstmt, 100000, 0 );

    retcode = ShowTable( hstmt, 97000, 0 );

exit:
    retcode = SQLFreeStmt( hstmt, SQL_DROP );
    retcode = SQLAllocStmt( hdbc, &hstmt );
    SPRINTF( textBuffer,
             "Main  \t\t -- SQLAllocStmt \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    /*
    retcode = SQLExecDirect( hstmt, "drop table longpos", SQL_NTS );
    */
    retcode = SQLDisconnect( hdbc );

    EndTest( PROGNAME );
    return 0;
}; /* main */



RETCODE InsertLong( HSTMT hstmt, UDWORD longLen1, UDWORD longLen2 )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR   textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR   stmtStr[O3UTIL_STMT_LEN];
    int     i;
    char    binValue1[CHAR_LEN];
    char    binValue2[CHAR_LEN];
    int     intValue;
    SQLLEN  binIndi1;
    SQLLEN  binIndi2;

    SPRINTF( stmtStr, "insert into %s values (?,?,?)", PROGNAME );
    SPRINTF( textBuffer, "InsertLong\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLPrepare( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLPrepare \t\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 1,
                                SQL_PARAM_INPUT,
                                SQL_C_CHAR,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &binValue1,
                                CHAR_LEN, &binIndi1 );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindParameter(1) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindParameter( hstmt, 2,
                                SQL_PARAM_INPUT,
                                SQL_C_CHAR,
                                SQL_LONGVARCHAR,
                                CHAR_LEN, 0,
                                &binValue2,
                                CHAR_LEN, &binIndi2 );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */


    retcode = SQLBindParameter( hstmt, 3,
                                SQL_PARAM_INPUT,
                                SQL_C_SHORT,
                                SQL_INTEGER,
                                0, 0,
                                &intValue,
                                0, NULL );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLBindparameter(2) \t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    memcpy( binValue1, longBuf, longLen1 );
    binValue1[longLen1] = '\0';
    memcpy( binValue2, longBuf, longLen2 );
    binValue2[longLen2] = '\0';
    SPRINTF( textBuffer, "InsertLong\t -- Insert %d Values"CRLF, NUM_VALUES );
    PRINTF( textBuffer );
    for (i=0; (i < NUM_VALUES && i < CHAR_LEN); i++) {
        binIndi1 = strlen( binValue1 );
        binIndi2 = strlen( binValue2 );
        intValue = i;
        retcode = SQLExecute( hstmt );
        SPRINTF( textBuffer,
                 "InsertLong\t -- SQLExecute (No %d)\t\t retcode: %s"CRLF,
                 i, cnvret( retcode ));
        PRINTF( textBuffer );
        ERROR_CHECK( hstmt );
        if (retcode != SQL_SUCCESS) {
            return retcode;
        }; /* if */
    }; /* for */
    /* Insert NULL Value */
#if 0
    intValue++;
    binIndi = SQL_NULL_DATA;
    retcode = SQLExecute( hstmt );
    ErrorCheck( SQL_HANDLE_STMT, hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */
#endif
    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "InsertLong\t -- SQLFreeStmt(CLOSE) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );

    return retcode;
}; /* InsertLong */

RETCODE ShowTable( HSTMT hstmt, UDWORD longPos1, UDWORD longPos2 )
{
    RETCODE retcode = SQL_SUCCESS;
    UCHAR     textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    UCHAR     stmtStr[O3UTIL_STMT_LEN];
    char      binValue1[CHAR_LEN];
    char      binValue2[CHAR_LEN];
    UWORD     intValue;
    SQLLEN    binIndi1;
    SQLLEN    binIndi2;
    SQLLEN    intIndi;

    /* Prepare select stmt */
    SPRINTF( stmtStr, "select c, b, a from %s order by c", PROGNAME );
    SPRINTF( textBuffer, "ShowTable\t -- %s"CRLF, stmtStr );
    PRINTF( textBuffer );

    retcode = SQLExecDirect( hstmt, stmtStr, strlen( stmtStr ) );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLExecDirect\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLBindCol( hstmt, 1,
                          SQL_C_SHORT, &intValue, 0, &intIndi );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLBindCol(2) \t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    /* show content of table */
    PRINTF( "ShowTable\t -- Content of table"CRLF );
    do {
        retcode = SQLFetch( hstmt );
        SPRINTF( textBuffer,
                 "ShowTable\t -- SQLFetch \t\t\t retcode: %s"CRLF,
                 cnvret( retcode ));
        PRINTF( textBuffer );
        ERROR_CHECK( hstmt );
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLSetStmtOption( hstmt, 3003 /* SQL_ATTR_READ_LONG_POS */,
                                      longPos1 );
            SPRINTF( textBuffer,
                     "ShowTable\t -- SetStmtAttr(READ_LONG_POS=%d)\t retcode: %s"CRLF,
                     longPos1, cnvret( retcode ));
            PRINTF( textBuffer );
            ERROR_CHECK( hstmt );
            SPRINTF( textBuffer, "ShowTable\t -- Column 1; startpos %d"CRLF,
                     longPos1 );
            PRINTF( textBuffer );
            do {
                retcode = SQLGetData( hstmt, 2,
                                      SQL_C_CHAR, binValue1, 20000,
                                      &binIndi1 );
                SPRINTF( textBuffer,
                         "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
                         cnvret( retcode ));
                PRINTF( textBuffer );
                if (retcode != SQL_NO_DATA_FOUND) {
                    ERROR_CHECK( hstmt );
                }; /* if */
                if (retcode == SQL_SUCCESS
                    || retcode == SQL_SUCCESS_WITH_INFO) {
                    if (binIndi1 != SQL_NULL_DATA) {
                        if (binIndi1>0) {
                            binValue1[binIndi1] = '\0';
                        }; /* if */
                        ShowLongChar( binValue1 );
                    } else {
                        printf("NULL\n");
                    }; /* else */
                    SPRINTF( textBuffer, "Fetch Row: %d %d strlen=%d "CRLF,
                             intValue, binIndi1, STRLEN(binValue1) );
                    PRINTF( textBuffer );
                }; /* if */
            } while (retcode == SQL_SUCCESS_WITH_INFO);
            retcode = SQLSetStmtOption( hstmt, 3003 /* SQL_ATTR_READ_LONG_POS */,
                                        longPos2 );
            SPRINTF( textBuffer,
                     "ShowTable\t -- SetStmtAttr(READ_LONG_POS=%d)\t retcode: %s"CRLF,
                     longPos2, cnvret( retcode ));
            PRINTF( textBuffer );
            ERROR_CHECK( hstmt );
            SPRINTF( textBuffer, "ShowTable\t -- Column 2; startpos: %d"CRLF,
                     longPos2 );
            PRINTF( textBuffer );
            do {
                retcode = SQLGetData( hstmt, 3,
                                      SQL_C_CHAR, binValue2, 20000,
                                      &binIndi2 );
                SPRINTF( textBuffer,
                         "ShowTable\t -- SQLGetData \t\t\t retcode: %s"CRLF,
                         cnvret( retcode ));
                PRINTF( textBuffer );
                if (retcode != SQL_NO_DATA_FOUND) {
                    ERROR_CHECK( hstmt );
                }; /* if */
                if (retcode == SQL_SUCCESS
                    || retcode == SQL_SUCCESS_WITH_INFO) {
                    if (binIndi2 != SQL_NULL_DATA) {
                        if (binIndi2>0) {
                            binValue2[binIndi2] = '\0';
                        }; /* if */
                        ShowLongChar( binValue2 );
                    } else {
                        printf("NULL\n");
                    }; /* else */
                    SPRINTF( textBuffer, "Fetch Row: %d %d strlen=%d "CRLF,
                             intValue, binIndi2, STRLEN(binValue2) );
                    PRINTF( textBuffer );
                }; /* if */
            } while (retcode == SQL_SUCCESS_WITH_INFO);

            if (retcode == SQL_NO_DATA_FOUND) {
                retcode = SQL_SUCCESS; /* we dont want to stop now */
            }; /* if */
        } else {
            ERROR_CHECK( hstmt );
        }; /* else */
    } while (retcode == SQL_SUCCESS);

    retcode = SQLFreeStmt( hstmt, SQL_CLOSE );
    SPRINTF( textBuffer,
             "ShowTable\t -- SQLFreeStmt(CLOSE)\t\t retcode: %s"CRLF,
             cnvret( retcode ));
    PRINTF( textBuffer );
    ERROR_CHECK( hstmt );

    return retcode;
}; /* ShowTable */
