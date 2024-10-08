#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "sqlexamp.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] =
{
	{ idsTestName,                             "EXAMPLES ODBC-DRIVER" },
	{ idsBinaryNumbers,                        "Binary-Datatype" },
	{ idsBinaryNumbersDesc,                    "Do test SQL_BINARY datatype" },
	{ idsBooleanTypes,                         "Boolean-Datatype" },
	{ idsBooleanTypesDesc,                     "Do test SQL_BIT datatype" },
	{ idsDescribeColumn,                       "Describe Column" },
	{ idsDescribeColumnDesc,                   "Do test columns with blank separate" },
	{ idsFloatNumbers,                         "Double-Datatype" },
	{ idsFloatNumbersDesc,                     "Do test C-datatype double numbers" },
	{ idsCFloatNumbers,                        "Float-Datatype" },
	{ idsCFloatNumbersDesc,                    "Do test C-datatype float numbers" },
	{ idsIntegerNumbers,                       "Integer-Datatype" },
	{ idsIntegerNumbersDesc,                   "Do test C-datatype integer numbers" },
	{ idsSetPosCurrent,                        "SetPos Current" },
	{ idsSetPosCurrentDesc,                    "Do Update current of cursor with SQLSetPos" },
	{ idsSimpleConnect,                        "Simple Connect" },
	{ idsSimpleConnectDesc,                    "Do a connect using SERVERINFO" },
	{ idsSmallintDatatype,                     "Smallint-Datatype" },
	{ idsSmallintDatatypeDesc,                 "Do test smallint datatype" },
	{ idsSQLGetInfo,                           "SQLGetInfo" },
	{ idsSQLGetInfoDesc,                       "Do test SQLGetInfo" },
	{ idsStaticExample,                        "StaticExample" },
	{ idsStaticExampleDesc,                    "Do test StaticExample" },
	{ idsTimeExample,                          "TimeExample" },
	{ idsTimeExampleDesc,                      "Do output execution time" },
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
