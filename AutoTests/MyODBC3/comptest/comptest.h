/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : COMPTEST.H
**
** Purpose      : COMPTEST Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| DOSTEST Auto Test DLL
//|
//| Title:  COMPTEST.H
//|
//| Purpose:
//|     This file contains the defines required for the resource strings in CUSTOM.RC.
//|     This file is optional for an auto test DLL.
//|
//|     Note that user resource strings should start at 30001.
//*---------------------------------------------------------------------------------
#ifndef _COMPTEST_DEFS
#define _COMPTEST_DEFS

#define idsTestName                     30001
#define idsCharUpdate 	                30002
#define idsCharUpdateDesc	        30003
#define idsDTypConvert 	                30004
#define idsDTypConvertDesc	        30005
#define idsMFetchTest 	                30006
#define idsMFetchTestDesc	        30007
#define idsMFetchObTest                 30008
#define idsMFetchObTestDesc	        30009
#define idsMFetchOpTest                 30010
#define idsMFetchOpTestDesc	        30011
#define idsNetCharTest 	                30012
#define idsNetCharTestDesc	        30013
#define idsNetLongTest 	                30014
#define idsNetLongTestDesc	        30015
#define idsNetVarcharTest               30016
#define idsNetVarcharTestDesc	        30017
#define idsSQLColumns                   30018
#define idsSQLColumnsDesc	        30019
#define idsSQLStatistics                30020
#define idsSQLStatisticsDesc	        30021
#define idsSQLTables                    30022
#define idsSQLTablesDesc	        30023

#endif

/* OUTPUT Parameter */
#define OUTPUT 1
#define NO_OUTPUT -1
#define OUTPUTCH 63 			/* Zeichen : '?' */

/* BREAK Parameter */
#define PRG_BREAK 3
#define DROP_TABLE
//----------------------------------------------------------------------------------
//              Defines and macros
//----------------------------------------------------------------------------------
typedef SWORD (FAR PASCAL *TESTCASEFUNC)(HENV FAR *, HDBC FAR *, HSTMT FAR *, lpSERVERINFO);

// **************************************************************************
#define ERRORCHECK(henv, hdbc, hstmt, lps, exprc, actrc, func) \
{\
	if(!Error_Check(henv, hdbc, hstmt, lps, exprc, actrc, (LPSTR) func)) \
		{ \
		if (actrc < 0) goto DoAbort; \
		/*return TEST_ABORTED;*/ \
		} \
}
// **************************************************************************
#define CHECKTEST(lps, exprc, actrc, func) \
{                                                  \
	if(!CheckTest(lps, exprc, actrc, (LPSTR) func))  \
		{ \
		if (actrc < 0) goto DoAbort; \
		/* return TEST_ABORTED; */ \
		} \
}
// **************************************************************************
#define CHECKERRS(sErr)        \
	if(!sErr) szLogPrintf(lpSrvr, FALSE, "\t\tPassed\r\n\r\n"); \
	else szLogPrintf(lpSrvr, FALSE, "\t\t%d errors\r\n\r\n", sErr);

//----------------------------------------------------------------------------------
//              Local function prototypes
//----------------------------------------------------------------------------------
BOOL FAR PASCAL CheckTest(lpSERVERINFO lps, RETCODE exprc, RETCODE actrc,
				LPSTR   szFuncName);
RETCODE Error_Check(HENV FAR * phenv, HDBC FAR * phdbc, HSTMT FAR * phstmt,
		    lpSERVERINFO lpSrvr, RETCODE exprc, RETCODE actrc,
                    LPSTR szFuncName);
RETCODE GetRowCount(HSTMT FAR * phstmt, UCHAR FAR * stmt, lpSERVERINFO lpSrvr);
/* ---------------------------------------------------------------------har- */
RETCODE Get_pfSqlType(SWORD pfNullable, UCHAR FAR * buffer);
RETCODE Get_pfNullable(SWORD pfNullable, UCHAR FAR * buffer);
RETCODE Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE Get_Searchable(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE Get_Updatable(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE lst_ColumnNames(HENV FAR * phenv, HDBC FAR * phdbc, HSTMT FAR * phstmt, lpSERVERINFO lpSrvr, SWORD outcol);
RETCODE GetTime(struct _timeb *timebuf);
long CheckTime (struct _timeb timebuf1, struct _timeb timebuf2,
			LPSTR szFuncName, lpSERVERINFO lpSrvr, SWORD opt);
/* ---------------------------------------------------------------------har- */
