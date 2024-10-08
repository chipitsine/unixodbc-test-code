/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLNUMCX.C
**
** Usage        : --
** Option       : --
**
** Purpose      : Auto Test DLL, Test von SQL_NUMERIC_STRUCT (put-/getdata Long)
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"
#include "funodbc3.h"

//*-------------------------------------------------------------------------
#define TESTNAME "SQLNUMERICSTRUCTX"

#define STR_LEN			128+1
#define MAX_NAME_LEN		50
#define MAX_COL_NAME_LEN 	18

#define NULL_VALUE "<NULL>"
#define MAX_INSERT 512
#define VALUE_ADD  1111

/* ------------------------------------------------------------------------- */
/* Funktion: aptanly

Analysiert einen in string_ptr uebergebenen Numerischen String und liefert
folgende Informationen
*/
/* ------------------------------------------------------------------------- */

#define API_STRLEN strlen
#define API_STRCAT strcat
#define API_STRCHR strchr
#define API_SPRINTF sprintf
#define API_STRCPY strcpy
#define API_STRNCPY strncpy

#define API_CHAR_LENGTH_CHAR (255)
#define SIGN_NEGATIV 1
#define SIGN_POSITIV 0

VOID aptanly_check ( UCHAR FAR *string_ptr, UWORD FAR *right_digits_ptr,
  UWORD FAR *left_digits_ptr,  UWORD FAR *sign_digits_ptr,
  UWORD FAR *sign_indicator_ptr, SWORD FAR *exponent_ptr);

VOID apdexp(UCHAR FAR *string_ptr, SWORD FAR *exponent_ptr);


