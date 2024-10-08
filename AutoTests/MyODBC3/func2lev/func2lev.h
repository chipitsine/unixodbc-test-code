/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : FUNC2LEV.H
**
** Purpose      : FUNCTION Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| FUNCTION Auto Test DLL
//|
//| Title:      FUNC2LEV.H
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
#define idsSQLBrowseConnect             30002
#define idsSQLBrowseConnectDesc         30003
#define idsSQLColumnPrivileges          30004
#define idsSQLColumnPrivilegesDesc      30005
#define idsSQLDataSources               30006
#define idsSQLDataSourcesDesc           30007
#define idsSQLDescribeParam             30008
#define idsSQLDescribeParamDesc         30009
#define idsSQLDrivers                   30010
#define idsSQLDriversDesc               30011
#define idsSQLExtendedFetch             30012
#define idsSQLExtendedFetchDesc         30013
#define idsSQLForeignKeys               30014
#define idsSQLForeignKeysDesc           30015
#define idsSQLMoreResults               30016
#define idsSQLMoreResultsDesc           30017
#define idsSQLNativeSql                 30018
#define idsSQLNativeSqlDesc             30019
#define idsSQLNumParams                 30020
#define idsSQLNumParamsDesc             30021
#define idsSQLParamOptions              30022
#define idsSQLParamOptionsDesc          30023
#define idsSQLPrimaryKeys               30024
#define idsSQLPrimaryKeysDesc           30025
#define idsSQLProcedures                30026
#define idsSQLProceduresDesc            30027
#define idsSQLProcedureColumns          30028
#define idsSQLProcedureColumnsDesc      30029
#define idsSQLSetPos  		        30030
#define idsSQLSetPosDesc		30031
#define idsSQLSetScrollOptions          30032
#define idsSQLSetScrollOptionsDesc      30033
#define idsSQLTablePrivileges           30034
#define idsSQLTablePrivilegesDesc       30035

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
