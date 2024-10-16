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
#define PROGNAME "ASYNCMT"
#include <tst.h>
#ifndef WIN32
#include <pthread.h>
#endif

#define TABLE PROGNAME

#ifndef DEFAULTS
#define USERNAME "SUPERDBA"
#define PASSWORD "ADMIN"
#define DBNODE   "sqlprec:D"

#define ODBCUSER  "ODBC"
#define ODBCPASSW "ODBC"
#endif

#define BUFSIZE 1000
#define ERROR_CHECK(hstmtx)      if (retcode != SQL_SUCCESS && retcode != SQL_STILL_EXECUTING) {\
                           SQLError(SQL_NULL_HENV, SQL_NULL_HDBC, hstmtx,\
                              sqlstate, &esq_sql_code, error_txt,\
                                    sizeof(error_txt), &used_error_txt);\
                           SPRINTF(buffer, " sqlstate: %s"CRLF, sqlstate);\
                           PRINTF(buffer);\
                           SPRINTF(buffer, " esq_sql_code: %ld"CRLF, esq_sql_code);\
                           PRINTF(buffer);\
                           SPRINTF(buffer," error_txt: %s"CRLF, error_txt);\
                           PRINTF(buffer);\
                  switch (retcode) {\
                  case(SQL_NO_DATA_FOUND) :\
                  case(SQL_STILL_EXECUTING) :\
                          break;\
                  case(SQL_ERROR) : \
                       if (STRCMP(sqlstate,"S0002"))\
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

UCHAR buffer [BUFSIZE];

HENV henv   = SQL_NULL_HENV;

typedef struct {
   WORD cbFunction;
   UCHAR szQualifier[32];
   UCHAR szTableName[32];
   UCHAR szColumnName[32];
   UCHAR szP1[132];
   SDWORD cbP1;
   UCHAR szP2[32];
   SDWORD cbP2;
   WORD cbSuccess;
} TestValues;

TestValues TestCase[] = {
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE"X", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE"X (i int, c char(10), primary key (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE" (i int, c char(10), foreign key (i) references "TABLE"X (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "grant select (i) on "TABLE" TO public", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create unique index ind"TABLE" on "TABLE" (c)", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE"X values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE" values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLPREPARE, "", "", "", "select * from "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECUTE, "", "", "", "", 0, "", 0, FALSE},
{SQL_API_SQLTABLES, "", TABLE, "", "TABLE", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNS, "", TABLE, "%", "", 0, "", 0, FALSE},
{SQL_API_SQLSTATISTICS, "", TABLE, "", "", SQL_INDEX_ALL, "", SQL_ENSURE, FALSE},
{SQL_API_SQLTABLEPRIVILEGES, "", TABLE, "", "", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNPRIVILEGES, "", TABLE, "I", "", 0, "", 0, FALSE},
{SQL_API_SQLPRIMARYKEYS, "", TABLE"X", "", "", 0, "", 0, FALSE},
{SQL_API_SQLFOREIGNKEYS, "", TABLE"X", "", "", 0, TABLE, 0, FALSE},
{SQL_API_SQLPROCEDURES, "", "PROC1", "", "", 0, "", 0, FALSE},
{SQL_API_SQLPROCEDURECOLUMNS, "", "PROC1", "P1", "", 0, "", 0, FALSE},
{0, "", "", "", "", 0, "", 0, FALSE},
};

#undef TABLE
#define TABLE PROGNAME"1"
TestValues TestCase1[] = {
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE"X", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE"X (i int, c char(10), primary key (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE" (i int, c char(10), foreign key (i) references "TABLE"X (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "grant select (i) on "TABLE" TO public", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create unique index ind"TABLE" on "TABLE" (c)", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE"X values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE" values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLPREPARE, "", "", "", "select * from "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECUTE, "", "", "", "", 0, "", 0, FALSE},
{SQL_API_SQLTABLES, "", TABLE, "", "TABLE", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNS, "", TABLE, "%", "", 0, "", 0, FALSE},
{SQL_API_SQLSTATISTICS, "", TABLE, "", "", SQL_INDEX_ALL, "", SQL_ENSURE, FALSE},
{SQL_API_SQLTABLEPRIVILEGES, "", TABLE, "", "", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNPRIVILEGES, "", TABLE, "I", "", 0, "", 0, FALSE},
{SQL_API_SQLPRIMARYKEYS, "", TABLE"X", "", "", 0, "", 0, FALSE},
{SQL_API_SQLFOREIGNKEYS, "", TABLE"X", "", "", 0, TABLE, 0, FALSE},
{SQL_API_SQLPROCEDURES, "", "PROC1", "", "", 0, "", 0, FALSE},
{SQL_API_SQLPROCEDURECOLUMNS, "", "PROC1", "P1", "", 0, "", 0, FALSE},
{0, "", "", "", "", 0, "", 0, FALSE},
};

#undef TABLE
#define TABLE PROGNAME"2"
TestValues TestCase2[] = {
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE"X", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE"X (i int, c char(10), primary key (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE" (i int, c char(10), foreign key (i) references "TABLE"X (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "grant select (i) on "TABLE" TO public", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create unique index ind"TABLE" on "TABLE" (c)", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE"X values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE" values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLPREPARE, "", "", "", "select * from "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECUTE, "", "", "", "", 0, "", 0, FALSE},
{SQL_API_SQLTABLES, "", TABLE, "", "TABLE", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNS, "", TABLE, "%", "", 0, "", 0, FALSE},
{SQL_API_SQLSTATISTICS, "", TABLE, "", "", SQL_INDEX_ALL, "", SQL_ENSURE, FALSE},
{SQL_API_SQLTABLEPRIVILEGES, "", TABLE, "", "", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNPRIVILEGES, "", TABLE, "I", "", 0, "", 0, FALSE},
{SQL_API_SQLPRIMARYKEYS, "", TABLE"X", "", "", 0, "", 0, FALSE},
{SQL_API_SQLFOREIGNKEYS, "", TABLE"X", "", "", 0, TABLE, 0, FALSE},
{SQL_API_SQLPROCEDURES, "", "PROC1", "", "", 0, "", 0, FALSE},
{SQL_API_SQLPROCEDURECOLUMNS, "", "PROC1", "P1", "", 0, "", 0, FALSE},
{0, "", "", "", "", 0, "", 0, FALSE},
};

#undef TABLE
#define TABLE PROGNAME"3"
TestValues TestCase3[] = {
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "drop table "TABLE"X", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE"X (i int, c char(10), primary key (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create table "TABLE" (i int, c char(10), foreign key (i) references "TABLE"X (i))", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "grant select (i) on "TABLE" TO public", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "create unique index ind"TABLE" on "TABLE" (c)", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE"X values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLEXECDIRECT, "", "", "", "insert into "TABLE" values (1, 'abc')", 0, "", 0, FALSE},
{SQL_API_SQLPREPARE, "", "", "", "select * from "TABLE, 0, "", 0, FALSE},
{SQL_API_SQLEXECUTE, "", "", "", "", 0, "", 0, FALSE},
{SQL_API_SQLTABLES, "", TABLE, "", "TABLE", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNS, "", TABLE, "%", "", 0, "", 0, FALSE},
{SQL_API_SQLSTATISTICS, "", TABLE, "", "", SQL_INDEX_ALL, "", SQL_ENSURE, FALSE},
{SQL_API_SQLTABLEPRIVILEGES, "", TABLE, "", "", 0, "", 0, FALSE},
{SQL_API_SQLCOLUMNPRIVILEGES, "", TABLE, "I", "", 0, "", 0, FALSE},
{SQL_API_SQLPRIMARYKEYS, "", TABLE"X", "", "", 0, "", 0, FALSE},
{SQL_API_SQLFOREIGNKEYS, "", TABLE"X", "", "", 0, TABLE, 0, FALSE},
{SQL_API_SQLPROCEDURES, "", "PROC1", "", "", 0, "", 0, FALSE},
{SQL_API_SQLPROCEDURECOLUMNS, "", "PROC1", "P1", "", 0, "", 0, FALSE},
{0, "", "", "", "", 0, "", 0, FALSE},
};

TestValues *AllTest[] = {TestCase, TestCase1, TestCase2, TestCase3, NULL};

int AsyncExecute(HSTMT hstmt, TestValues *TestCase);
int AsyncThreadProc(TestValues *TestCase);
#ifndef WIN32
#define SQLGETTHREADID thr_self
#else
#define SQLGETTHREADID GetCurrentThreadId
#endif
#define MAXTHREADS 100

int DropTables ()
{
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UCHAR buffer [BUFSIZE];
  RETCODE retcode;

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);

  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);

#undef TABLE
#define TABLE PROGNAME
  retcode = SQLExecDirect (hstmt, "drop table "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "drop table "TABLE"X", SQL_NTS);
#undef TABLE
#define TABLE PROGNAME"1"
  retcode = SQLExecDirect (hstmt, "drop table "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "drop table "TABLE"X", SQL_NTS);
#undef TABLE
#define TABLE PROGNAME"2"
  retcode = SQLExecDirect (hstmt, "drop table "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "drop table "TABLE"X", SQL_NTS);
#undef TABLE
#define TABLE PROGNAME"3"
  retcode = SQLExecDirect (hstmt, "drop table "TABLE, SQL_NTS);
  retcode = SQLExecDirect (hstmt, "drop table "TABLE"X", SQL_NTS);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);
}


int main(int argc, argv_t argv)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  RETCODE retcode;
  TestValues **p;
  DWORD i, ThreadCount;
#ifndef WIN32
  pthread_t t_id[MAXTHREADS];
  void *status;
#else
  DWORD dwNULL;
  HANDLE t_id[MAXTHREADS];
  DWORD status;
#endif

  ProcessCmdArguments(argc,argv);

  SPRINTF(buffer, PROGNAME" running."CRLF);
  PRINTF(buffer);

  retcode = SQLAllocEnv(&henv);
  SPRINTF(buffer, " SQLAllocEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  ThreadCount=0;
  for (p=AllTest,i=0; *p != NULL; p++,i++) {
#ifndef WIN32
    int error;
    error = pthread_create(&t_id[i], NULL, (void *(*)(void *))AsyncThreadProc, *p);
    if (error)
      return(error);
#else
    t_id[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AsyncThreadProc, *p, 0, &dwNULL);
    if (!t_id[i])
      return(-1);
#endif
    ThreadCount++;
    SPRINTF(buffer, " AsyncThreadProc retcode: %d"CRLF, retcode);
    /*    PRINTF(buffer);    not thread safe */
  }
  SPRINTF(buffer, " All threads are created wait for termitation."CRLF);
  /* PRINTF(buffer); not thread safe */

  for(i=0; i < ThreadCount; i++) {
#ifndef WIN32
    int ret;
#endif
    SPRINTF(buffer, " join %ld."CRLF, t_id[i]);
    /* PRINTF(buffer); not thread safe */
#ifndef WIN32
    ret = pthread_join(t_id[i], &status);
    if (ret)
      return(ret);
#else
    for(;;) {
      GetExitCodeThread(t_id[i], &status);
      if (status == STILL_ACTIVE)
	Sleep(0);
      else
	break;
    }
#endif
  }
  for (p=AllTest,i=0; *p != NULL; i++,p++) {
    TestValues *p1;
    int j;
    for(p1=(TestValues *)*p,j=0;p1->cbFunction;j++,p1++) {
      if (p1->cbSuccess == FALSE) {
	SPRINTF(buffer, "Thread %d, Test %d on %s failed"CRLF, i, j, GetODBCFunctionName(p1->cbFunction));
	PRINTF(buffer);
      }
      else {
	SPRINTF(buffer, "Thread %d, Test %d on %s: ok"CRLF, i, j, GetODBCFunctionName(p1->cbFunction));
	PRINTF(buffer);
      }
    }
  }

  DropTables ();

  retcode = SQLFreeEnv(henv);
  SPRINTF(buffer, " SQLFreeEnv retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);
  SPRINTF(buffer, " %s ending"CRLF, PROGNAME);
  PRINTF(buffer);

  return(0);
}

/* printf is not multi thread safe (under NT) => no printing
   in thread worker routines */
#undef PRINTF

#define PRINTF(x)

int AsyncThreadProc(TestValues *TestCase)
{
  HDBC hdbc   = SQL_NULL_HDBC;
  HSTMT hstmt = SQL_NULL_HSTMT;
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UCHAR buffer [BUFSIZE];
  RETCODE retcode;

  SPRINTF(buffer, " AsyncThreadProc running %x"CRLF, TestCase);
  PRINTF(buffer);

  retcode = SQLAllocConnect(henv, &hdbc);
  SPRINTF(buffer, " SQLAllocConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLConnect(hdbc, DBNODE, (SWORD) SQL_NTS, ODBCUSER, (SWORD) SQL_NTS,
                       ODBCPASSW, (SWORD) SQL_NTS );
  SPRINTF(buffer," SQLConnect retcode: %d"CRLF, retcode);

  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLAllocStmt (hdbc, &hstmt);
  retcode = SQLSetStmtOption (hstmt, SQL_ASYNC_ENABLE, SQL_ASYNC_ENABLE_ON);
  SPRINTF(buffer," SQLSetStmtOption retcode: %s"CRLF, cnvret(retcode));
  PRINTF(buffer);

  retcode = AsyncExecute(hstmt, TestCase);
  SPRINTF(buffer, " AsyncExecute retcode: %d"CRLF, retcode);
  PRINTF(buffer);

  retcode = SQLDisconnect(hdbc);
  SPRINTF(buffer, " SQLDisconnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  retcode = SQLFreeConnect(hdbc);
  SPRINTF(buffer, " SQLFreeConnect retcode: %d"CRLF, retcode);
  PRINTF(buffer);
  ERROR_CHECK(NULL);

  SPRINTF(buffer, " AsyncThreadProc ending"CRLF);
  PRINTF(buffer);

  return(retcode);
}

int AsyncExecute(HSTMT hstmt, TestValues *TestCase)
{
  UCHAR sqlstate  [ 10 ];
  SDWORD esq_sql_code;
  UCHAR error_txt  [ 101 ] ;
  SWORD len_error_txt = 100;
  SWORD used_error_txt;
  UCHAR buffer [BUFSIZE];
  RETCODE retcode;
  TestValues *p;

  SPRINTF(buffer, " AsyncExecute running"CRLF);
  PRINTF(buffer);

  for(p=TestCase;p->cbFunction;p++) {
     retcode = SQL_STILL_EXECUTING;
     p->cbSuccess = FALSE;
     while (retcode == SQL_STILL_EXECUTING) {
        switch(p->cbFunction) {
           case (SQL_API_SQLEXECDIRECT): {
              retcode = SQLExecDirect(hstmt, p->szP1, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLPREPARE): {
              retcode = SQLPrepare(hstmt, p->szP1, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLEXECUTE): {
              retcode = SQLExecute(hstmt);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLTABLES): {
              retcode = SQLTables(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS, p->szP1, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLCOLUMNS): {
              retcode = SQLColumns(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS, p->szColumnName, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLSTATISTICS): {
              retcode = SQLStatistics(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS, (WORD)p->cbP1, (WORD)p->cbP2);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLTABLEPRIVILEGES): {
              retcode = SQLTablePrivileges(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLCOLUMNPRIVILEGES): {
              retcode = SQLColumnPrivileges(hstmt, p->szQualifier, SQL_NTS, "ODBC", SQL_NTS,
              p->szTableName, SQL_NTS, p->szColumnName, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLPRIMARYKEYS): {
              retcode = SQLPrimaryKeys(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLFOREIGNKEYS): {
              retcode = SQLForeignKeys(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS, p->szColumnName, SQL_NTS, ODBCUSER, SQL_NTS, p->szP2, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLPROCEDURES): {
              retcode = SQLProcedures(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
           case (SQL_API_SQLPROCEDURECOLUMNS): {
              retcode = SQLProcedureColumns(hstmt, p->szQualifier, SQL_NTS, ODBCUSER, SQL_NTS,
              p->szTableName, SQL_NTS, p->szColumnName, SQL_NTS);
              ERROR_CHECK(hstmt);
              break;
           }
        }

#ifndef WIN32
#ifdef OSF1
        sched_yield();
        /*
#elif LINUX
        sleep (0);
#else
        */
        sleep (0);
        /*	pthread_yield(); */
#endif
#else
	Sleep(0);
#endif
     }
     p->cbSuccess = TRUE;
     if (retcode == SQL_SUCCESS) {
        SQLLEN crow;
        retcode = SQLRowCount(hstmt, &crow);
        if (retcode == SQL_SUCCESS && crow != 0) {
           retcode = SQLFetch(hstmt);
        }
     }
     retcode = SQLFreeStmt (hstmt, SQL_CLOSE);
     ERROR_CHECK(hstmt);
  }
  SPRINTF(buffer, " AsyncExecute ending"CRLF);
  PRINTF(buffer);

  return(retcode);
}

