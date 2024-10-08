/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : FUNC1LEV.H
**
** Purpose      : FUNCTION Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| FUNCTION Auto Test DLL
//|
//| Title:      FUNC1LEV.H
//|
//| Purpose:
//|     This file contains the defines required for the resource strings in CUSTOM.RC.
//|     This file is optional for an auto test DLL.
//|
//|     Note that user resource strings should start at 30001.
//*---------------------------------------------------------------------------------
#ifndef _FUNCTEST_DEFS
#define _FUNCTEST_DEFS

#define idsTestName                     30001
#define idsSQLBindParameter             30002
#define idsSQLBindParameterDesc         30003
#define idsSQLColumns                   30004
#define idsSQLColumnsDesc               30005
#define idsSQLDriverConnect             30006
#define idsSQLDriverConnectDesc         30007
#define idsSQLGetConnectOpt             30008
#define idsSQLGetConnectOptDesc         30009
#define idsSQLGetData                   30010
#define idsSQLGetDataDesc               30011
#define idsSQLGetFunctions              30012
#define idsSQLGetFunctionsDesc          30013
#define idsSQLGetInfo                   30014
#define idsSQLGetInfoDesc               30015
#define idsSQLGetStmtOption             30016
#define idsSQLGetStmtOptionDesc         30017
#define idsSQLGetTypeInfo               30018
#define idsSQLGetTypeInfoDesc           30019
#define idsSQLParamData                 30020
#define idsSQLParamDataDesc             30021
#define idsSQLPutData                   30022
#define idsSQLPutDataDesc               30023
#define idsSQLSetConnectOpt             30024
#define idsSQLSetConnectOptDesc         30025
#define idsSQLSpecialColumns            30026
#define idsSQLSpecialColumnsDesc        30027
#define idsSQLSetStmtOption             30028
#define idsSQLSetStmtOptionDesc         30029
#define idsSQLStatistics                30030
#define idsSQLStatisticsDesc            30031
#define idsSQLTables	                30032
#define idsSQLTablesDesc                30033

#endif

/* OUTPUT Parameter */
#define OUTPUT 1
#define NO_OUTPUT -1
#define OUTPUTCH 63                     /* Zeichen : '?' */

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
/* ---------------------------------------------------------------------har- */
