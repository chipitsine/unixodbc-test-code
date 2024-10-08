/*!**************************************************************************

  module       : uniutil.h

  -------------------------------------------------------------------------

  responsible : Arsyonov Anton

  special area: Unicode Utilites
  description : Function accessing string convertion between Ascii/UCS2 and UTF8

  last changed: 2000-09-04  22:14
  first created:2000-08-23  18:38

  -------------------------------------------------------------------------





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



*****************************************************************************/

#ifndef _ODBC
#define _ODBC
#endif


#define MAX_NAME_LEN 512

#ifdef _ODBC
	#define LIBWCHAR SQLWCHAR
	#define LIBCHAR SQLCHAR
	#define LIBSMALLINT SQLSMALLINT
	#define LIBUSMALLINT SQLUSMALLINT
	#define LIBINTEGER SQLINTEGER
	#define LIBSCHAR SQLSCHAR
	#define LIBBIGINT SQLBIGINT
	#define LIBREAL SQLREAL
	#define LIBDOUBLE SQLDOUBLE
#else
	#define LIBWCHAR SQLUCS2
	#define LIBCHAR unsigned char
	#define LIBSMALLINT sqlint2
	#define LIBUSMALLINT sqluns2
	#define LIBINTEGER sqllongint
	#define LIBSCHAR signed char
        #define UINT sqluns4
        #define BOOL char
#ifdef _BIT64
	#define LIBBIGINT sqlint8
#else
    #define LIBBIGINT sqlint2
#endif
	#define LIBREAL sqlr4
	#define LIBDOUBLE sqlr8

	#define PRINTF printf
	#define SPRINTF sprintf
	#define MEMCPY memcpy
	#define MEMSET memset
	#define MALLOC malloc
	#define FREE free
	#define STRCPY strcpy
	#define STRCAT strcat
#endif

/*
 -------------------------------------------------------------------------
|  Single byte character (CP1251) to unicode conversion
 --------------------------------------------------------------------------
*/

LIBWCHAR CharToWChar(LIBCHAR c);
/*
 -------------------------------------------------------------------------
|  Unicode character to single byte (CP1251) conversion
 --------------------------------------------------------------------------
*/
LIBCHAR WCharToChar(LIBWCHAR c);


/*
 -------------------------------------------------------------------------
|  Single byte character string to unicode string conversion
|
|  Parameters:
|  dst - pointer to a destination unicod string buffer (in)
|  dstLen - destination string length in characters    (in)
|  dstWrite - pointer to a buffer that contain number character in dst buffer, can be NULL (out)
|  zeroFlag - if TRUE append null terminated character to dst (in)
|  src - pointer to a source single byte string buffer (in)
|  srcLen - source string length in characters         (in)
|  srcRead - pointer to a buffer that contain number character in src buffer, can be NULL  (out)
 --------------------------------------------------------------------------
*/
void StringToWString(LIBWCHAR *dst, UINT dstLen, UINT *dstWrite, BOOL zeroFlag, LIBCHAR *src, UINT srcLen, UINT *srcRead);

/*
 -------------------------------------------------------------------------
|  Unicode string to single byte character string conversion
|
|  Parameters:
|  dst - pointer to a destination single byte string buffer (in)
|  dstLen - destination string length in characters    (in)
|  dstWrite - pointer to a buffer that contain number character in dst buffer, can be NULL (out)
|  zeroFlag - if TRUE append null terminated character to dst (in)
|  src - pointer to a source unicode string buffer (in)
|  srcLen - source string length in characters         (in)
|  srcRead - pointer to a buffer that contain number character in src buffer, can be NULL  (out)
 --------------------------------------------------------------------------
*/
void WStringToString(LIBCHAR *dst, UINT dstLen, UINT *dstWrite, BOOL zeroFlag, LIBWCHAR *src, UINT srcLen, UINT *srcRead);

/*
 -------------------------------------------------------------------------
|  Return length of unicode string
|
|  Parameters:
|  str - pointer to a unicode string buffer (in)
|  bufLen - length of a buffer              (in)
 --------------------------------------------------------------------------
*/
UINT WStrLen(LIBWCHAR *str, UINT bufLen);

/*
 --------------------------------------------------------------------------
|  Next functions generate a unicode string, that represent SQL_C types
|
|  Parameters:
|  str - pointer to a buffer for unicode string (must have suitable length)
|  strLen - length of a buffer
|  num - numeric value
|  precision (for Double and Float types) - number of digits after dot.
 --------------------------------------------------------------------------
*/

void GetUnicodeSShort(LIBWCHAR *str, UINT strLen, LIBSMALLINT num);

void GetUnicodeUShort(LIBWCHAR *str, UINT strLen, LIBUSMALLINT num);

void GetUnicodeSLong(LIBWCHAR *str, UINT strLen, LIBINTEGER num);

void GetUnicodeSTinyint(LIBWCHAR *str, UINT strLen, LIBSCHAR num);

void GetUnicodeUTinyint(LIBWCHAR *str, UINT strLen, LIBCHAR num);

/*
void GetUnicodeSBigint(LIBWCHAR *str, UINT strLen, LIBBIGINT num);
*/

void GetUnicodeFloat(LIBWCHAR *str, UINT strLen, LIBREAL num, UINT precision);

void GetUnicodeDouble(LIBWCHAR *str, UINT strLen, LIBDOUBLE num, UINT precision);

#ifdef _ODBC

void GetUnicodeDate(LIBWCHAR *str, UINT strLen, SQL_DATE_STRUCT *sds);

