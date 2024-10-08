#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "funodbc3.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] = 
{
	{ idsTestName,                             "FUNCTION (ODBC30) TEST ODBC-DRIVER" },
	{ idsSQLAllocHandle,                       "SQLAllocHandle" },
	{ idsSQLAllocHandleDesc,                   "alloc memory for all handle (ENV, CON, STMT, DESC)" },
	{ idsSQLCloseCursor,                       "SQLCloseCursor" },
	{ idsSQLCloseCursorDesc,                   "closes a cursor that has been opened on a statement" },
	{ idsSQLColAttribute,                      "SQLColAttribute" },
	{ idsSQLColAttributeDesc,                  "returns descriptor information for a column in a result set" },
	{ idsSQLCopyDesc,                          "SQLCopyDesc" },
	{ idsSQLCopyDescDesc,                      "copies descriptor information from one descriptor handle to another" },
	{ idsSQLDescribeParam,                     "SQLDescribeParam" },
	{ idsSQLDescribeParamDesc,                 "returns the description of a parameter marker associated" },
	{ idsSQLEndTran,                           "SQLEndTran" },
	{ idsSQLEndTranDesc,                       "requests a commit or rollback operation for all active operations" },
	{ idsSQLFetchScroll,                       "SQLFetchScroll" },
	{ idsSQLFetchScrollDesc,                   "fetches the specified rowset of data from the result set" },
	{ idsSQLFreeHandle,                        "SQLFreeHandle" },
	{ idsSQLFreeHandleDesc,                    "frees memory for all handle (ENV, CON, STMT, DESC)" },
	{ idsSQLGetConnectAttr,                    "SQLGetConnectAttr" },
	{ idsSQLGetConnectAttrDesc,                "the current setting of a connection attributes" },
	{ idsSQLGetDiagField,                      "SQLGetDiagField" },
	{ idsSQLGetDiagFieldDesc,                  "returns the current value of record of the diagnostic data" },
	{ idsSQLGetDiagRec,                        "SQLGetDiagRec" },
	{ idsSQLGetDiagRecDesc,                    "returns the current values of multiple fields of a diagnostic record" },
	{ idsSQLGetDescField,                      "SQLGetDescField" },
	{ idsSQLGetDescFieldDesc,                  "returns the current value of a single field of a descriptor record" },
	{ idsSQLGetDescRec,                        "SQLGetDescRec" },
	{ idsSQLGetDescRecDesc,                    "returns the current values of multiple fields of a descriptor record" },
	{ idsSQLGetEnvAttr,                        "SQLGetEnvAttr" },
	{ idsSQLGetEnvAttrDesc,                    "the current setting of a environment attributes" },
	{ idsSQLGetFunctions,                      "SQLGetFunctions" },
	{ idsSQLGetFunctionsDesc,                  "returns information about a driver supports ODBC function" },
	{ idsSQLGetInfo,                           "SQLGetInfo" },
	{ idsSQLGetInfoDesc,                       "general information about the driver and data source" },
	{ idsSQLGetStmtAttr,                       "SQLGetStmtAttr" },
	{ idsSQLGetStmtAttrDesc,                   "the current setting of a statement attributes" },
	{ idsSQLSetConnectAttr,                    "SQLSetConnectAttr" },
	{ idsSQLSetConnectAttrDesc,                "sets attributes that aspect of connection" },
	{ idsSQLSetDescField,                      "SQLSetDescField" },
	{ idsSQLSetDescFieldDesc,                  "sets the value of a single field of a descriptor record" },
	{ idsSQLSetDescRec,                        "SQLSetDescRec" },
	{ idsSQLSetDescRecDesc,                    "sets multiple descriptor fields of columns or parameters data" },
	{ idsSQLSetEnvAttr,                        "SQLSetEnvAttr" },
	{ idsSQLSetEnvAttrDesc,                    "sets attributes that aspect of environment" },
	{ idsSQLSetStmtAttr,                       "SQLSetStmtAttr" },
	{ idsSQLSetStmtAttrDesc,                   "sets attributes related to an hstmt" },
	{ idsSQLColumnPrivileges,                  "SQLColumnPrivileges" },
	{ idsSQLColumnPrivilegesDesc,              "returns a list of columns and the privileges." },
	{ idsSQLColumns,                           "SQLColumns" },
	{ idsSQLColumnsDesc,                       "a list of column in specified table" },
	{ idsSQLForeignKeys,                       "SQLForeignKeys" },
	{ idsSQLForeignKeysDesc,                   "a list of foreign keys in the specified table." },
	{ idsSQLPrimaryKeys,                       "SQLPrimaryKeys" },
	{ idsSQLPrimaryKeysDesc,			"returns the column names that comprise the primary key for a table." },
	{ idsSQLProcedures,                        "SQLProcedures" },
	{ idsSQLProceduresDesc,                    "returns the list of procedure names stored in a specific data source." },
	{ idsSQLProcedureColumns,                  "SQLProcedureColumns" },
	{ idsSQLProcedureColumnsDesc,              "returns the list of input and output parameters for the specified procedure." },
	{ idsSQLSpecialColumns,                    "SQLSpecialColumns" },
	{ idsSQLSpecialColumnsDesc,                "information about columns within a specified in table" },
	{ idsSQLStatistics,                        "SQLStatistics" },
	{ idsSQLStatisticsDesc,                    "a list of statistics about a single table" },
	{ idsSQLTables,                            "SQLTables" },
	{ idsSQLTablesDesc,                        "list of table names stored in a specific data source" },
	{ idsSQLTablePrivileges,                   "SQLTablePrivileges" },
	{ idsSQLTablePrivilegesDesc,               "returns a list of tables and the privileges." },
	{ idsSQLNumericStructI,                    "SQL_C_NUMERIC I(Double value)" },
	{ idsSQLNumericStructIDesc,                "tests datatype SQL_C_NUMERIC (put-/getdata - Double)." },
	{ idsSQLNumericStructS,                    "SQL_C_NUMERIC S(Double value)" },
	{ idsSQLNumericStructSDesc,                "tests datatype SQL_C_NUMERIC (getdata - Double)." },
	{ idsSQLNumericStructX,                    "SQL_C_NUMERIC X(Long value)" },
	{ idsSQLNumericStructXDesc,                "tests datatype SQL_C_NUMERIC (put-/getdata - Long)." },
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
