#include "tst.h"

RETCODE TSTAllocEnv(HENV FAR *phenv)
{
  RETCODE ret = SQLAllocEnv(phenv);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_ERROR:
    fprintf(stderr, "[TSTAllocEnv] SQL_ERROR\n");
    break;

  default:
    fprintf(stderr, "[TSTAllocEnv] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTAllocConnect(HENV henv, HDBC FAR *phdbc)
{
  RETCODE ret = SQLAllocConnect(henv, phdbc);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
    tst_print_errors(SQL_NULL_HENV, *phdbc, SQL_NULL_HSTMT);
    return ret;

  case SQL_ERROR:
    tst_print_errors(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTAllocConnect] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTAllocConnect] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTAllocStmt(HDBC hdbc, HSTMT FAR *phstmt)
{
  RETCODE ret = SQLAllocStmt(hdbc, phstmt);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, *phstmt);
    return ret;

  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTAllocStmt] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTAllocStmt] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTFreeEnv(HENV henv)
{
  RETCODE ret = SQLFreeEnv(henv);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
    /* spec suggests that handle still viable when this happens */
    tst_print_errors(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);
    return ret;

  case SQL_ERROR:
    tst_print_errors(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTFreeEnv] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTFreeEnv] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTFreeConnect(HDBC hdbc)
{
  RETCODE ret = SQLFreeConnect(hdbc);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
    /* spec suggests that handle still viable when this happens */
    tst_print_errors(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT);
    return ret;

  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTFreeConnect] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTFreeConnect] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTFreeStmt(HSTMT hstmt, UWORD fOption)
{
  RETCODE ret = SQLFreeStmt(hstmt, fOption);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
    /* spec suggests that handle still viable when this happens */
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTFreeStmt] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTFreeStmt] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTConnect(HDBC hdbc, UCHAR FAR *szDSN, SWORD cbDSN, UCHAR FAR *szUID, SWORD cbUID, UCHAR FAR *szAuthStr, SWORD cbAuthStr)
{
  RETCODE ret = SQLConnect(hdbc, szDSN, cbDSN, szUID, cbUID, szAuthStr, cbAuthStr);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTConnect] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTConnect] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTDisconnect(HDBC hdbc)
{
  RETCODE ret = SQLDisconnect(hdbc);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTDisconnect] SQL_INVALID_HANDLE\n");
    return ret;

  default:
    fprintf(stderr, "[TSTDisconnect] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTExecDirect(HSTMT hstmt, UCHAR FAR *szSqlStr, SDWORD cbSqlStr)
{
  RETCODE ret = SQLExecDirect(hstmt, szSqlStr, cbSqlStr);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTExecDirect] SQL_INVALID_HANDLE\n");
    return ret;

  case SQL_NEED_DATA:
  case SQL_STILL_EXECUTING:
    return ret;

  default:
    fprintf(stderr, "[TSTExecDirect] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTFetch(HSTMT hstmt)
{
  RETCODE ret = SQLFetch(hstmt);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTFetch] SQL_INVALID_HANDLE\n");
    return ret;

  case 100: /* SQL_NO_DATA_FOUND */
  case SQL_STILL_EXECUTING:
    return ret;

  default:
    fprintf(stderr, "[TSTFetch] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTNumResultCols(HSTMT hstmt, SWORD FAR *pccol)
{
  RETCODE ret = SQLNumResultCols(hstmt, pccol);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTNumResultCols] SQL_INVALID_HANDLE\n");
    return ret;

  case SQL_STILL_EXECUTING:
    return ret;

  default:
    fprintf(stderr, "[TSTNumResultCols] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTGetData(HSTMT hstmt, UWORD icol, SWORD fCType, PTR rgbValue, SDWORD cbValueMax, SDWORD FAR *pcbValue)
{
  RETCODE ret = SQLGetData(hstmt, icol, fCType, rgbValue, cbValueMax, pcbValue);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTGetData] SQL_INVALID_HANDLE\n");
    return ret;

  case 100: /* SQL_NO_DATA_FOUND */
  case SQL_STILL_EXECUTING:
    return ret;

  default:
    fprintf(stderr, "[TSTGetData] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTGetTypeInfo(HSTMT hstmt, SWORD fSqlType)
{
  RETCODE ret = SQLGetTypeInfo(hstmt, fSqlType);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTGetTypeInfo] SQL_INVALID_HANDLE\n");
    return ret;

  case SQL_STILL_EXECUTING:
    return ret;

  default:
    fprintf(stderr, "[TSTGetTypeInfo] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

RETCODE TSTTables(HSTMT hstmt, UCHAR FAR *szTableQualifier, SWORD cbTableQualifier, UCHAR FAR *szTableOwner, SWORD cbTableOwner, UCHAR FAR *szTableName, SWORD cbTableName, UCHAR FAR *szTableType, SWORD cbTableType)
{
  RETCODE ret = SQLTables(hstmt, szTableQualifier, cbTableQualifier, szTableOwner, cbTableOwner, szTableName, cbTableName, szTableType, cbTableType);

  switch (ret)
  {
  case SQL_SUCCESS:
    return ret;

  case SQL_SUCCESS_WITH_INFO:
  case SQL_ERROR:
    tst_print_errors(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt);
    return ret;

  case SQL_INVALID_HANDLE:
    fprintf(stderr, "[TSTTables] SQL_INVALID_HANDLE\n");
    return ret;

  case SQL_STILL_EXECUTING:
    return ret;

  default:
    fprintf(stderr, "[TSTTables] Invalid return value %d\n", ret);
  }

  return SQL_ERROR;
}

void tst_print_errors(HENV henv, HDBC hdbc, HSTMT hstmt)
{
  RETCODE ret = SQL_SUCCESS;
  UCHAR  szSqlState[6];
  SDWORD  fNativeError;  
  UCHAR  szErrorMsg[SQL_MAX_MESSAGE_LENGTH];
  SWORD   cbErrorMsgMax = SQL_MAX_MESSAGE_LENGTH;
  SWORD   cbErrorMsg;
  int     nError = 0;

  szSqlState[5] = '\0';

  do
  {
    ret = SQLError(henv,
                   hdbc,
                   hstmt,
                   szSqlState,
                   &fNativeError,
                   szErrorMsg,
                   cbErrorMsgMax,
                   &cbErrorMsg);
    if (SQL_SUCCEEDED(ret))
    {
      nError++;
      szErrorMsg[SQL_MAX_MESSAGE_LENGTH - 1] = '\0';
      fprintf(stderr, "[%s]%s\n", szSqlState, szErrorMsg);
    }

    /* spec says 64 is max - any number larger is probably endless loop */
    if (nError > 64)
    {
      fprintf(stderr, "[tst_print_errors] Errors exceeded 64\n");
      break;
    }

  } while (SQL_SUCCEEDED(ret));

  if (nError < 1)
    fprintf(stderr, "[tst_print_errors] Called but no messages?\n");
}


