#include <stdio.h>
#include "windowport.h"
#include "autotest.h"
#include "wintest.h"

typedef struct stringtable_s {
	int id;
	char *str;
} STRINGTABLE;

STRINGTABLE idsStrings[] = 
{
	{ idsTestName,                             "WINDOWS TEST ODBC-DRIVER" },
	{ idsCursorName,                           "CursorName" },
	{ idsCursorNameDesc,                       "Test using CursorName" },
	{ idsDatatype,                             "Datatype-Test" },
	{ idsDatatypeDesc,                         "Test all Datatypes Insert/Select" },
	{ idsDBProc,                               "DBProc-Test" },
	{ idsDBProcDesc,                           "Test call DBProcedures" },
	{ idsFloatNumbers,                         "FLOAT-Datatype" },
	{ idsFloatNumbersDesc,                     "Insert u.Select bis 18 Stellen Zahlen" },
	{ idsFixedNumbers,                         "FIXED-Datatype" },
	{ idsFixedNumbersDesc,                     "Insert u.Select bis 16,2 Stellen Zahlen" },
	{ idsGetDataR,                         	"GetData 1." },
	{ idsGetDataRDesc,                     	"Read, until return SQL_NO_DATA_FOUND" },
	{ idsGetDataS,                         	"GetData 2." },
	{ idsGetDataSDesc,                     	"Test Function, read parts from String" },
	{ idsLongDatatype0,                        "LONG-Datatype 0." },
	{ idsLongDatatype0Desc,                    "Select <NULL>-Value." },
	{ idsLongDatatype1,                        "LONG-Datatype 1." },
	{ idsLongDatatype1Desc,                    "Insert u.Select, Zeitmessung." },
	{ idsLongDatatype2,                        "LONG-Datatype 2." },
	{ idsLongDatatype2Desc,                    "Insert u.Select bis 255 Zeichen (um 10)." },
	{ idsLongDatatype3,                        "LONG-Datatype 3." },
	{ idsLongDatatype3Desc,                    "Insert u.Select ueber 255 Zeichen (4096)." },
	{ idsLongDatatype4,                        "LONG-Datatype 4." },
	{ idsLongDatatype4Desc,                    "Insert u.Update Zeile, altes ueberschreiben." },
	{ idsLongDatatype5,                        "LONG-Datatype 5." },
	{ idsLongDatatype5Desc,                    "Test Data-Truncate auf Zeilen." },
	{ idsLongDatatype6,                        "LONG-Datatype 6." },
	{ idsLongDatatype6Desc,                    "Insert u.Select bis 10K (um 100 erhoehen)." },
	{ idsLongDatatype7,                        "LONG-Datatype 7." },
	{ idsLongDatatype7Desc,                    "Insert u.Select von 10K Zeichen." },
	{ idsLongDatatype8,                        "LONG-Datatype 8." },
	{ idsLongDatatype8Desc,                    "Insert u.Select von 100 Zch.100 Splt." },
	{ idsLongDatatype9,                        "LONG-Datatype 9." },
	{ idsLongDatatype9Desc,                    "Insert u.Select mit SQLPutData u.SQLGetData" },
	{ idsLongDatatypeA,                        "LONG-Datatype 10." },
	{ idsLongDatatypeADesc,                    "Insert u.Select LongData 1024K" },
	{ idsMFetchTest,                           "MFETCH" },
	{ idsMFetchTestDesc,                       "Insert u.Select von 1000 Zeilen, Zeitmessung MFETCH" },
	{ idsMultIndex,                            "Multiple-Index" },
	{ idsMultIndexDesc,                        "Test Multiple-Index (benannter Index)" },
	{ idsReConnect,                            "ReConnect" },
	{ idsReConnectDesc,                        "Test Multi-Connect to Database" },
	{ idsResultMax,                            "ResultMax" },
	{ idsResultMaxDesc,                        "Test Ergebnismenge auf max.Zeilen setzen" },
	{ idsSingleIndex,                          "Single-Index" },
	{ idsSingleIndexDesc,                      "Test Single-Index (unbenannter Index)" },
	{ idsTimestamp,                            "Timestamp" },
	{ idsTimestampDesc,                        "Test Timestamp-Datentyp" },
	{ idsTimestampfr,                          "Timestamp" },
	{ idsTimestampfrDesc,                      "Test Timestamp-Fraction" },
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
