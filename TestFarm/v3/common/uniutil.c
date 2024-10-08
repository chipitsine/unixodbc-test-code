/*!**************************************************************************

  module       : uniutil.c

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

#include "tst.h"
#include "uniutil.h"
#define NUM_BUFFER_LEN 64

/*
 -------------------------------------------------------------------------
|  UNICODE -> CP1251 conversion table
 --------------------------------------------------------------------------
*/
static LIBWCHAR        utable[] = {    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                                0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                                                0x20, 0x21,
                                                0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
                                                0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
                                                0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                                0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
                                                0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
                                                0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
                                                0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
                                                0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61,
                                                0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
                                                0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
                                                0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
                                                0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x20, 0x20, 0x20,
                                                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2018,
                                                0x2019, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                                0x40e, 0x45e, 0x408, 0xa4, 0x490, 0xa6, 0xa7,
                                                0x401, 0xa9, 0x404, 0xab, 0xac, 0xad, 0xae,
                                                0x407, 0xb0, 0xb1, 0x406, 0x456, 0x491, 0xb5,
                                                0xb6, 0xb7, 0x451, 0x2116, 0x454, 0xbb, 0x458,
                                                0x405, 0x455, 0x457, 0x410, 0x411, 0x412, 0x413,
                                                0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41a,
                                                0x41b, 0x41c, 0x41d, 0x41e, 0x41f, 0x420, 0x421,
                                                0x422, 0x423, 0x424, 0x425, 0x426, 0x427, 0x428,
                                                0x429, 0x42a, 0x42b, 0x42c, 0x42d, 0x42e, 0x42f,
                                                0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436,
                                                0x437, 0x438, 0x439, 0x43a, 0x43b, 0x43c, 0x43d,
                                                0x43e, 0x43f, 0x440, 0x441, 0x442, 0x443, 0x444,
                                                0x445, 0x446, 0x447, 0x448, 0x449, 0x44a, 0x44b,
                                                0x44c, 0x44d, 0x44e, 0x44f      };

/* ----- Global variables ----- */

static unsigned long seed = 411965;      /* if generator is not initialized */

/* -------------------------- */


/* ----- Implementation ----- */
void InitializeGenerator (long init)
{
  seed = init;
}

unsigned long GetRandomNumber ()
{
  LONG_64 prod = GEN_A;

  prod *= seed;
  prod += GEN_B;

  seed = prod % GEN_M;

  return (seed);
}


/*
 -------------------------------------------------------------------------
|  Single byte character (CP1251) to unicode conversion
 --------------------------------------------------------------------------
*/
LIBWCHAR CharToWChar(LIBCHAR c) {
	return (LIBWCHAR)utable[c];
}

/*
 -------------------------------------------------------------------------
|  Unicode character to single byte (CP1251) conversion
 --------------------------------------------------------------------------
*/
LIBCHAR WCharToChar(LIBWCHAR c) {
	UINT i;

	if (c < 0x7e)                       /* Code Page specific conversion for max speed */
		return (LIBCHAR)c;
	if ((c > 0x40F)&&(c < 0x4410))
		return (LIBCHAR)(c - 0x350);    /* Code Page specific conversion for max speed */
	for (i = 127; i < 192; i++) {
		if (utable[i] == c)
			return (LIBCHAR)i;
	}
	return (LIBCHAR)'?';
}

/*
 -------------------------------------------------------------------------
|  Single byte character string to unicode string conversion
|
|  Parameters:
|  dst - pointer to a destination unicode string buffer (in)
|  dstLen - destination string length in characters    (in)
|  dstWrite - pointer to a buffer that contains number character in dst buffer, can be NULL (out)
|  zeroFlag - if TRUE appends null terminated character to dst (in)
|  src - pointer to a source single byte string buffer (in)
|  srcLen - source string length in characters         (in)
|  srcRead - pointer to a buffer that contains number character in src buffer, can be NULL  (out)
 --------------------------------------------------------------------------
*/
void StringToWString(LIBWCHAR *dst, UINT dstLen, UINT *dstWrite, BOOL zeroFlag, LIBCHAR *src, UINT srcLen, UINT *srcRead) {
	UINT i = 0;

	while ((i < dstLen)&&(i < srcLen)) {
		if (src[i] == 0) {
			dst[i] = 0;
			i++;
			zeroFlag = FALSE;
			break;
		}
		dst[i] = CharToWChar(src[i]);
		i++;
	}
	if (zeroFlag) {
		if (i == dstLen)
			i--;
		dst[i] = 0;
		if (dstWrite != NULL)
			*(dstWrite) = i + 1;
	}
	else
		if (dstWrite != NULL)
			*(dstWrite) = i;
	if (srcRead != NULL)
		*(srcRead) = i;
}