void GetUnicodeTime(LIBWCHAR *str, UINT strLen, SQL_TIME_STRUCT *sts);

void GetUnicodeTimeStamp(LIBWCHAR *str, UINT strLen, SQL_TIMESTAMP_STRUCT *sts);

void GetUnicodeULong(LIBWCHAR *str, UINT strLen, SQLUINTEGER num);

/*
void GetUnicodeUBigint(LIBWCHAR *str, UINT strLen, SQLUBIGINT num);
*/

#endif
/*
 ------------------------------------------------------------------------------
| Generate random LIBINTEGER value
 ------------------------------------------------------------------------------
*/
LIBINTEGER GenerateSQLInteger();

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBINTEGER value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLIntegerArray(LIBINTEGER *buf, UINT len);

/*
 ------------------------------------------------------------------------------
| Generate random LIBSMALLINT value
 ------------------------------------------------------------------------------
*/
LIBSMALLINT GenerateSQLSmallInt();

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBSMALLINT value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLSmallIntArray(LIBSMALLINT *buf, UINT len);

/*
 ------------------------------------------------------------------------------
| Generate random LIBREAL value
 ------------------------------------------------------------------------------
*/
LIBREAL GenerateSQLReal();

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBREAL value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLRealArray(LIBREAL *buf, UINT len);

/*
 ------------------------------------------------------------------------------
| Generate random LIBDOUBLE value
 ------------------------------------------------------------------------------
*/
LIBREAL GenerateSQLDouble();

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBDOUBLE value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLDoubleArray(LIBDOUBLE *buf, UINT len);

#ifdef _ODBC

/*
 ------------------------------------------------------------------------------
| Generate random SQLDATE value
|
| Parameters:
| sds - pointer to a SQL_DATE_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLDate(SQL_DATE_STRUCT *sds);

/*
 ------------------------------------------------------------------------------
| Generate random SQLTIME value
|
| Parameters:
| sts - pointer to a SQL_TIME_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLTime(SQL_TIME_STRUCT *sts);

/*
 ------------------------------------------------------------------------------
| Generate random SQLTIMESTAMP value
|
| Parameters:
| sts - pointer to a SQL_TIMESTAMP_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLTimeStamp(SQL_TIMESTAMP_STRUCT *sts);

/*
 ------------------------------------------------------------------------------
| Generate random SQLDECIMAL value
|
| Parameters:
| sns - pointer to a SQL_NUMERIC_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLDecimal(SQL_NUMERIC_STRUCT *sns, UINT precision, UINT scale);

#endif

/*
 ------------------------------------------------------------------------------
| Generate long 7 bit ASCII string
|
| Parameters:
| buf - pointer to a buffer
| len - length of string in characters
| markCount - period of mark digits in random string (if it less than 5 no marks in random string).
 ------------------------------------------------------------------------------
*/
void GenerateLongAscii(LIBCHAR *buf, UINT len, UINT markCount);

/*
 ------------------------------------------------------------------------------
| Generate long unicode string
|
| Parameters:
| buf - pointer to a buffer
| len - length of string in characters
| markCount - period of mark digits in random string (if it less than 6 no marks in random string).
 ------------------------------------------------------------------------------
*/
void GenerateLongUnicode(LIBWCHAR *buf, UINT len, UINT markCount);

/*
 ------------------------------------------------------------------------------
| Compare long 7 bit ASCII string
|      (ref string can have not last mark if it not entire)
|
| Parameters:
| buf - pointer to a buffer
| len - length of string in characters
| markCount - period of mark digits in random string (if it less than 5 no marks in random string).
 ------------------------------------------------------------------------------
*/
BOOL CompareAsciiLong(LIBCHAR *ref, UINT refLen, UINT markCount);

/*
 ------------------------------------------------------------------------------
| Compare long unicode string
|      (ref string can have not last mark if it not entire)
|
| Parameters:
| buf - pointer to a buffer
| len - length of string in characters
| markCount - period of mark digits in random string (if it less than 6 no marks in random string).
 ------------------------------------------------------------------------------
*/
BOOL CompareUnicodeLong(LIBWCHAR *ref, UINT refLen, UINT markCount);

#ifdef _ODBC

/*
 ---------------------------------------------------------------------------------
| Creat a test table with 3 columns (CHAR(256), VARCHAR(256) and LONG VARCHAR types)
|
| Parameters:
| hstmt - statement handle
| tabname - name of table
| columndef - string with table column defenition
 ---------------------------------------------------------------------------------
*/
RETCODE CreateTestTable(HSTMT *hstmt, LIBCHAR *tabname, LIBCHAR *columndef);

/*
 ----------------------------------------------------------------------------------
| Drop a test table
|
| Parameters:
| hstmt - statement handle
| tabname - name of table
 ----------------------------------------------------------------------------------
*/
RETCODE DropTestTable(HSTMT *hstmt, LIBCHAR *tabname);

#endif


/* random generator for ODBC test collection

   linear congruence generator

   x_i+1 = (a * x_i + b) mod m

 */

/* ----- Defines ----- */

#define GEN_A    5
#define GEN_B    117
#define GEN_M    ((unsigned long) 1<<31)


#ifdef WIN32
typedef __int64 LONG_64;
#else
typedef long long LONG_64;
#endif

/* ----- Declaration ----- */

/* initializes generator */
void InitializeGenerator (long init);

/* gets a random number from the generator */
unsigned long GetRandomNumber ();


RETCODE FullConnectW( HENV *phenv, HDBC *phdbc );

