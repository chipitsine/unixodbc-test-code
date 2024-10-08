/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : FUNCCORE.H
**
** Purpose      : FUNCTION Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| FUNCTION Auto Test DLL
//|
//| Title:      FUNCCORE.H
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
#define idsSQLAllocConnect              30002
#define idsSQLAllocConnectDesc          30003
#define idsSQLAllocEnv                  30004
#define idsSQLAllocEnvDesc              30005
#define idsSQLAllocStmt                 30006
#define idsSQLAllocStmtDesc             30007
#define idsSQLBindCol	                30008
#define idsSQLBindColDesc               30009
#define idsSQLCancel		        30010
#define idsSQLCancelDesc   	        30011
#define idsSQLColAttributes             30012
#define idsSQLColAttributesDesc         30013
#define idsSQLConnect                   30014
#define idsSQLConnectDesc               30015
#define idsSQLDescribeCol               30016
#define idsSQLDescribeColDesc           30017
#define idsSQLDisconnect                30018
#define idsSQLDisconnectDesc            30019
#define idsSQLError         	        30020
#define idsSQLErrorDesc	                30021
#define idsSQLExecDirect                30022
#define idsSQLExecDirectDesc            30023
#define idsSQLExecute                   30024
#define idsSQLExecuteDesc               30025
#define idsSQLFetch         	        30026
#define idsSQLFetchDesc	                30027
#define idsSQLFreeConnect               30028
#define idsSQLFreeConnectDesc           30029
#define idsSQLFreeEnv                   30030
#define idsSQLFreeEnvDesc               30031
#define idsSQLFreeStmt                  30032
#define idsSQLFreeStmtDesc              30033
#define idsSQLGetCursorName             30034
#define idsSQLGetCursorNameDesc         30035
#define idsSQLNumResultCols             30036
#define idsSQLNumResultColsDesc         30037
#define idsSQLPrepare                   30038
#define idsSQLPrepareDesc               30039
#define idsSQLRowCount                  30040
#define idsSQLRowCountDesc              30041
#define idsSQLSetCursorName             30042
#define idsSQLSetCursorNameDesc         30043
#define idsSQLTransact                  30044
#define idsSQLTransactDesc              30045

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
