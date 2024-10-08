/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLEXAMP.C
**
** Purpose      : SQL-EXAMPLE Auto Test DLL
**
****************************************************************************
*/
//*---------------------------------------------------------------------------------
//| SQLEXAMPLE Auto Test DLL
//|
//| Title:      EXAMPLE SQLODBC TEST
//|
//| Purpose:
//|     This sample Auto Test DLL shows how an ODBC auto test may be written and
//|     subsequently run via the ODBC Test application.  To use this DLL:
//|             1) Compile the source code via the BUILD.EXE program
//|                                     SQLEXAMP.C      This shource file with test code
//|                                     SQLEXAMP.H      Include files with defines, macros, and prototypes
//|                                     SQLEXAMP.RC     Resource file for string tables
//|                     (Note that .H and .RC files are optional in for an Auto Test)
//|             2) Start the ODBC Test application
//|             3) If you have not done so, choose Tools, Manage Test Sources and define
//|                             a test source to run against
//|             4) Choose Tools, Manage Tests and add the CUSTOM.DLL created in step 1
//|                             to the list of installed test DLLs
//|             5) Choose Tools, Run Tests and select "Custom Auto Test" and your Test Source
//|                             from step #3
//|     For more details, please see the SDK documentation.
//*---------------------------------------------------------------------------------
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "sqlexamp.h"

/* ------------------------------------------------------------------------- */

#define MAX_NAME_LEN 50
#define ERROR_TEXT_LEN 511

