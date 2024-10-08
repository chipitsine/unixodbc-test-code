/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLGDSCR.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQLGETDESCREC
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------

#define TESTNAME "SQLGetDescRec"

#define TABLE	"GETDESCREC"
#define SQLSTMT1 "SELECT * FROM DUAL"
#define MAX_ROWS 5			/* Max. Zeilen			*/
#define MAX_NAME_LEN 256

//*-------------------------------------------------------------------------

RETCODE pa13CheckDescIdentifier(SQLHDESC DescriptorHandle,SQLSMALLINT RecNumber,
         SQLSMALLINT FieldIdentifier, SQLSMALLINT TypesDesc);

/* Descriptor Types	*/
#define API_DESC_IMP_NONE	(0)
#define API_DESC_IMP_APD	(1)
#define API_DESC_IMP_IPD	(2)
#define API_DESC_IMP_ARD	(3)
#define API_DESC_IMP_IRD	(4)

#define API_DESC_NONE      (0)
#define API_DESC_READ      (1)
#define API_DESC_RWRITE    (2)
#define API_DESC_UNUSED    (-1)

//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
//*------------------------------------------------------------------------
//| DoSQLGetDescRec:
//|
//|
//| Returns:
//*-----------------------------------------------------------------------
SWORD FAR PASCAL DoSQLGetDescRec(HENV FAR * phenv, HDBC FAR * phdbc,
				HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UCHAR                   create[MAX_NAME_LEN+1];
	UCHAR                   drop[MAX_NAME_LEN+1];
	UCHAR                   insert[MAX_NAME_LEN+1];
	UCHAR                   select[MAX_NAME_LEN+1];
	SQLCHAR		        buffer[MAX_NAME_LEN+1];

	UCHAR                   szCharData[MAX_NAME_LEN];
	SWORD		        i,j,count=0;
	SQLINTEGER		BufferLength;
	SQLINTEGER		StringLengthPtr=0;

	SQLHDESC                ard_hdesc;
	SQLHDESC                ird_hdesc;
	SQLHDESC                apd_hdesc;
	SQLHDESC                ipd_hdesc;

/* ------------------------------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
	/* - Connect ------------------------------------------------------- */
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HENV)");

	rc = SQLSetEnvAttr(*phenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLSetEnvAttr (HENV)");

	szLogPrintf(lpSrvr,FALSE,"\t SQL_ATTR_ODBC_VERSION : 'SQL_OV_ODBC3' \r\n");

	rc = SQLAllocHandle(SQL_HANDLE_DBC, *phenv, phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HDBC)");

	rc = SQLConnect(*phdbc, lpSrvr->szValidServer0, SQL_NTS,
			                  lpSrvr->szValidLogin0, SQL_NTS,
			                  lpSrvr->szValidPassword0, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLConnect");

	rc = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocHandle (HSTMT)");

	/* ----------------------------------------------------------------- */
	/* - Get Descriptor Handles ------------*/
	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetStmtAttr(*phstmt, SQL_ATTR_APP_ROW_DESC, &ard_hdesc, SQL_IS_INTEGER, NULL);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr");
	sprintf(buffer,"APP_ROW_DESC (APD) : '%p' ", ard_hdesc);
	szLogPrintf(lpSrvr, FALSE,"\t\t -> %s \r\n",buffer);

	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetStmtAttr(*phstmt,SQL_ATTR_APP_PARAM_DESC, &apd_hdesc, SQL_IS_INTEGER, NULL);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr");
	sprintf(buffer,"APP_PARAM_DESC (ARD) : '%p' ", apd_hdesc);
	szLogPrintf(lpSrvr, FALSE,"\t\t -> %s \r\n",buffer);

	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetStmtAttr(*phstmt,SQL_ATTR_IMP_ROW_DESC, &ird_hdesc, SQL_IS_INTEGER, NULL);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr");
	sprintf(buffer,"IMP_ROW_DESC (IRD) : '%p' ", ird_hdesc);
	szLogPrintf(lpSrvr, FALSE,"\t\t -> %s \r\n",buffer);

	BufferLength = SQL_IS_UINTEGER;
	rc = SQLGetStmtAttr(*phstmt,SQL_ATTR_IMP_PARAM_DESC, &ipd_hdesc, SQL_IS_INTEGER, NULL);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetConnectAttr");
	sprintf(buffer,"IMP_PARAM_DESC (IPD) : '%p' ", ipd_hdesc);
	szLogPrintf(lpSrvr, FALSE,"\t\t -> %s \r\n",buffer);

	/* --- Create Table 1. --------------------------------------------- */
	lstrcpy(create,"CREATE TABLE ");
	lstrcat(create,TABLE);
	lstrcat(create," ( NAME CHAR(50), AGE INTEGER, ADRESSE CHAR(80))");
	szLogPrintf(lpSrvr, FALSE,"\tCreate Stmt 1. = '%s'\r\n",create);

	rc = SQLExecDirect(*phstmt, create, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* --- Insert Table 1. ------------------------------------------- */
	 lstrcpy(insert,"INSERT INTO ");lstrcat(insert,TABLE);
	 lstrcat(insert," VALUES ('Heinrich', 44, 'Alt-Moabit 96, Berlin')");

	 for (j=0; j < MAX_ROWS; j++) {
	  szLogPrintf(lpSrvr, FALSE,"\tInsert Stmt 1. = <%d.> '%s'\r\n",j+1,insert);

	  rc = SQLExecDirect(*phstmt, insert, SQL_NTS);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
	 }

	/* --- Select Table 1. ------------------------------------------- */
	lstrcpy(select,"SELECT * FROM ");lstrcat(select,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tSelect Stmt 1. = '%s'\r\n",select);

	rc = SQLExecDirect(*phstmt, select, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");
	count = 1; i=1;
	while (rc != SQL_NO_DATA_FOUND) {
	 rc = SQLFetch(*phstmt);
	 DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetch");

	 if (rc != SQL_NO_DATA_FOUND) {
	  rc = SQLGetData(*phstmt, i, SQL_C_CHAR, szCharData, sizeof(szCharData), NULL);
	  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetData");

	  szLogPrintf(lpSrvr, FALSE,"\t Column %d (Pos.%d) = '%s'\r\n",i,count,szCharData);
	  count ++;
	 }
	}
	rc = SQLCloseCursor(*phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLCloseCursor");

	/* --- Drop Table 1. -------------------------------------------- */
	lstrcpy(drop,"DROP TABLE ");lstrcat(drop,TABLE);
	szLogPrintf(lpSrvr, FALSE,"\tDrop Stmt 1.= '%s'\r\n",drop);

	rc = SQLExecDirect(*phstmt, drop, SQL_NTS);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

	/* - Disconnect ---------------------------------------------------- */
	rc = SQLFreeHandle(SQL_HANDLE_STMT, *phstmt);
	DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HSTMT)");

	rc = SQLDisconnect(*phdbc);
   	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLDisconnect");

	rc = SQLFreeHandle(SQL_HANDLE_DBC, *phdbc);
	DIAGRECCHECK(SQL_HANDLE_DBC, *phdbc, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HDBC)");

	rc = SQLFreeHandle(SQL_HANDLE_ENV, *phenv);
	DIAGRECCHECK(SQL_HANDLE_ENV, *phenv, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeHandle (HENV)");

	// check for errors
	CHECKERRS(sErr);

DoAbort:
	return sErr;
}

/* ------------------------------------------------------------------------ */
/* Function : pa13CheckDescIdentifier */
/* ------------------------------------------------------------------------ */

RETCODE pa13CheckDescIdentifier
(SQLHDESC     DescriptorHandle,
 SQLSMALLINT  RecNumber,
 SQLSMALLINT  FieldIdentifier,
 SQLSMALLINT  TypesDesc)
{
   SQLRETURN            retcode;

   retcode = SQL_SUCCESS;

   if (TypesDesc == API_DESC_IMP_NONE) {
	retcode = API_DESC_RWRITE;
	goto error_exit;
   }

	/* ---------------------------------------------------------------- */
	switch (FieldIdentifier) {
	  case (SQL_DESC_ALLOC_TYPE) : {        /* Read */
	    retcode = API_DESC_READ;
	    break;
	  }
	  case (SQL_DESC_ARRAY_STATUS_PTR) : {  /* Read/Write */
	      retcode = API_DESC_RWRITE;
	    break;
	  }
	  case (SQL_DESC_COUNT) : {}            /* Read/Write, Read: IRD */
	  case (SQL_DESC_CONCISE_TYPE) : {}	/* Read/Write, Read: IRD */
	  case (SQL_DESC_DATETIME_INTERVAL_CODE): {}    /*Read/Write,Read:IRD*/
	  case (SQL_DESC_DATETIME_INTERVAL_PRECISION):{}/*Read/Write,Read:IRD*/
	  case (SQL_DESC_LENGTH) : {}		/* Read/Write, Read: IRD */
	  case (SQL_DESC_OCTET_LENGTH) : {}	/* Read/Write, Read: IRD */
	  case (SQL_DESC_PRECISION) : {}	/* Read/Write, Read: IRD */
	  case (SQL_DESC_SCALE) : {}		/* Read/Write, Read: IRD */
	  case (SQL_DESC_TYPE) : {	        /* Read/Write, Read: IRD */
	    if (TypesDesc != API_DESC_IMP_IRD) retcode = API_DESC_RWRITE;
	      else retcode = API_DESC_READ;
	    break;
	  }
	  case (SQL_DESC_ARRAY_SIZE) : {}       /* Read/Write,Unused:IRD,IPD */
	  case (SQL_DESC_BIND_OFFSET_PTR) : {}  /* Read/Write,Unused:IRD,IPD */
	  case (SQL_DESC_BIND_TYPE) : {}        /* Read/Write,Unused:IRD,IPD */
	  case (SQL_DESC_DATA_PTR) : {}	        /* Read/Write,Unused:IRD,IPD */
  	  case (SQL_DESC_INDICATOR_PTR) : {}    /* Read/Write,Unused:IRD,IPD */
	  case (SQL_DESC_OCTET_LENGTH_PTR) : {  /* Read/Write,Unused:IRD,IPD */
	    if ((TypesDesc != API_DESC_IMP_IRD) &&
      	  (TypesDesc != API_DESC_IMP_IPD)) retcode = API_DESC_RWRITE;
	      else retcode = API_DESC_UNUSED;
	    break;
	  }
	  case (SQL_DESC_ROWS_PROCESSED_PTR) : {/* Read/Write,Unused:ARD,APD */
	    if ((TypesDesc == API_DESC_IMP_IRD) ||
		     (TypesDesc == API_DESC_IMP_IPD)) retcode=API_DESC_RWRITE;
	      else retcode = API_DESC_UNUSED;
	    break;
	  }
	  case (SQL_DESC_AUTO_UNIQUE_VALUE) : {}/* Read: IRD, Unused */
	  case (SQL_DESC_BASE_COLUMN_NAME) : {} /* Read: IRD, Unused */
	  case (SQL_DESC_BASE_TABLE_NAME) : {}  /* Read: IRD, Unused */
	  case (SQL_DESC_CATALOG_NAME) : {}	/* Read: IRD, Unused */
	  case (SQL_DESC_DISPLAY_SIZE) : {}	/* Read: IRD, Unused */
	  case (SQL_DESC_LABEL) : {}		/* Read: IRD, Unused */
	  case (SQL_DESC_LITERAL_PREFIX) : {}	/* Read: IRD, Unused */
	  case (SQL_DESC_LITERAL_SUFFIX) : {} 	/* Read: IRD, Unused */
	  case (SQL_DESC_SCHEMA_NAME) : {}	/* Read: IRD, Unused */
	  case (SQL_DESC_SEARCHABLE) : {}       /* Read: IRD, Unused */
	  case (SQL_DESC_TABLE_NAME) : {}       /* Read: IRD, Unused */
	  case (SQL_DESC_UPDATABLE) : {     	/* Read: IRD, Unused */
	    if (TypesDesc == API_DESC_IMP_IRD) retcode = API_DESC_READ;
	      else retcode = API_DESC_UNUSED;
	    break;
	  }
	  case (SQL_DESC_CASE_SENSITIVE) : {}	/* Read: IRD,IPD, Unused */
	  case (SQL_DESC_FIXED_PREC_SCALE) : {} /* Read: IRD,IPD, Unused */
	  case (SQL_DESC_LOCAL_TYPE_NAME) : {}	/* Read: IRD,IPD, Unused */
	  case (SQL_DESC_NULLABLE) : {}		/* Read: IRD,IPD, Unused */
	  case (SQL_DESC_TYPE_NAME) : {}        /* Read: IRD,IPD, Unused */
	  case (SQL_DESC_UNSIGNED) : {	        /* Read: IRD,IPD, Unused */
	    if ((TypesDesc == API_DESC_IMP_IRD) ||
	        (TypesDesc == API_DESC_IMP_IPD)) retcode = API_DESC_READ;
	      else retcode = API_DESC_UNUSED;
	    break;
	  }
	  case (SQL_DESC_NAME) : {}             /* Read: IRD, R/W:IPD,Unused */
	  case (SQL_DESC_UNNAMED) : {		/* Read: IRD, R/W:IPD,Unused */
	    if (TypesDesc == API_DESC_IMP_IPD) retcode = API_DESC_RWRITE;
	     else if (TypesDesc == API_DESC_IMP_IRD) retcode = API_DESC_READ;
	     else retcode = API_DESC_UNUSED;
	    break;
	  }
	  case (SQL_DESC_PARAMETER_TYPE) : {	/* Read/Write:IPD, Unused */
	    if (TypesDesc == API_DESC_IMP_IPD) retcode = API_DESC_RWRITE;
	      else retcode = API_DESC_UNUSED;
	    break;
	  }
	  /* -------------------------------------------------------------- */
	  default : {
	    retcode = API_DESC_NONE;
	  }
	} /* switch */

	/* ---------------------------------------------------------------- */
error_exit:
   return(retcode);
}
/* ------------------------------------------------------------------------- */