/*
 -------------------------------------------------------------------------
|  Unicode string to single byte character string conversion
|
|  Parameters:
|  dst - pointer to a destination single byte string buffer (in)
|  dstLen - destination string length in characters    (in)
|  dstWrite - pointer to a buffer that contains number character in dst buffer, can be NULL (out)
|  zeroFlag - if TRUE appends null terminated character to dst (in)
|  src - pointer to a source unicode string buffer (in)
|  srcLen - source string length in characters         (in)
|  srcRead - pointer to a buffer that contains number character in src buffer, can be NULL  (out)
 --------------------------------------------------------------------------
*/
void WStringToString(LIBCHAR *dst, UINT dstLen, UINT *dstWrite, BOOL zeroFlag, LIBWCHAR *src, UINT srcLen, UINT *srcRead) {
	UINT i = 0;

	while ((i < dstLen)&&(i < srcLen)) {
		if (src[i] == 0) {
			dst[i] = 0;
			i++;
			zeroFlag = FALSE;
			break;
		}
		dst[i] = WCharToChar(src[i]);
		i++;
	}
	if (zeroFlag) {
		if (i == dstLen)
			i--;
		dst[i] = 0;
		if (dstWrite != NULL)
			*(dstWrite) = i + 1;
	}
	else
		if (dstWrite != NULL)
			*(dstWrite) = i;
	if (srcRead != NULL)
		*(srcRead) = i;
}

/*
 -------------------------------------------------------------------------
|  Return length of unicode string
|
|  Parameters:
|  str - pointer to a unicode string buffer (in)
|  bufLen - length of a buffer              (in)
 --------------------------------------------------------------------------
*/
UINT WStrLen(LIBWCHAR *str, UINT bufLen) {
	UINT i = 0;

	while(i < bufLen) {
		if (str[i] == 0)
			break;
		i++;
	}
	return i;
}

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

