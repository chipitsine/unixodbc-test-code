#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "unixtest.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] = 
{
	{ idsTestName,                             "UNIX TEST ODBC-DRIVER" },
	{ idsConnectMix,                           "Connect-Mix" },
	{ idsConnectMixDesc,                       "SQLConnect mix" },
	{ idsDateTime,                             "DateTime-Datatype" },
	{ idsDateTimeDesc,                         "Insert u.Select von Date/Time" },
	{ idsSQLGetInfo,                           "Test Function" },
	{ idsSQLGetInfoDesc,                       "SQLGetInfo, Werte ausgeben" },
	{ idsSetConnectOption,                     "SQLSetConnectOption" },
	{ idsSetConnectOptionDesc,                 "Test Rollback (AUTOCOMMIT=OFF)" },
	{ idsSmallintDatatype,                     "Smallint-Datatype" },
	{ idsSmallintDatatypeDesc,                 "Insert u.Select von Smallint Werten" },
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
