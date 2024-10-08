/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : WINTEST.H
**
** Purpose      : WINTEST Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| DOSTEST Auto Test DLL
//|
//| Title:  WINTEST.H
//|
//| Purpose:
//|     This file contains the defines required for the resource strings in CUSTOM.RC.
//|     This file is optional for an auto test DLL.
//|
//|     Note that user resource strings should start at 30001.
//*---------------------------------------------------------------------------------
#ifndef _WINTEST_DEFS
#define _WINTEST_DEFS

#define idsTestName                     30001
#define idsCursorName                   30002
#define idsCursorNameDesc               30003
#define idsDatatype                     30004
#define idsDatatypeDesc                 30005
#define idsDBProc                       30006
#define idsDBProcDesc                   30007
#define idsFloatNumbers                 30008
#define idsFloatNumbersDesc             30009
#define idsFixedNumbers                 30010
#define idsFixedNumbersDesc             30011
#define idsGetDataR	                30012
#define idsGetDataRDesc		        30013
#define idsGetDataS	                30014
#define idsGetDataSDesc		        30015
#define idsLongDatatype0                30016
#define idsLongDatatype0Desc            30017
#define idsLongDatatype1                30018
#define idsLongDatatype1Desc            30019
#define idsLongDatatype2                30020
#define idsLongDatatype2Desc            30021
#define idsLongDatatype3                30022
#define idsLongDatatype3Desc            30023
#define idsLongDatatype4                30024
#define idsLongDatatype4Desc            30025
#define idsLongDatatype5                30026
#define idsLongDatatype5Desc            30027
#define idsLongDatatype6                30028
#define idsLongDatatype6Desc            30029
#define idsLongDatatype7                30030
#define idsLongDatatype7Desc            30031
#define idsLongDatatype8                30032
#define idsLongDatatype8Desc            30033
#define idsLongDatatype9                30034
#define idsLongDatatype9Desc            30035
#define idsLongDatatypeA                30036
#define idsLongDatatypeADesc            30037
#define idsMFetchTest 	                30038
#define idsMFetchTestDesc	        30039
#define idsMultIndex 	                30040
#define idsMultIndexDesc	        30041
#define idsReConnect 	                30042
#define idsReConnectDesc	        30043
#define idsResultMax 	                30044
#define idsResultMaxDesc	        30045
#define idsSingleIndex 	                30046
#define idsSingleIndexDesc	        30047
#define idsTimestamp 	                30048
#define idsTimestampDesc	        30049
#define idsTimestampfr 	                30050
#define idsTimestampfrDesc	        30051

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
/* UCHAR prg_name[13]; */
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
