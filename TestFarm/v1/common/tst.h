#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
    #include <windows.h>
#endif

#include <string.h>

/*
    Build as a specific ODBC version. The default is the latest ver 
    but we do not want that here.
*/
#define ODBCVER	0x0100

#include <sql.h>
#include <sqlext.h>

/*
    Function wrappers.
    
    Call these instead of calling the ODBC API directly and you get some additional
    error checks. These may not *always* be appropriate but most of the time they are
    best choice.
    
    NOTE: The checks in these wrappers *MUST* be non-intrusive so as to keep the 
    testing as pure as possible.
*/

/* allocate */
RETCODE TSTAllocEnv(HENV FAR *phenv);
RETCODE TSTAllocConnect(HENV henv, HDBC FAR *phdbc);
RETCODE TSTAllocStmt(HDBC hdbc, HSTMT FAR *phstmt);

/* free */
RETCODE TSTFreeEnv(HENV henv);
RETCODE TSTFreeConnect(HDBC hdbc);
RETCODE TSTFreeStmt(HSTMT hstmt, UWORD fOption);

/* connect */
RETCODE TSTConnect(HDBC hdbc, UCHAR FAR *szDSN, SWORD cbDSN, UCHAR FAR *szUID, SWORD cbUID, UCHAR FAR *szAuthStr, SWORD cbAuthStr);
RETCODE TSTDisconnect(HDBC hdbc);

/* execute */
RETCODE TSTExecDirect(HSTMT hstmt, UCHAR FAR *szSqlStr, SDWORD cbSqlStr);
RETCODE TSTFetch(HSTMT hstmt);
RETCODE TSTNumResultCols(HSTMT hstmt, SWORD FAR *pccol);
RETCODE TSTGetData(HSTMT hstmt, UWORD icol, SWORD fCType, PTR rgbValue, SDWORD cbValueMax, SDWORD FAR *pcbValue);

/* catalog/info */
RETCODE TSTGetTypeInfo(HSTMT hstmt, SWORD fSqlType);
RETCODE TSTTables(HSTMT hstmt, UCHAR FAR *szTableQualifier, SWORD cbTableQualifier, UCHAR FAR *szTableOwner, SWORD cbTableOwner, UCHAR FAR *szTableName, SWORD cbTableName, UCHAR FAR *szTableType, SWORD cbTableType);

/* internal */
void tst_print_errors(HENV env, HDBC dbc, HSTMT stm);


