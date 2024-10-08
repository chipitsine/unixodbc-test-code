/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : UNIXTEST.H
**
** Purpose      : UNIXTEST Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| UNIXTEST Auto Test DLL
//|
//| Title:      UNIXTEST.H
//|
//| Purpose:
//|     This file contains the defines required for the resource strings in CUSTOM.RC.
//|     This file is optional for an auto test DLL.
//|
//|     Note that user resource strings should start at 30001.
//*---------------------------------------------------------------------------------
#ifndef _CUSTOM_DEFS
#define _CUSTOM_DEFS

#define idsTestName                     30001
#define idsConnectMix                   30002
#define idsConnectMixDesc               30003
#define idsDateTime                     30004
#define idsDateTimeDesc                 30005
#define idsSQLGetInfo                   30006
#define idsSQLGetInfoDesc               30007
#define idsSetConnectOption             30008
#define idsSetConnectOptionDesc         30009
#define idsSmallintDatatype             30010
#define idsSmallintDatatypeDesc         30011

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
//**************************************************************************
#define CHECKERRS(sErr)        \
        if(!sErr) szLogPrintf(lpSrvr, FALSE, "\t\tPassed\r\n\r\n"); \
        else szLogPrintf(lpSrvr, FALSE, "\t\t%d errors\r\n\r\n", sErr);
//**************************************************************************
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
/* ************************************************************************* */
