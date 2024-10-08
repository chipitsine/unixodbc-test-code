#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <uo_test.h>

/* build for latest ODBC v3 */
#define ODBCVER	0x0351

#ifdef WIN32
    #include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>

#if !defined(__attribute__) && (defined(__cplusplus) || !defined(__GNUC__)  || __GNUC__ == 2 && __GNUC_MINOR__ < 8)
#define __attribute__(A)
#endif

#define EXIT(x, y) exit(x)
#define STRLEN(s1) strlen((char*)s1)
#define STRCPY(s1,s2) strcpy((char*)s1,(char*)s2)
#define STRNCPY(s1,s2,n) strncpy((char*)s1,(char*)s2,n)
#define STRCAT(s1,s2) strcat((char*)s1,(char*)s2)
#define STRNCAT(s1,s2,n) strncat((char*)s1,(char*)s2,n)
#define STRCMP(s1,s2) strcmp((char*)s1,(char*)s2)
#define MEMSET memset
#define MEMCPY memcpy
#define MALLOC malloc
#define FREE   free
#define SPRINTF sprintf
#define PRINTF(xbuffer) printf("%s", (char*)xbuffer)
#define CRLF "\n"


/*
    Function wrappers.
    
    Call these instead of calling the ODBC API directly and you get some additional
    error checks. These may not *always* be appropriate but most of the time they are
    best choice.
    
    NOTE: The checks in these wrappers *MUST* be non-intrusive so as to keep the 
    testing as pure as possible.
*/

/* allocate */
SQLRETURN TSTAllocHandle(SQLSMALLINT handle_type, SQLHANDLE input_handle, SQLHANDLE *output_handle);

/* free */
SQLRETURN TSTFreeHandle(SQLSMALLINT handle_type, SQLHANDLE handle);

/* */
SQLRETURN TSTSetEnvAttr(SQLHENV environment_handle, SQLINTEGER attribute, SQLPOINTER value, SQLINTEGER string_length);

/* connect */
SQLRETURN TSTConnect(SQLHDBC connection_handle, SQLCHAR *server_name, SQLSMALLINT name_length1, SQLCHAR *user_name, SQLSMALLINT name_length2, SQLCHAR *authentication, SQLSMALLINT name_length3);
SQLRETURN TSTDisconnect(SQLHDBC connection_handle);

/* execute */
SQLRETURN TSTExecDirect(SQLHSTMT statement_handle, SQLCHAR *statement_text, SQLINTEGER text_length);
SQLRETURN TSTFetch(SQLHSTMT hstmt);
SQLRETURN TSTNumResultCols(SQLHSTMT hstmt, SQLSMALLINT *column_count_ptr);
SQLRETURN TSTGetData(SQLHSTMT hstmt, SQLUSMALLINT column_number, SQLSMALLINT target_type, SQLPOINTER target_value_ptr, SQLINTEGER buffer_length, SQLINTEGER *strlen_or_indptr);

/* catalog/info */
SQLRETURN TSTGetTypeInfo(SQLHSTMT hstmt, SQLSMALLINT data_type);

/* internal */
void tst_print_diagnostics(SQLHENV env, SQLHDBC dbc, SQLHSTMT stm, SQLHDESC des);


/* SAP */
int DisplayColAttributes(SQLHSTMT hstmt);
UCHAR FAR* MakeLongName(UCHAR FAR *szShortName, UCHAR FAR *szLongName, DWORD cbNumberOfChars);
UCHAR FAR *cnvret(RETCODE retcode);
RETCODE GetColAll(SQLHSTMT hstmt);
UCHAR FAR *GetODBCFunctionName(WORD fn);

#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif
#define CRLF "\n"

#define CONTROLUSER "DBA"
#define CONTROLPASSWORD "DBA"
#define SUPERUID CONTROLUSER
#define USERNAME odbcuser
#define ODBCUSER odbcuser
#define PASSWORD odbcpassw
#define ODBCPASSW odbcpassw
#define DBNODE   datasourcename
#define DATASOURCENAME datasourcename
#define DEFAULTS

extern char *odbcuser;
extern char *odbcpassw;
extern char *datasourcename;

#define GREPWEG "GREPWEG :"
#define QUALIFIER_LEN 64
#define OWNER_LEN 64
#define NAME_LEN 64
#define TABLENAME_LEN NAME_LEN
#define COLUMNNAME_LEN 64
#define TYPE_LEN 128
#define TYPENAME_LEN TYPE_LEN
#define REMARKS_LEN 254
#define DATATYPE_LEN 0
#define PRECISION_LEN 0
#define LENGTH_LEN 0
#define SCALE_LEN 0
#define RADIX_LEN 0
#define NULLABLE_LEN 0
#define INDEX_QUALIFIER_LEN 64
#define INDEX_NAME_LEN 64
#define COLLATION_LEN 2

#define WIN_BUFSIZE 600
#define ERROR_TEXT_LEN 511

#define WHCASESTRING(buffer, x) \
case (x): {\
STRCPY((char*)buffer, #x);\
break;\
}

#include "o3util.h"