void GetUnicodeSShort(LIBWCHAR *str, UINT strLen, LIBSMALLINT num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%d", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeUShort(LIBWCHAR *str, UINT strLen, LIBUSMALLINT num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%u", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeSLong(LIBWCHAR *str, UINT strLen, LIBINTEGER num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%d", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeSTinyint(LIBWCHAR *str, UINT strLen, LIBSCHAR num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%d", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeUTinyint(LIBWCHAR *str, UINT strLen, LIBCHAR num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%u", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

/*
void GetUnicodeSBigint(LIBWCHAR *str, UINT strLen, LIBBIGINT num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%d", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}
*/

void GetUnicodeFloat(LIBWCHAR *str, UINT strLen, LIBREAL num, UINT precision) {
	LIBCHAR buffer[NUM_BUFFER_LEN];
	LIBCHAR format[NUM_BUFFER_LEN];
	LIBCHAR numc[NUM_BUFFER_LEN];

	SPRINTF((char *) numc, "%d", precision);
	STRCPY((char *) format, "%.");
	STRCAT((char *) format, (char *) numc);
	STRCAT((char *) format, "E");
	SPRINTF((char *) buffer, (char *) format, num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeDouble(LIBWCHAR *str, UINT strLen, LIBDOUBLE num, UINT precision) {
	LIBCHAR buffer[NUM_BUFFER_LEN];
	LIBCHAR format[NUM_BUFFER_LEN];
	LIBCHAR numc[NUM_BUFFER_LEN];

	SPRINTF((char *) numc, "%d", precision);
	STRCPY((char *) format, "%.");
	STRCAT((char *) format, (char *) numc);
	STRCAT((char *) format, "E");
	SPRINTF((char *) buffer, (char *) format, num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

#ifdef _ODBC
void GetUnicodeDate(LIBWCHAR *str, UINT strLen, SQL_DATE_STRUCT *sds) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%02d-%02d-%02d", sds -> year, sds -> month, sds -> day);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeTime(LIBWCHAR *str, UINT strLen, SQL_TIME_STRUCT *sts) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer,"%02d:%02d:%02d", sts -> hour, sts -> minute, sts -> second);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeTimeStamp(LIBWCHAR *str, UINT strLen, SQL_TIMESTAMP_STRUCT *sts) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

        /* SAP DB has only milliseconds in timestamp fraction -> %06ld */
	SPRINTF((char *) buffer,"%04d-%02d-%02d %02d:%02d:%02d.%06ld", sts -> year, sts -> month, sts -> day, sts -> hour, sts -> minute, sts -> second, sts -> fraction);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

void GetUnicodeULong(LIBWCHAR *str, UINT strLen, SQLUINTEGER num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%u", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}

/*
void GetUnicodeUBigint(LIBWCHAR *str, UINT strLen, SQLUBIGINT num) {
	LIBCHAR buffer[NUM_BUFFER_LEN];

	SPRINTF((char *) buffer, "%u", num);
	StringToWString(str, strLen, NULL, TRUE, buffer, O3UTIL_TEXT_BUFFER_LEN, NULL);
	return;
}
*/

#endif
/*
 ------------------------------------------------------------------------------
| Generate random LIBINTEGER value
 ------------------------------------------------------------------------------
*/
LIBINTEGER GenerateSQLInteger() {
	UINT	r;

	r = GetRandomNumber();
	if ((r & 4) == 0)
		return GetRandomNumber();
	else
		return GetRandomNumber() * -1;
}

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBINTEGER value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLIntegerArray(LIBINTEGER *buf, UINT len) {
	UINT i;

	for (i = 0; i < len; i++)
		buf[i] = GenerateSQLInteger();
}

/*
 ------------------------------------------------------------------------------
| Generate random LIBSMALLINT value
 ------------------------------------------------------------------------------
*/
LIBSMALLINT GenerateSQLSmallInt() {
	UINT z;

	z = sizeof(LIBSMALLINT);
	switch(z) {
		case 1:
			return GetRandomNumber() & 0xFF;
		case 2:
			return GetRandomNumber() & 0xFFFF;
		default:
			return GetRandomNumber();
	}
}

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBSMALLINT value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLSmallIntArray(LIBSMALLINT *buf, UINT len) {
	UINT i;

	for (i = 0; i < len; i++)
		buf[i] = GenerateSQLSmallInt();
}

/*
 ------------------------------------------------------------------------------
| Generate random LIBREAL value
 ------------------------------------------------------------------------------
*/
LIBREAL GenerateSQLReal() {
	LIBREAL val;
	LIBREAL *pVal = &val;
	UINT r;

	r = GetRandomNumber();
	if ((r & 4) == 0)
		r = GetRandomNumber();
	else
		r = GetRandomNumber() * -1;

	*((LIBINTEGER *)pVal) = r;
	return val;
}

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBREAL value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLRealArray(LIBREAL *buf, UINT len) {
	UINT i;

	for (i = 0; i < len; i++)
		buf[i] = GenerateSQLReal();
}

/*
 ------------------------------------------------------------------------------
| Generate random LIBDOUBLE value
 ------------------------------------------------------------------------------
*/
LIBREAL GenerateSQLDouble() {
	LIBDOUBLE val;
	LIBDOUBLE *pVal = &val;
	UINT r;

	r = GetRandomNumber();
	if ((r & 4) == 0)
		r = GetRandomNumber();
	else
		r = GetRandomNumber() * -1;
	*((LIBINTEGER *)pVal) = r;
	r = GetRandomNumber();
	if ((r & 4) == 0)
		r = GetRandomNumber();
	else
		r = GetRandomNumber() * -1;
	*((LIBINTEGER *)pVal + 1) = r;
	return val;
}

/*
 ------------------------------------------------------------------------------
| Generate array of random LIBDOUBLE value
|
| Parameters:
| buf - pointer to a buffer
| len - number of elements
 ------------------------------------------------------------------------------
*/
void GenerateSQLDoubleArray(LIBDOUBLE *buf, UINT len) {
	UINT i;

	for (i = 0; i < len; i++)
		buf[i] = GenerateSQLReal();
}

#ifdef _ODBC
/*
 ------------------------------------------------------------------------------
| Generate random SQLDATE value
|
| Parameters:
| sds - pointer to a SQL_DATE_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLDate(SQL_DATE_STRUCT *sds) {
     LIBSMALLINT year;		/* range between 1 and 9999 */
     LIBUSMALLINT month;    /* range between 1 and 12 */
     LIBUSMALLINT day;
	 UINT numDay[] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	 UINT r;

	 year = GetRandomNumber() % 9999 + 1;
	 if ((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0))
		 numDay[1] = 29;
	 else
		 numDay[1] = 28;
     month = GetRandomNumber() % 12 + 1;
	 day = GetRandomNumber() % numDay[month - 1] + 1;

	 sds -> year = year;
	 sds -> month = month;
	 sds -> day = day;
}

/*
 ------------------------------------------------------------------------------
| Generate random SQLTIME value
|
| Parameters:
| sts - pointer to a SQL_TIME_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLTime(SQL_TIME_STRUCT *sts) {
	 sts -> hour = GetRandomNumber() % 24;
	 sts -> minute = GetRandomNumber() % 60;
	 sts -> second = GetRandomNumber() % 60;
}

/*
 ------------------------------------------------------------------------------
| Generate random SQLTIMESTAMP value
|
| Parameters:
| sts - pointer to a SQL_TIMESTAMP_STRUCT
 ------------------------------------------------------------------------------
*/
void GenerateSQLTimeStamp(SQL_TIMESTAMP_STRUCT *sts) {
	 SQL_DATE_STRUCT sds;

	 GenerateSQLDate(&sds);
	 sts -> year = sds.year;
     sts -> month = sds.month;
	 sts -> day = sds.day;
	 sts -> hour = GetRandomNumber() % 24;
	 sts -> minute = GetRandomNumber() % 60;
	 sts -> second = GetRandomNumber() % 60;
	 sts -> fraction = GetRandomNumber() % 1000000;  /* milliseconds in SAP DB */
}

/*
 ------------------------------------------------------------------------------
| Generate random SQLDECIMAL value
|
| Parameters:
| sns - pointer to a SQL_NUMERIC_STRUCT
| precision -
| scale -
 ------------------------------------------------------------------------------
*/
void GenerateSQLDecimal(SQL_NUMERIC_STRUCT *sns, UINT precision, UINT scale) {
     UINT num;
	 UINT *pNum = &num;
	 LIBCHAR *pChar;

	 pChar = sns -> val;
	 sns -> sign = GetRandomNumber() & 1;
	 sns -> precision = precision;
	 sns -> scale = scale;
	 MEMSET(sns -> val, 0, SQL_MAX_NUMERIC_LEN);
	 num = GetRandomNumber();
	 pChar[0] = *((char *)pNum + 3);
	 pChar[1] = *((char *)pNum + 2);
	 pChar[2] = *((char *)pNum + 1);
	 pChar[3] = *((char *)pNum + 0);
}

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
void GenerateLongAscii(LIBCHAR *buf, UINT len, UINT markCount) {
	UINT i;
	UINT tmp1;
	UINT tmp2 = 1;
	LIBCHAR num[7];

	tmp1 = 1;
	for (i = 0; i < len; i++, tmp1++) {
		if ((tmp1 == markCount)&&(markCount > 5)) {
			tmp1 = 0;
			SPRINTF((char *) num, "%06d", tmp2);
			MEMCPY(buf + i - 5, num, 6);
			tmp2++;
		}
		else {
			do {
				buf[i] = GetRandomNumber() & 0x7F;
			} while(buf[i] == 0);
		}
	}
}

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
void GenerateLongUnicode(LIBWCHAR *buf, UINT len, UINT markCount) {
	UINT i;
	UINT tmp1;
	UINT tmp2 = 1;
	LIBCHAR num[7];
	LIBWCHAR wnum[7];

	tmp1 = 1;
	for (i = 0; i < len; i++, tmp1++) {
		if ((tmp1 == markCount)&&(markCount > 5)) {
			tmp1 = 0;
			SPRINTF((char *) num, "%06d", tmp2);
			StringToWString(wnum, 6, NULL, FALSE, num, 6, NULL);
			MEMCPY(buf + i - 5, wnum, 12);
			tmp2++;
		}
		else {
			do {
				buf[i] = (LIBWCHAR)GetRandomNumber();
			} while(buf[i] == 0);
		}
	}
}

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
BOOL CompareAsciiLong(LIBCHAR *ref, UINT refLen, UINT markCount) {
	LIBCHAR *buf;
	UINT lastMark;
	UINT limit;
	BOOL flag;
	int i;

	buf = MALLOC(refLen);
	GenerateLongAscii(buf, refLen, markCount);
	if (markCount > 5) {
		lastMark = (refLen / markCount + 1) * markCount - 6;
		if (refLen < lastMark)
			limit = refLen;
		else
			limit = lastMark;
	} else
		limit = refLen;
	flag = TRUE;
	for(i = 0; i < limit; i++)
		if (buf[i] != ref[i])
			flag = FALSE;
	FREE(buf);
	return flag;
}

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
BOOL CompareUnicodeLong(LIBWCHAR *ref, UINT refLen, UINT markCount) {
	LIBWCHAR *buf;
	UINT lastMark;
	UINT limit;
	BOOL flag;
	int i;

	buf = MALLOC(refLen * sizeof(LIBWCHAR));
	GenerateLongUnicode(buf, refLen, markCount);
	if (markCount > 5) {
		lastMark = (refLen / markCount + 1) * markCount - 6;
		if (refLen < lastMark)
			limit = refLen;
		else
			limit = lastMark;
	} else
		limit = refLen;
	flag = TRUE;
	for(i = 0; i < limit; i++)
		if (buf[i] != ref[i])
			flag = FALSE;
	FREE(buf);
	return flag;
}

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
RETCODE CreateTestTable(HSTMT *hstmt, LIBCHAR *tabname, LIBCHAR *columndef) {
	LIBCHAR create[MAX_NAME_LEN+1];
	LIBCHAR buffer[O3UTIL_TEXT_BUFFER_LEN];
	LIBCHAR str1[] = "CREATE TABLE ";
	LIBCHAR str2[] = " (%s)";
	RETCODE rc;

	STRCPY(create, str1);
	STRCAT(create, tabname);
	SPRINTF((char *) buffer, str2, columndef);
	STRCAT(create, buffer);
	SPRINTF((char *) buffer,"Create Stmt = '%s'"CRLF, create);
	PRINTF(buffer);
	rc = SQLExecDirect(hstmt, create, SQL_NTS);
	SPRINTF((char *) buffer, "SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
    ErrorCheck(SQL_HANDLE_STMT, hstmt);
	return rc;
}

/*
 ----------------------------------------------------------------------------------
| Drop a test table
|
| Parameters:
| hstmt - statement handle
| tabname - name of table
 ----------------------------------------------------------------------------------
*/
RETCODE DropTestTable(HSTMT *hstmt, LIBCHAR *tabname) {
	LIBCHAR drop[MAX_NAME_LEN+1];
	LIBCHAR buffer[O3UTIL_TEXT_BUFFER_LEN];
	LIBCHAR str1[] = "DROP TABLE ";
	LIBCHAR str2[] = " CASCADE";
	RETCODE rc;

	STRCPY(drop, str1);
	STRCAT(drop, tabname);
	STRCAT(drop, str2);
	SPRINTF((char *) buffer,"Drop Stmt = '%s'"CRLF, drop);
	PRINTF(buffer);
	rc = SQLExecDirect(hstmt, drop, SQL_NTS);
	SPRINTF((char *) buffer, "SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
    ErrorCheck(SQL_HANDLE_STMT, hstmt);
	return rc;
}



RETCODE FullConnectW( HENV *phenv, HDBC *phdbc )
{
    RETCODE retcode;
    char    textBuffer[O3UTIL_TEXT_BUFFER_LEN];
    SQLWCHAR dbnodeW[256];
    SQLWCHAR userW[256];
    SQLWCHAR passW[256];

    retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv );
    SPRINTF( textBuffer,
             "FullConnect\t -- AllocHandle (ENV) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLSetEnvAttr( *phenv, SQL_ATTR_ODBC_VERSION,
                             (void*) SQL_OV_ODBC3, 0 );
    SPRINTF( textBuffer,
             "FullConnect\t -- SetEnvAttr (ODBC_VERSION) \t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    retcode = SQLAllocHandle( SQL_HANDLE_DBC, *phenv, phdbc );
    SPRINTF( textBuffer,
             "FullConnect\t -- AllocHandle (DBC) \t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    if (retcode != SQL_SUCCESS) {
        return retcode;
    }; /* if */

    StringToWString (dbnodeW, sizeof(dbnodeW), NULL, TRUE, DBNODE,    strlen(DBNODE),    NULL);
    StringToWString (userW,   sizeof(userW),   NULL, TRUE, ODBCUSER,  strlen(ODBCUSER),  NULL);
    StringToWString (passW,   sizeof(passW),   NULL, TRUE, ODBCPASSW, strlen(ODBCPASSW), NULL);

    retcode = SQLConnectW( *phdbc,
                           dbnodeW, (SWORD) SQL_NTS,
                           userW,   (SWORD) SQL_NTS,
                           passW,   (SWORD) SQL_NTS );
    SPRINTF( textBuffer, "FullConnect \t -- Connect \t\t\t retcode: %s"CRLF,
             cnvret(retcode) );
    PRINTF( textBuffer );
    ErrorCheck( SQL_HANDLE_DBC, *phdbc );

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        exit(4);

    return retcode;
} /* FullConnectW */

#endif
