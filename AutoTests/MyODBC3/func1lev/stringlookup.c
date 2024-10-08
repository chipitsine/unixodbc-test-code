#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "func1lev.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] = 
{
	{ idsTestName,                             "FUNCTION (LEVEL1) TEST ODBC-DRIVER" },
	{ idsSQLBindParameter,                     "SQLBindParameter" },
	{ idsSQLBindParameterDesc,                 "bind a buffer to a parameter marker in an SQL statement" },
	{ idsSQLColumns,                           "SQLColumns" },
	{ idsSQLColumnsDesc,                       "a list of column in specified table" },
	{ idsSQLDriverConnect,                     "SQLDriverConnect" },
	{ idsSQLDriverConnectDesc,                 "an alternative to SQLConnect, dialog boxes prompt" },
	{ idsSQLGetConnectOpt,                     "SQLGetConnectOption" },
	{ idsSQLGetConnectOptDesc,                 "the current setting of a connection option" },
	{ idsSQLGetData,                           "SQLGetData" },
	{ idsSQLGetDataDesc,                       "result data for a single unbound column in the current row" },
	{ idsSQLGetFunctions,                      "SQLGetFunctions" },
	{ idsSQLGetFunctionsDesc,                  "infromations about whether a driver supports a ODBC function" },
	{ idsSQLGetInfo,                           "SQLGetInfo" },
	{ idsSQLGetInfoDesc,                       "general information about the driver and data source" },
	{ idsSQLGetStmtOption,                     "SQLGetStmtOption" },
	{ idsSQLGetStmtOptionDesc,                 "the current setting of a statement option" },
	{ idsSQLGetTypeInfo,                       "SQLGetTypeInfo" },
	{ idsSQLGetTypeInfoDesc,                   "information about data types supported" },
	{ idsSQLParamData,                         "SQLParamData" },
	{ idsSQLParamDataDesc,                     "conjunction with SQLPutData to supply parameter data at execution time" },
	{ idsSQLPutData,                           "SQLPutData" },
	{ idsSQLPutDataDesc,                       "send data for a parameter or column to the driver at execution" },
	{ idsSQLSetConnectOpt,                     "SQLSetConnectOption" },
	{ idsSQLSetConnectOptDesc,                 "sets options that aspect of connection" },
	{ idsSQLSetStmtOption,                     "SQLSetStmtOption" },
	{ idsSQLSetStmtOptionDesc,                 "sets options related to an hstmt" },
	{ idsSQLSpecialColumns,                    "SQLSpecialColumns" },
	{ idsSQLSpecialColumnsDesc,                "information about columns within a specified in table" },
	{ idsSQLStatistics,                        "SQLStatistics" },
	{ idsSQLStatisticsDesc,                    "a list of statistics about a single table" },
	{ idsSQLTables,                            "SQLTables" },
	{ idsSQLTablesDesc,                        "list of table names stored in a specific data source" },
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
