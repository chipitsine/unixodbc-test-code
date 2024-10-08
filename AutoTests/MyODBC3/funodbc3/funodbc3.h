/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : FUNODBC3.H
**
** Purpose      : FUNCTION ODBC 3.0 Auto Test DLL
**
****************************************************************************
*/
//*-------------------------------------------------------------------------
//| FUNCTION Auto Test DLL
//|
//| Title:      FUNODBC3.H
//|
//| Purpose:
//|     This file contains the defines required for the resource strings in CUSTOM.RC.
//|     This file is optional for an auto test DLL.
//|
//|     Note that user resource strings should start at 30001.
//*-------------------------------------------------------------------------
#ifndef _FUNCTEST_DEFS
#define _FUNCTEST_DEFS

#define idsTestName                     30001
#define idsSQLAllocHandle               30002
#define idsSQLAllocHandleDesc           30003
#define idsSQLCloseCursor               30004
#define idsSQLCloseCursorDesc           30005
#define idsSQLColAttribute              30006
#define idsSQLColAttributeDesc          30007
#define idsSQLCopyDesc                  30008
#define idsSQLCopyDescDesc              30009
#define idsSQLDescribeParam             30010
#define idsSQLDescribeParamDesc         30011
#define idsSQLEndTran                   30012
#define idsSQLEndTranDesc               30013
#define idsSQLFetchScroll               30014
#define idsSQLFetchScrollDesc           30015
#define idsSQLFreeHandle                30016
#define idsSQLFreeHandleDesc            30017
#define idsSQLGetConnectAttr            30018
#define idsSQLGetConnectAttrDesc        30019
#define idsSQLGetDiagField              30020
#define idsSQLGetDiagFieldDesc          30021
#define idsSQLGetDiagRec                30022
#define idsSQLGetDiagRecDesc            30023
#define idsSQLGetDescField              30024
#define idsSQLGetDescFieldDesc          30025
#define idsSQLGetDescRec                30026
#define idsSQLGetDescRecDesc            30027
#define idsSQLGetEnvAttr                30028
#define idsSQLGetEnvAttrDesc            30029
#define idsSQLGetFunctions              30030
#define idsSQLGetFunctionsDesc          30031
#define idsSQLGetInfo                   30032
#define idsSQLGetInfoDesc               30033
#define idsSQLGetStmtAttr               30034
#define idsSQLGetStmtAttrDesc           30035
#define idsSQLSetConnectAttr            30036
#define idsSQLSetConnectAttrDesc        30037
#define idsSQLSetDescField              30038
#define idsSQLSetDescFieldDesc          30039
#define idsSQLSetDescRec                30040
#define idsSQLSetDescRecDesc            30041
#define idsSQLSetEnvAttr                30042
#define idsSQLSetEnvAttrDesc            30043
#define idsSQLSetStmtAttr               30044
#define idsSQLSetStmtAttrDesc           30045
#define idsSQLColumnPrivileges          30046
#define idsSQLColumnPrivilegesDesc      30047
#define idsSQLColumns                   30048
#define idsSQLColumnsDesc               30049
#define idsSQLForeignKeys               30050
#define idsSQLForeignKeysDesc           30051
#define idsSQLPrimaryKeys               30052
#define idsSQLPrimaryKeysDesc           30053
#define idsSQLProcedures                30054
#define idsSQLProceduresDesc            30055
#define idsSQLProcedureColumns          30056
#define idsSQLProcedureColumnsDesc      30057
#define idsSQLSpecialColumns            30058
#define idsSQLSpecialColumnsDesc        30059
#define idsSQLStatistics                30060
#define idsSQLStatisticsDesc            30061
#define idsSQLTables 	                30062
#define idsSQLTablesDesc                30063
#define idsSQLTablePrivileges           30064
#define idsSQLTablePrivilegesDesc       30065
#define idsSQLNumericStructI            30066
#define idsSQLNumericStructIDesc        30067
#define idsSQLNumericStructS            30068
#define idsSQLNumericStructSDesc        30069
#define idsSQLNumericStructX            30070
#define idsSQLNumericStructXDesc        30071

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
#define DIAGRECCHECK(handletype, handle,  recnum, lps, exprc, actrc, func) \
{\
	if(!DiagRec_Check(handletype, (SQLHANDLE) handle, recnum, lps, \
	                  exprc, actrc, (LPSTR) func)) \
		{ \
		if (actrc == SQL_ERROR) goto DoAbort; \
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
RETCODE DiagRec_Check(SQLSMALLINT HandleType, SQLHANDLE Handle,
          SQLSMALLINT RecNumber, lpSERVERINFO lpSrvr, RETCODE exprc,
          RETCODE actrc, LPSTR   szFuncName);
RETCODE GetRowCount(HSTMT FAR * phstmt, UCHAR FAR * stmt, lpSERVERINFO lpSrvr);
/* ---------------------------------------------------------------------har- */
RETCODE Get_pfSqlType(SWORD pfNullable, UCHAR FAR * buffer);
RETCODE Get_pfNullable(SWORD pfNullable, UCHAR FAR * buffer);
RETCODE Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE Get_Searchable(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE Get_Updatable(SWORD pfDesc, UCHAR FAR * buffer);
RETCODE lst_ColumnNames(HENV FAR * phenv, HDBC FAR * phdbc, HSTMT FAR * phstmt, lpSERVERINFO lpSrvr, SWORD outcol);
/* ---------------------------------------------------------------------har- */