//----------------------------------------------------------------------------------
//              Local function prototypes
//----------------------------------------------------------------------------------
SWORD FAR PASCAL DoBinaryNumbers(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoBooleanTypes(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoDescribeColumn(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoFloatNumbers(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoCFloatNumbers(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoIntegerNumbers(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoSetPosCurrent(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoSimpleConnect(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoSmallintDatatype(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoSQLGetInfo(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoStaticExample(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
SWORD FAR PASCAL DoTimeExample(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr);
BOOL FAR PASCAL CheckTest(lpSERVERINFO lps, RETCODE exprc, RETCODE actrc,
				LPSTR   szFuncName);


//
// This structure is declared to describe the test cases and descriptions
//              that this auto test supports.  Note that the strings are stored in
//              the resource fork, but could have been hard coded.
//
struct {
	UINT                                    uiName;                 // Test case name
	UINT                                    uiDesc;                 // Test case description
	TESTCASEFUNC            lpTestFunc;             // Pointer to function to implement test
	} TestCases[] = {
// szName                                       szDesc                                          lpTestFunc
// -------------------- ----------------------- ------------------------
	idsBinaryNumbers,       idsBinaryNumbersDesc,           DoBinaryNumbers,
	idsBooleanTypes,        idsBooleanTypesDesc,            DoBooleanTypes,
	idsDescribeColumn,      idsDescribeColumnDesc,          DoDescribeColumn,
	idsFloatNumbers,        idsFloatNumbersDesc,            DoFloatNumbers,
	idsCFloatNumbers,       idsCFloatNumbersDesc,           DoCFloatNumbers,
	idsIntegerNumbers,      idsIntegerNumbersDesc,          DoIntegerNumbers,
	idsSetPosCurrent,       idsSetPosCurrentDesc,           DoSetPosCurrent,
	idsSimpleConnect,       idsSimpleConnectDesc,           DoSimpleConnect,
	idsSmallintDatatype,    idsSmallintDatatypeDesc,        DoSmallintDatatype,
	idsSQLGetInfo,          idsSQLGetInfoDesc,              DoSQLGetInfo,
	idsStaticExample,       idsStaticExampleDesc,           DoStaticExample,
	idsTimeExample,         idsTimeExampleDesc,             DoTimeExample,
	};


//**************************************************************************
//***************************  External Interfaces  ************************
//*  These functions are called by ODBC Test to gather run-time information
//**************************************************************************


//*---------------------------------------------------------------------------------
//| AutoTestName:
//|     This function is called to give the name of the auto test (which cannot
//|     exceed AUTO_MAX_TEST_NAME) as well as the number of test cases which
//|     are implemented in this test DLL.
//| Parms:
//|     szName                                  The name to be displayed
//|     pcbTestCases                    Pointer to count of test cases
//| Returns:
//|     TRUE if successful (pcbTestCases set), FALSE for error
//*---------------------------------------------------------------------------------
BOOL EXTFUN AutoTestName(LPSTR szName, UINT FAR * pcbTestCases)
{
	GetRCString(hLoadedInst, szName, AUTO_MAX_TEST_NAME, idsTestName);
	*pcbTestCases = NumItems(TestCases);

   return TRUE;
}

//*---------------------------------------------------------------------------------
//| AutoTestDesc:
//|     This function is called by ODBC Test when a description of a specific
//|     test case is required.  The returned name must be no larger than
//|     AUTO_MAX_TESTCASE_NAME including the NULL terminator.  The returned
//|     description must be no larger than AUTO_MAX_TESTDESC_NAME including the
//|     NULL.  iTest will be 1-based index of the test required.
//|
//|     Note that iTest will start at 1 and will go to the number of
//|             test cases as specified by the AutoTestName function.
//|
//| Parms:
//|     iTest                                           1-based index of test case required
//|     szName                                  The name of the test case
//|     szDesc                                  A description of the test case
//| Returns:
//|     TRUE if successful, FALSE for error
//*---------------------------------------------------------------------------------
BOOL EXTFUN AutoTestDesc(UWORD iTest, LPSTR szName, LPSTR szDesc)
{
	// Extra protection should AutoTestName return invalid pcbTestCases
	if(iTest > NumItems(TestCases))
		return FALSE;

	// Use GetRCString to retrieve resource string directly into return
	//              values
	GetRCString(hLoadedInst, szName,
					AUTO_MAX_TESTCASE_NAME, TestCases[(iTest-1)].uiName);
	GetRCString(hLoadedInst, szDesc,
					AUTO_MAX_TESTDESC_NAME, TestCases[(iTest-1)].uiDesc);

   return TRUE;
}

//*---------------------------------------------------------------------------------
//| AutoTestFunc:
//|     This function is called to execute a test case selected by the user for
//|     execution.  The lpSrvr structure contains the information required for
//|     connected (as defined in the chosen Test Source), as well as other
//|     usefull information.  See the AUTOTEST.H file for the structure
//|     declaration.
//|
//|     Use the GETBIT macro to determine which test should be executed.
//|
//| Parms:
//|     lpSrvr                                  Information required for running the test
//| Returns:
//|     Nothing
//*---------------------------------------------------------------------------------
void EXTFUN AutoTestFunc(lpSERVERINFO lpSrvr)
{
	HENV            henv=NULL;
	HDBC            hdbc=NULL;
	HSTMT           hstmt0=NULL;
	int             iDex;
	SWORD           cErrCnt;                                                                                // Count errors
	char            szName[AUTO_MAX_TESTCASE_NAME+6];       // Room for NULL and \r\n


	// Sets the error count to 0
	InitTest(lpSrvr);

	// Loop through the count of test cases looking for set bits via GETBIT.
	//              When a bit is set, that test is to be run.  We've stored the
	//              function address which will implement the test, so simply call it.
	for(iDex=1;  iDex<=NumItems(TestCases);  iDex++)
		if(GETBIT(lpSrvr->rglMask, iDex)) {
			// Print out title of test
			GetRCString(hLoadedInst, szName,
					AUTO_MAX_TESTCASE_NAME, TestCases[(iDex-1)].uiName);
			szLogPrintf(lpSrvr, FALSE, "%s:\r\n", (LPSTR)szName);

			// Call the test case and add errors
			cErrCnt =
				(*TestCases[(iDex-1)].lpTestFunc)(&henv, &hdbc, &hstmt0, lpSrvr);
			if(cErrCnt != TEST_ABORTED)
				lpSrvr->cErrors += cErrCnt;
			else
				goto abort;
			}
	return;


	// When a test must abort, the test case should call the AbortTest
	//              macro which sets lpSrvr->cErrors to TEST_ABORTED.
abort:
	return;
}

//*************************  Utility Functions  ****************************
//*  This section contains internal utility functions
//**************************************************************************
//*---------------------------------------------------------------------------------
//| CheckTest:
//|     This function will do a simple comparison of return codes and issue
//|     erros on failure.  Use the CHECKTEST macro to invoke.
//|
//| Returns:
//|     TRUE if the codes match, FALSE on error
//*---------------------------------------------------------------------------------
BOOL FAR PASCAL CheckTest(lpSERVERINFO lps, RETCODE exprc, RETCODE actrc,
				LPSTR   szFuncName)
{
 SWORD opt=OUTPUT,i;

 i = lstrlen(szFuncName); if (szFuncName[i-1] == OUTPUTCH) opt = NO_OUTPUT;

/* ------------------------------------------------------------------------- */
 if ( opt != NO_OUTPUT) {
/*
	sprintf(buffer,"\t\t %s -> retcode: %d\r\n", szFuncName, actrc);
	PutComp(buffer, prg_name, lps);
*/
	szLogPrintf(lps, FALSE,"\t\t %s -> retcode: %d\r\n",
				   szFuncName, actrc);
 }
/* ---------------------------------------------------------------------har- */
	if(exprc != actrc) {
		if (opt != NO_OUTPUT) {
		 szLogPrintf(lps, FALSE, "\t%s failed:\r\n", (LPSTR)szFuncName);
		 szLogPrintf(lps, FALSE, "\t\tExpected: %d\r\n", exprc);
		 szLogPrintf(lps, FALSE, "\t\tActual:   %d\r\n", actrc);
		}
		return FALSE;
		}
	else
		return TRUE;
}

/* ---------------------------------------------------------------------har- */
//*---------------------------------------------------------------------------------
//| ErrorCheck:
//|     This function will do a simple comparison of return codes and issue
//|     erros on failure.
//|
//| Returns:
//|
//*---------------------------------------------------------------------------------
RETCODE Error_Check(HENV FAR * phenv, HDBC FAR * phdbc, HSTMT FAR * phstmt,
		    lpSERVERINFO lpSrvr, RETCODE exprc, RETCODE actrc,
		    LPSTR   szFuncName)
{
 RETCODE	api_rc;
 UCHAR 		sqlstate[10];
 SDWORD 	esq_sql_code;
 UCHAR 		error_txt[ERROR_TEXT_LEN+1];
 SWORD 		len_error_txt = ERROR_TEXT_LEN;
 SWORD 		used_error_txt;
 UCHAR 		buffer[1024];
 UCHAR 		outtxt[MAX_NAME_LEN];
 SWORD 		opt = OUTPUT,i;
 SWORD 		rc;

 i = lstrlen(szFuncName); if (szFuncName[i-1] == OUTPUTCH) opt = NO_OUTPUT;

/* ------------------------------------------------------------------------- */
 if ( opt != NO_OUTPUT) {
	szLogPrintf(lpSrvr, FALSE,"\t\t %s -> retcode: %d\r\n",
							szFuncName, actrc);
 }
/* ---------------------------------------------------------------------har- */
	if(exprc != actrc) {
		if (opt != NO_OUTPUT) {
		 szLogPrintf(lpSrvr, FALSE, "\t%s failed:\r\n", (LPSTR)szFuncName);
		 szLogPrintf(lpSrvr, FALSE, "\t\tExpected: %d\r\n", exprc);
		 szLogPrintf(lpSrvr, FALSE, "\t\tActual:   %d\r\n", actrc);
		}
		rc=FALSE;
	}
	else
		rc=TRUE;
/* ---------------------------------------------------------------------har- */
 if (actrc != SQL_SUCCESS) {
	api_rc = SQLError(phenv, phdbc, phstmt, sqlstate, &esq_sql_code,
		 	  error_txt, len_error_txt, &used_error_txt);
	if ( opt != NO_OUTPUT) {
	 if (api_rc == SQL_NO_DATA_FOUND) {
	   szLogPrintf(lpSrvr, FALSE,"\tSQLError -> (SQL_NO_DATA_FOUND)\r\n");
	 } else {
	   if (actrc < 0) lstrcpy(outtxt,"Error");
	     else lstrcpy(outtxt,"Warning");
	   sprintf(buffer,"sqlstate: %s",sqlstate);
	   szLogPrintf(lpSrvr, FALSE,"\tSQLError (%s) -> %s\r\n", outtxt, buffer);
	   sprintf(buffer,"esq_sql_code: %ld",esq_sql_code);
	   szLogPrintf(lpSrvr, FALSE,"\tSQLError (%s) -> %s\r\n", outtxt, buffer);
	   sprintf(buffer,"error_txt: %s", error_txt);
	   szLogPrintf(lpSrvr, FALSE,"\tSQLError (%s) -> %s\r\n", outtxt, buffer);
	 }
	}
	switch (actrc) {
		case(100):
			 break;
		default  :
			  /* exit(16); */
			 break;
	}
 }
/* ---------------------------------------------------------------------har- */
 return(rc);
}
/* ---------------------------------------------------------------------har- */
//*---------------------------------------------------------------------------------
//| GetRowCount:
//|
//| Returns:
//|
//*---------------------------------------------------------------------------------
RETCODE GetRowCount(HSTMT FAR * phstmt, UCHAR FAR * stmt, lpSERVERINFO lpSrvr)
{
 RETCODE rc;
 /* SDWORD FAR pcrow; */
 SDWORD pcrow;
 UCHAR buffer[1024];

 rc = SQLRowCount(phstmt, &pcrow);
 CHECKTEST(lpSrvr, SQL_SUCCESS, rc, "SQLRowCount");
DoAbort:
 sprintf(buffer,"%s -> affected rows = %ld.",stmt, pcrow);
 szLogPrintf(lpSrvr, FALSE,"\t%s\r\n",buffer);

 return rc;
}

/* ************************************************************************* */
