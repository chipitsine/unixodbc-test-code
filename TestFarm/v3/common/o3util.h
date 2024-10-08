/*



    ========== licence begin  GPL
    Copyright (c) 2001-2004 SAP AG

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
#ifndef O3UTIL
#define O3UTIL

/* Define argv_t to allow passing the cmdline arguments to something other
   than main().  This is tricky because on VMS, argv pointers are always
   32-bits wide, even if sizeof(void *) is 8.
 */
#ifdef VMS
#if __INITIAL_POINTER_SIZE
#pragma __pointer_size __save
#pragma __pointer_size 32
typedef char *argv_t[];
typedef char **argp_t;
#pragma __pointer_size __restore
#else
typedef char *argv_t[];
typedef char **argp_t;
#endif
#else
typedef char *argv_t[];
typedef char **argp_t;
#endif

void ProcessCmdArguments(int argc, argv_t argv);


#define O3UTIL_TEXT_LEN  1024
#define O3UTIL_TEXT_BUFFER_LEN  2*O3UTIL_TEXT_LEN
#define O3UTIL_SQLSTATE_LEN 10
#define O3UTIL_STMT_LEN 1024

void ErrorCheck( SWORD handleType, SQLHANDLE handle );

enum O3Types {
    O3TYPE_SQLUINTEGER,
    O3TYPE_SQLPOINTER,
    O3TYPE_SQLUSMALLINT,
    O3TYPE_SQLLEN,
    O3TYPE_SQLINTEGER,
    O3TYPE_SQLCHARS,
    O3TYPE_SQLSMALLINT
};

typedef struct {
    UWORD fFetchType;
    SQLROWOFFSET irow;        /* irow is the size of the rowset if
                         * fFetchType == SQL_SELECT_NEW */
    UCHAR szChar[30];   /* value for update, insert */
    DWORD fSqlCursorType;
    DWORD fSqlConcur;
} TestTableStruct;

#define SQL_SELECT_NEW (1000)
#define SQL_UPDATE_ROW (1001)
#define SQL_INSERT_ROW (1002)
#define SQL_DELETE_ROW (1003)

UCHAR FAR *cnvFetchType(UWORD fFetchType);
UCHAR  *cnvcurs(DWORD fSqlCursorType);

void StartTest( char *testName );
void EndTest( char *testName );

void ShowLongChar( char *longChar );
void InitLongBuf( char *buf, UDWORD len );

RETCODE FullConnect( HENV *phenv, HDBC *phdbc );

typedef struct {
    SWORD   fieldType;
    UCHAR  *name;
    UDWORD  dataType;
} descFieldType;

RETCODE ShowDescAll( SQLHDESC hdesc );
RETCODE ShowStmtDesc( HSTMT hstmt );
RETCODE ShowDescField( SQLHDESC hdesc, descFieldType *field, SWORD recNo );


#endif