//*****************************  Test Cases  *******************************
//*  The following functions implement the tests
//**************************************************************************
/*
 ------------------------------------------------------------------------
| DoSQLNumericStruct:
|
|
| Returns:
|     Number of Errors or TEST_ABORTED
 ------------------------------------------------------------------------
*/
SWORD FAR PASCAL DoSQLNumericStructX(HENV FAR * phenv, HDBC FAR * phdbc,
				  HSTMT FAR * phstmt, lpSERVERINFO lpSrvr)
{
    RETCODE rc;
    SWORD   sErr=0;
/* ------------------------------------------------------------------------- */
    UCHAR   buffer[512];
    UCHAR   stmt[256];
    SDWORD  local_value;
    SDWORD  local_long;
    SDWORD  save_long=0;
    UCHAR   local_str[SQL_MAX_NUMERIC_LEN];
    SWORD   i, xi, ixx, len;

    SQL_NUMERIC_STRUCT local_numeric;
    SQL_NUMERIC_STRUCT local_numeric_input;
    UDWORD  iValue;
    UDWORD  iValueInput;

    SDWORD  cbNumber;
    SDWORD  pcbValue;

    UWORD   right_digits;
    UWORD   left_digits;
    UWORD   sign_digits;
    UWORD   sign_indicator;
    SWORD   exponent = 0;
    UCHAR   local_string[API_CHAR_LENGTH_CHAR+1];
/* ------------------------------------------------------------------------- */
    char *sqltable  = "NUMCSTRUCTX";
    char *sqlcreate = "create table %s (keyx int, numv decimal(10))";
    char *sqlinsert = "insert into %s values (?, ?)";
    char *sqlselect = "select keyx, numv from %s";
    char *sqldrop   = "drop table %s";
/* ------------------------------------------------------------------------- */
    /* --- Connect DB ----------------------------------------------------- */
	// This test will assume that the ODBC handles passed in
	//              are NULL.  One could have this function do a connection
	//              and pass the handles to other test functions.
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

    /* --- Create Table -------------------------------------------------- */
    sprintf(stmt, sqlcreate, sqltable);
    szLogPrintf(lpSrvr,FALSE,"\t Create Table (%s)\r\n",stmt);

    rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

    /* --- Insert into Table --------------------------------------------- */
    sprintf(stmt, sqlinsert, sqltable);
    szLogPrintf(lpSrvr,FALSE,"\t Insert Table (%s)\r\n",stmt);

    rc = SQLPrepare(*phstmt, stmt, SQL_NTS);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLPrepare");

    rc = SQLBindParameter(*phstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT,
			SQL_INTEGER, 0, 0, &iValueInput, 0, &cbNumber);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

    rc = SQLBindParameter(*phstmt, 2, SQL_PARAM_INPUT, SQL_C_NUMERIC,
			SQL_NUMERIC, 0, 0, &local_numeric_input, 0, &cbNumber);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLBindParameter");

    szLogPrintf(lpSrvr,FALSE,"\t Insert into Table :\r\n",stmt);

    for (i=0; i < MAX_INSERT; i ++) {
      /* Insert positive value */
      iValueInput = i+1;
      local_long=save_long;
      sprintf(local_string, "%d",local_long);

      /* convert SQL_NUMERIC_STRUCT to local_numeric_input */
      aptanly_check(local_string, (UWORD FAR *)&right_digits,
	 		(UWORD FAR *)&left_digits, (UWORD FAR *)&sign_digits,
        		(UWORD FAR *)&sign_indicator,&exponent);

      /* set precisio, scale, sign value */
      local_numeric_input.precision=right_digits+left_digits;
      local_numeric_input.sign=(SQLCHAR)1;
      if (left_digits > 0) {
          local_numeric_input.scale=(SQLSCHAR) left_digits+1;
	  for (ixx=right_digits; ixx < local_numeric_input.precision; ixx ++)
		local_string[ixx] = local_string[ixx+1];
          local_string[ixx] = '\0';
	  strcat(local_string, "0");
      }

      /* set value to little endean mode (with string) */
      for (ixx=0; ixx < sizeof(local_long); ixx++) {
           local_numeric_input.val[ixx]=(char)((local_long >> (ixx*8)) & 0xff);
      }

      sprintf(buffer," keyx:%d - numv: %d (prec:%d scale:%d sign:%d)",
		iValueInput, local_long, local_numeric_input.precision,
	        local_numeric_input.scale, local_numeric_input.sign);
      szLogPrintf(lpSrvr,FALSE,"\t%s\r\n",buffer);

      rc = SQLExecute(*phstmt);
      DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecute");

      /* Insert negative value */
      iValueInput = i+1;
      local_long = save_long * (-1);
      sprintf(local_string, "%d",local_long);

      /* convert SQL_NUMERIC_STRUCT to local_numeric_input */
      aptanly_check(local_string, (UWORD FAR *)&right_digits,
	 		(UWORD FAR *)&left_digits, (UWORD FAR *)&sign_digits,
        		(UWORD FAR *)&sign_indicator,&exponent);

      /* set precisio, scale, sign value */
      local_numeric_input.precision=right_digits+left_digits;
      local_numeric_input.sign=(SQLCHAR)0;
      if (left_digits > 0) {
          local_numeric_input.scale=(SQLSCHAR) left_digits+1;
	  for (ixx=right_digits; ixx < local_numeric_input.precision; ixx ++)
		local_string[ixx] = local_string[ixx+1];
          local_string[ixx] = '\0';
	  strcat(local_string, "0");
      }

      /* set value to little endean mode (with string) */
      local_long=save_long;
      for (ixx=0; ixx < sizeof(local_long); ixx++) {
           local_numeric_input.val[ixx]=(char)((local_long >> (ixx*8)) & 0xff);
      }

      local_long = save_long * (-1);
      sprintf(buffer," keyx:%d - numv: %d (prec:%d scale:%d sign:%d)",
		iValueInput, local_long, local_numeric_input.precision,
	        local_numeric_input.scale, local_numeric_input.sign);
      szLogPrintf(lpSrvr,FALSE,"\t%s\r\n",buffer);

      rc = SQLExecute(*phstmt);
      DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecute");

      save_long += VALUE_ADD;
    }

    rc = SQLFreeStmt(*phstmt, SQL_DROP);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

  /* --- Select Table --------------------------------------------------- */
  /* --- Select table Data SQL_NUMERIC_STRUCT from table ---------------- */
    rc = SQLAllocStmt(*phdbc, phstmt);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLAllocStmt");

    sprintf(stmt, sqlselect, sqltable);
    szLogPrintf(lpSrvr,FALSE,"\t Select Table (%s)\r\n",stmt);

    rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
    DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

    szLogPrintf(lpSrvr,FALSE,"\t Fetch Table \r\n");

    rc=SQL_SUCCESS;
    while (rc != SQL_NO_DATA_FOUND) {
      rc = SQLFetch(*phstmt);
      DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFetch");

      if (rc != SQL_NO_DATA_FOUND) {
	rc = SQLGetData(*phstmt, (UWORD)1, SQL_C_ULONG, &iValue, sizeof(iValue),
			&pcbValue);
	if (rc != SQL_SUCCESS) {
          DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetData");
	}
	rc = SQLGetData(*phstmt, (UWORD)2, SQL_C_NUMERIC, &local_numeric,
			sizeof(SQL_NUMERIC_STRUCT), &pcbValue);
	if (rc != SQL_SUCCESS) {
          DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLGetData");
	}
        /* convert SQL_NUMERIC_STRUCT to local_value */
	local_value = 0;
        for (xi=0; xi < sizeof(local_value); xi++) {
	   local_value += ((unsigned char) local_numeric.val[xi] << (xi*8));
	}
        if (local_numeric.sign == 0) {
	       sprintf(local_str,"-%d",local_value);
	} else sprintf(local_str,"%d",local_value);
        if (local_numeric.scale > 0) {
		len = local_numeric.precision - local_numeric.scale;
		for (xi=local_numeric.precision; xi > len;xi--)
			local_str[xi+1] = local_str[xi];
		local_str[xi+1] = '.';
		local_str[local_numeric.precision+1]='\0';
	}

        /* Output SQL_NUMERIC_STRUCT value */
	sprintf(buffer, "keyx: %d - numv: %s (prec:%d scale:%d sign:%d)",
	       iValue, local_str, local_numeric.precision,
	       local_numeric.scale, local_numeric.sign);
        szLogPrintf(lpSrvr,FALSE,"\t%s\r\n",buffer);
      }
    }

  rc = SQLFreeStmt(*phstmt, SQL_CLOSE);
  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

  /* --- Drop Table --------------------------------------------------- */
  sprintf(stmt, sqldrop, sqltable);
  sprintf(buffer,"Drop Table (%s)\n", stmt);
  szLogPrintf(lpSrvr,FALSE,"\t Drop Table (%s)\r\n",stmt);

  rc = SQLExecDirect(*phstmt, stmt, SQL_NTS);
  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLExecDirect");

  rc = SQLFreeStmt(*phstmt, SQL_DROP);
  DIAGRECCHECK(SQL_HANDLE_STMT, *phstmt, 1, lpSrvr,SQL_SUCCESS, rc,"SQLFreeStmt");

  /* --- Disconnect -------------------------------------------------- */
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
/* ------------------------------------------------------------------------- */
/* Funktion: aptanly

Analysiert einen in string_ptr uebergebenen Numerischen String und liefert
folgende Informationen

right_digits_ptr : Nachkommastellen
left_digits_ptr  : Vorkommastellen
sign_digits_ptr  : Signifikanten Stellen (Vorzeichen, Vorkommastellen,
                   Exponent)
exponent_ptr     : Exponent des Wertes
*/
/* ------------------------------------------------------------------------- */
/*
VOID aptanly_check ( UCHAR FAR *string_ptr,
  UWORD FAR *right_digits_ptr,
  UWORD FAR *left_digits_ptr,
  UWORD FAR *sign_digits_ptr,
  UWORD FAR *sign_indicator_ptr,
  SWORD FAR *exponent_ptr)
{
  SWORD i, j, k;
  UWORD string_length;
  UCHAR FAR * e_ptr;
  UCHAR exponent[10];

  * drop exponetial part localy *
  if ((e_ptr = (UCHAR FAR *) API_STRCHR(string_ptr, 'e')) == (UCHAR FAR *) NULL)
     e_ptr = (UCHAR FAR *) API_STRCHR(string_ptr, 'E');
  if (e_ptr != NULL) {
     API_STRNCPY(exponent, e_ptr, sizeof(exponent)-1);
     *e_ptr = '\0';
  }

  string_length = (UWORD) API_STRLEN((UCHAR FAR *) string_ptr);
  i = j = k = 0;

  if (string_ptr [0]  == '-')  * set sign indicator    *
  {
    *sign_indicator_ptr = SIGN_NEGATIV;
    i = 1;
    k = 1;
  }
  else
  {
     if (string_ptr [0]  == '+') * ignore sign indicator *
	i = 1;
    *sign_indicator_ptr = SIGN_POSITIV;
  }

                            * delete leading blancs     *
  for (; string_ptr [i]  == ' ' &&
     i < (SWORD)(API_STRLEN(string_ptr) -1); i++);

  * delete leading zeroes     *
  for (; string_ptr [i]  == '0' &&
     i < (SWORD)(API_STRLEN(string_ptr) -1); i++);

  * delete appending blancs   *
  for (j = (UWORD) (API_STRLEN(string_ptr) - 1); j > 0 && string_ptr [j]  == ' '; j--);

  * delete appending zeroes   *
  * after decimal point       *
  if (((UCHAR FAR *) API_STRCHR(string_ptr+i, '.')) !=
       (UCHAR FAR *) NULL) {
    for (; string_ptr [j]  == '0' && j > 0; j--);
  }

  for (; i <= j; i++, k++)
     string_ptr [k] = string_ptr [i] ;

  if (k > 1 && string_ptr[k-1] == '.')
     k--;
  string_ptr [k]  = '\0';

  if (k == 1 && (string_ptr[0] == '-' || string_ptr[0] == '.')) {
     *sign_indicator_ptr = SIGN_POSITIV;
     string_ptr[0] = '0';
  }

  if (k == 2 && string_ptr[0] == '.' && string_ptr[1] == '0') {
     *sign_indicator_ptr = SIGN_POSITIV;
     string_ptr[0] = '0';
     string_ptr[1] = '\0';
  }

  * set position of dec. point*
  for (i = 0; string_ptr [i]  != '.' && string_ptr [i]  != '\0'; i++);

  * get nr of significant dig.*
  if (*sign_indicator_ptr == SIGN_NEGATIV && i == 1)
     *sign_digits_ptr = (UWORD) (2);
  if (*sign_indicator_ptr == SIGN_POSITIV && i == 1)
     *sign_digits_ptr = (UWORD) (1);
  else
     *sign_digits_ptr = i;

  * get nr of right digits    *
  *right_digits_ptr = i;

  * get nr of left digits     *
  if (i != (SWORD)API_STRLEN(string_ptr))
     *left_digits_ptr = (UWORD) (API_STRLEN(string_ptr) - i - 1);
  else
    *left_digits_ptr = 0;

  if (e_ptr != NULL) {
     API_STRCAT(string_ptr, exponent);
     *apdexp(string_ptr, exponent_ptr);*
  }
  else {
     *exponent_ptr = 0;
  }

  return;
}
*/
/* apdexp()   - Dependent routine to get the exponent as an   */
/*              numeric field                                 */
/*
VOID apdexp(UCHAR FAR *string_ptr, SWORD FAR *exponent_ptr)
{
  UCHAR FAR * e_ptr;

  if ((e_ptr = (UCHAR FAR *) API_STRCHR(string_ptr, 'e')) == (UCHAR FAR *) NULL)
     e_ptr = (UCHAR FAR *) API_STRCHR(string_ptr, 'E');
  if (e_ptr != NULL) {
     *(e_ptr) = 'E';
     *exponent_ptr = (SWORD)atoi(e_ptr+1);
  }
  else
     *exponent_ptr = 0;
  return;
}
*/
