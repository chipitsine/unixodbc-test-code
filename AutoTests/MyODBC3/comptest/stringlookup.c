#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "comptest.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] = 
{
	{ idsTestName,            	"COMPARE DATABASES ODBC-DRIVER" },
	{ idsCharUpdate,          	"CharUpdate" },
	{ idsCharUpdateDesc,      	"Update CHAR-Spalte 1000x, Zeitmessung" },
	{ idsDTypConvert,         	"Convert" },
	{ idsDTypConvertDesc,     	"Datatypen-Konvertierung, Zeitmessung" },
	{ idsMFetchTest,          	"MFETCH" },
	{ idsMFetchTestDesc,      	"Insert u.Select von 1000 Zeilen, Zeitmessung MFETCH" },
	{ idsMFetchObTest,        	"MFETCH o.B." },
	{ idsMFetchObTestDesc,    	"Insert u.Select von 1000 Zeilen, ohne SQLBindCol" },
	{ idsMFetchOpTest,        	"MFETCH o.P." },
	{ idsMFetchOpTestDesc,    	"Insert u.Select von 1000 Zeilen, ohne Param." },
	{ idsNetCharTest,         	"NetChar" },
	{ idsNetCharTestDesc,     	"Insert u.Select 1000x1000 Zeilen, Zeitmessung" },
	{ idsNetLongTest,         	"NetLong" },
	{ idsNetLongTestDesc,     	"Insert u.Select 1000x1000 Zeilen, Zeitmessung" },
	{ idsNetVarcharTest,    	"NetVarchar" },
	{ idsNetVarcharTestDesc,	"Insert u.Select 1000x1000 Zeilen, Zeitmessung" },
	{ idsSQLColumns,        	"SQLColumns" },
	{ idsSQLColumnsDesc,    	"Test Katalog Funktion" },
	{ idsSQLStatistics,     	"SQLStatistics" },
	{ idsSQLStatisticsDesc, 	"Test Katalog Funktion" },
	{ idsSQLTables,         	"SQLTables" },
	{ idsSQLTablesDesc,     	"Test Katalog Funktion" },
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
