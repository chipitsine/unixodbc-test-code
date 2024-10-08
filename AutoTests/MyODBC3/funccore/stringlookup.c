#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "funccore.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] =
{
	{ idsTestName,                             "FUNCTION (CORE) TEST ODBC-DRIVER" },
	{ idsSQLAllocConnect,                      "SQLAllocConnect" },
	{ idsSQLAllocConnectDesc,                  "allocates memory for connection handle" },
	{ idsSQLAllocEnv,                          "SQLAllocEnv" },
	{ idsSQLAllocEnvDesc,                      "allocates memory for environment handle" },
	{ idsSQLAllocStmt,                         "SQLAllocStmt" },
	{ idsSQLAllocStmtDesc,                     "allocates memory for statement handle" },
	{ idsSQLBindCol,		                "SQLBindCol" },
	{ idsSQLBindColDesc,	                "assigns the storage and data type for a column" },
	{ idsSQLCancel,            	        "SQLCancel" },
	{ idsSQLCancelDesc,	                "cancels the processing on an hstmt" },
	{ idsSQLColAttributes,                     "SQLColAttributes" },
	{ idsSQLColAttributesDesc,                 "descriptor information for a column in result set" },
	{ idsSQLConnect,                           "SQLConnect" },
	{ idsSQLConnectDesc,                       "loads s driver and connection to a data source" },
	{ idsSQLDescribeCol,                       "SQLDescribeCol" },
	{ idsSQLDescribeColDesc,                   "a result descriptor for one column in result set" },
	{ idsSQLDisconnect,                        "SQLDisconnect" },
	{ idsSQLDisconnectDesc,                    "closes the connection" },
	{ idsSQLError,          	                "SQLError" },
	{ idsSQLErrorDesc,	                        "error or status information" },
	{ idsSQLExecDirect,                        "SQLExecDirect" },
	{ idsSQLExecDirectDesc,                    "executes a preparable statement" },
	{ idsSQLExecute,                           "SQLExecute" },
	{ idsSQLExecuteDesc,                       "executes a preparable statement" },
	{ idsSQLFetch,	       	                "SQLFetch" },
	{ idsSQLFetchDesc,	                        "fetches a row of data from a result set" },
	{ idsSQLFreeConnect,                       "SQLFreeConnect" },
	{ idsSQLFreeConnectDesc,                   "releases a connection handle and frees all memory" },
	{ idsSQLFreeEnv,                           "SQLFreeEnv" },
	{ idsSQLFreeEnvDesc,                       "frees the environment handel and releases all memory" },
	{ idsSQLFreeStmt,                          "SQLFreeStmt" },
	{ idsSQLFreeStmtDesc,                      "stops processing, closes any open cursors" },
	{ idsSQLGetCursorName,                     "SQLGetCursorName" },
	{ idsSQLGetCursorNameDesc,                 "the cursor name associated with a specified hstmt" },
	{ idsSQLNumResultCols,                     "SQLNumResultCols" },
	{ idsSQLNumResultColsDesc,                 "the number of columns an a result set" },
	{ idsSQLPrepare,                           "SQLPrepare" },
	{ idsSQLPrepareDesc,                       "prepares an SQL string for execution" },
	{ idsSQLRowCount,                          "SQLRowCount" },
	{ idsSQLRowCountDesc,                      "the number of rows affected by an UPDATE, INSERT,.." },
	{ idsSQLSetCursorName,                     "SQLSetCursorName" },
	{ idsSQLSetCursorNameDesc,                 "associated a cursor name with an active hstmt" },
	{ idsSQLTransact,                          "SQLTransact" },
	{ idsSQLTransactDesc,                      "requests a commit or rollback operation" },
	{ -1,						NULL },
};

/*
 * Provide our own version of this...
 */

LPTSTR EXTFUN GetRCString(HINSTANCE hInst, LPTSTR szName, int max_len, UINT id)
{
	int i = 0;

	while( idsStrings[ i ].id >= 0 ) {
		if ( idsStrings[ i ].id == id ) {
			if ( strlen( idsStrings[ i ].str ) < max_len ) {
				strcpy( szName, idsStrings[ i ].str );
			}
			else {
				memcpy( szName, idsStrings[ i ].str, max_len );
				szName[ max_len - 1 ] = '\0';
			}
			return idsStrings[ i ].str;
		}

		i ++;
	}

	return NULL;
}
