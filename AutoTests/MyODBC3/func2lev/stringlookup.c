#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "func2lev.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] = 
{
	{ idsTestName,                             "FUNCTION (LEVEL2) TEST ODBC-DRIVER" },
	{ idsSQLBrowseConnect,                     "SQLBrowseConnect" },
	{ idsSQLBrowseConnectDesc,                 "an iterative method of discovering." },
	{ idsSQLColumnPrivileges,                  "SQLColumnPrivileges" },
	{ idsSQLColumnPrivilegesDesc,              "returns a list of columns and the privileges." },
	{ idsSQLDataSources,                       "SQLDataSources (DM)" },
	{ idsSQLDataSourcesDesc,                   "lists data source names." },
	{ idsSQLDescribeParam,                     "SQLDecribeParam" },
	{ idsSQLDescribeParamDesc,                 "the describtion of a parameter marker." },
	{ idsSQLDrivers,                           "SQLDrivers (DM)" },
	{ idsSQLDriversDesc,                       "lists driver descriptions and driver attributes." },
	{ idsSQLExtendedFetch,                     "SQLExtendedFetch" },
	{ idsSQLExtendedFetchDesc,                 "extends the functionality of SQLFetch." },
	{ idsSQLForeignKeys,                       "SQLForeignKeys" },
	{ idsSQLForeignKeysDesc,                   "a list of foreign keys in the specified table." },
	{ idsSQLMoreResults,                       "SQLMoreResults" },
	{ idsSQLMoreResultsDesc,                   "determines whether there are more results." },
	{ idsSQLNativeSql,                         "SQLNativeSql" },
	{ idsSQLNativeSqlDesc,                     "returns the SQL string as translated by the driver." },
	{ idsSQLNumParams,                         "SQLNumParams" },
	{ idsSQLNumParamsDesc,                     "the number of paramters in an SQL statement." },
	{ idsSQLParamOptions,                      "SQLParamOptions" },
	{ idsSQLParamOptionsDesc,			"allows an applications to specify multiple values for the set of parameters." },
	{ idsSQLPrimaryKeys,                       "SQLPrimaryKeys" },
	{ idsSQLPrimaryKeysDesc,			"returns the column names that comprise the primary key for a table." },
	{ idsSQLProcedures,                        "SQLProcedures" },
	{ idsSQLProceduresDesc,                    "returns the list of procedure names stored in a specific data source." },
	{ idsSQLProcedureColumns,                  "SQLProcedureColumns" },
	{ idsSQLProcedureColumnsDesc,              "returns the list of input and output parameters for the specified procedure." },
	{ idsSQLSetPos,                            "SQLSetPos" },
	{ idsSQLSetPosDesc,                        "sets the cursor position in a rowset." },
	{ idsSQLSetScrollOptions,                  "SQLSetScrollOptions" },
	{ idsSQLSetScrollOptionsDesc,              "sets options that control the behavior of cursors associated with an hstmt." },
	{ idsSQLTablePrivileges,                   "SQLTablePrivileges" },
	{ idsSQLTablePrivilegesDesc,               "returns a list of tables and the privileges." },
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
