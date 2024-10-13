#include "tst.h"

SQLRETURN TSTAllocHandle(SQLSMALLINT handle_type, SQLHANDLE input_handle, SQLHANDLE *output_handle)
{
    SQLRETURN ret = SQLAllocHandle(handle_type, input_handle, output_handle);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:/* any diag info is in the output handle */
            switch ( handle_type )
            {
                case SQL_HANDLE_ENV:
                    tst_print_diagnostics((SQLHENV)(*output_handle), NULL, NULL, NULL);
                    return ret;

                case SQL_HANDLE_DBC:
                    tst_print_diagnostics(NULL, (SQLHDBC)(*output_handle), NULL, NULL);
                    return ret;

                case SQL_HANDLE_STMT:
                    tst_print_diagnostics(NULL, NULL, (SQLHSTMT)(*output_handle), NULL);
                    return ret;

                case SQL_HANDLE_DESC:
                    tst_print_diagnostics(NULL, NULL, NULL, (SQLHDESC)(*output_handle));
                    return ret;

                default:
                    fprintf(stderr, "[sql_alloc_handle] Invalid handle type %d\n", handle_type);
            }
            break;

        case SQL_INVALID_HANDLE:
            fprintf(stderr, "[sql_alloc_handle] SQL_INVALID_HANDLE\n");
            return ret;

        case SQL_ERROR: /* any diag info is in input_handle */
            switch ( handle_type )
            {
                case SQL_HANDLE_ENV:
                    fprintf(stderr, "[sql_alloc_handle] SQL_ERROR\n");
                    return ret;
                case SQL_HANDLE_DBC:
                    tst_print_diagnostics((SQLHENV)input_handle, NULL, NULL, NULL);
                    return ret;
                case SQL_HANDLE_STMT:
                    tst_print_diagnostics(NULL, (SQLHDBC)input_handle, NULL, NULL);
                    return ret;
                case SQL_HANDLE_DESC:
                    tst_print_diagnostics(NULL, NULL, (SQLHSTMT)input_handle, NULL);
                    return ret;
                default:
                    fprintf(stderr, "[sql_alloc_handle] Invalid handle type %d\n", handle_type);
            }
            break;

        default:
            fprintf(stderr, "[sql_alloc_handle] Invalid return value %d\n", ret);
    }

    return SQL_ERROR;
}

SQLRETURN TSTFreeHandle(SQLSMALLINT handle_type, SQLHANDLE handle)
{
    SQLRETURN ret = SQLFreeHandle(handle_type, handle);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_ERROR:
            switch ( handle_type )
            {
                case SQL_HANDLE_ENV:
                    tst_print_diagnostics((SQLHENV)handle, NULL, NULL, NULL);
                    return ret;

                case SQL_HANDLE_DBC:
                    tst_print_diagnostics(NULL, (SQLHDBC)handle, NULL, NULL);
                    return ret;

                case SQL_HANDLE_STMT:
                    tst_print_diagnostics(NULL, NULL, (SQLHSTMT)handle, NULL);
                    return ret;

                case SQL_HANDLE_DESC:
                    tst_print_diagnostics(NULL, NULL, NULL, (SQLHDESC)handle);
                    return ret;

                default:
                    fprintf(stderr, "[sql_free_handle] Invalid handle type %d\n", handle_type);
            }
            break;

        case SQL_INVALID_HANDLE:
            fprintf(stderr, "[sql_free_handle] SQL_INVALID_HANDLE\n");
            return ret;

        default:
            fprintf(stderr, "[sql_free_handle] Invalid return value %d\n", ret);
    }

    return SQL_ERROR;
}

SQLRETURN TSTSetEnvAttr(SQLHENV environment_handle, SQLINTEGER attribute, SQLPOINTER value, SQLINTEGER string_length)
{
    SQLRETURN ret = SQLSetEnvAttr(environment_handle, attribute, value, string_length);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(environment_handle, NULL, NULL, NULL);
            return ret;

        case SQL_INVALID_HANDLE:
            fprintf(stderr, "[sql_set_env_attr] SQL_INVALID_HANDLE\n");
            return ret;

        default:
            fprintf(stderr, "[sql_set_env_attr] Invalid return value %d\n", ret);
    }

    return SQL_ERROR;
}

SQLRETURN TSTConnect(SQLHDBC connection_handle, SQLCHAR *server_name, SQLSMALLINT name_length1, SQLCHAR *user_name, SQLSMALLINT name_length2, SQLCHAR *authentication, SQLSMALLINT name_length3)
{
    SQLRETURN ret = SQLConnect(connection_handle, server_name, name_length1, user_name, name_length2, authentication, name_length3);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(NULL, connection_handle, NULL, NULL);
            return ret;

        case SQL_INVALID_HANDLE:
            fprintf(stderr, "[sql_connect] SQL_INVALID_HANDLE\n");
            return ret;

        default:
            fprintf(stderr, "[sql_connect] Invalid return value %d\n", ret);
    }

    return SQL_ERROR;
}

SQLRETURN TSTDisconnect(SQLHDBC connection_handle)
{
    SQLRETURN ret = SQLDisconnect(connection_handle);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(NULL, connection_handle, NULL, NULL);
            return ret;

        case SQL_INVALID_HANDLE:
            fprintf(stderr, "[sql_disconnect] SQL_INVALID_HANDLE\n");
            return ret;

        default:
            fprintf(stderr, "[sql_disconnect] Invalid return value %d\n", ret);
    }

    return SQL_ERROR;
}

SQLRETURN TSTExecDirect(SQLHSTMT statement_handle, SQLCHAR *statement_text, SQLINTEGER text_length)
{
    SQLRETURN ret = SQLExecDirect(statement_handle, statement_text, text_length);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
            tst_print_diagnostics(NULL, NULL, statement_handle, NULL);
            return ret;

        case SQL_NEED_DATA:
        case SQL_STILL_EXECUTING:
            return ret;

        case SQL_ERROR:
            tst_print_diagnostics(NULL, NULL, statement_handle, NULL);
            return ret;

        case SQL_NO_DATA:
            return ret;

        case SQL_INVALID_HANDLE:
            fprintf(stderr, "[sql_exec_direct] SQL_INVALID_HANDLE\n");
            return ret;

        default:
            fprintf(stderr, "[sql_exec_direct] Invalid return value %d\n", ret);
    }

    return SQL_ERROR;
}

SQLRETURN TSTFetch(SQLHSTMT hstmt)
{
    SQLRETURN ret = SQLFetch(hstmt);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt, SQL_NULL_HDESC);
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

SQLRETURN TSTNumResultCols(SQLHSTMT hstmt, SQLSMALLINT *column_count_ptr)
{
    SQLRETURN ret = SQLNumResultCols(hstmt, column_count_ptr);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt, SQL_NULL_HDESC);
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

SQLRETURN TSTGetData(SQLHSTMT hstmt, SQLUSMALLINT column_number, SQLSMALLINT target_type, SQLPOINTER target_value_ptr, SQLINTEGER buffer_length, SQLLEN *strlen_or_indptr)
{
    SQLRETURN ret = SQLGetData(hstmt, column_number, target_type, target_value_ptr, buffer_length, strlen_or_indptr);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt, SQL_NULL_HDESC);
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

SQLRETURN TSTGetTypeInfo(SQLHSTMT hstmt, SQLSMALLINT data_type)
{
    SQLRETURN ret = SQLGetTypeInfo(hstmt, data_type);

    switch ( ret )
    {
        case SQL_SUCCESS:
            return ret;

        case SQL_SUCCESS_WITH_INFO:
        case SQL_ERROR:
            tst_print_diagnostics(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt, SQL_NULL_HDESC);
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


/*
    tst_print_diagnostics
    
    Gets any diagnostic info for each handle and prints to stdout.    
*/
void tst_print_diagnostics(SQLHENV env, SQLHDBC dbc, SQLHSTMT stm, SQLHDESC des)
{
    SQLSMALLINT rec;
    SQLCHAR     sql_state[6];
    SQLINTEGER  native_error;
    SQLCHAR     message_text[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT text_length;

    sql_state[5] = '\0';

    if ( des )
    {
        for ( rec = 1; 
            SQLGetDiagRec(SQL_HANDLE_DESC, 
                          des, 
                          rec, 
                          sql_state, 
                          &native_error, 
                          message_text, 
                          SQL_MAX_MESSAGE_LENGTH, 
                          &text_length) == SQL_SUCCESS;
            rec++ )
        {
            message_text[SQL_MAX_MESSAGE_LENGTH - 1] = '\0';
            fprintf(stderr, "[%s]%s\n", sql_state, message_text);
        }
    }

    if ( stm )
    {
        for ( rec = 1; 
            SQLGetDiagRec(SQL_HANDLE_STMT, 
                          stm, 
                          rec, 
                          sql_state, 
                          &native_error, 
                          message_text, 
                          SQL_MAX_MESSAGE_LENGTH, 
                          &text_length) == SQL_SUCCESS;
            rec++ )
        {
            message_text[SQL_MAX_MESSAGE_LENGTH - 1] = '\0';
            fprintf(stderr, "[%s]%s\n", sql_state, message_text);
        }
    }

    if ( dbc )
    {
        for ( rec = 1; 
            SQLGetDiagRec(SQL_HANDLE_DBC, 
                          dbc, 
                          rec, 
                          sql_state, 
                          &native_error, 
                          message_text, 
                          SQL_MAX_MESSAGE_LENGTH, 
                          &text_length) == SQL_SUCCESS;
            rec++ )
        {
            message_text[SQL_MAX_MESSAGE_LENGTH - 1] = '\0';
            fprintf(stderr, "[%s]%s\n", sql_state, message_text);
        }
    }

    if ( env )
    {
        for ( rec = 1; 
            SQLGetDiagRec(SQL_HANDLE_ENV, 
                          env, 
                          rec, 
                          sql_state, 
                          &native_error, 
                          message_text, 
                          SQL_MAX_MESSAGE_LENGTH, 
                          &text_length) == SQL_SUCCESS;
            rec++ )
        {
            message_text[SQL_MAX_MESSAGE_LENGTH - 1] = '\0';
            fprintf(stderr, "[%s]%s\n", sql_state, message_text);
        }
    }
}


UCHAR FAR *cnvret(RETCODE retcode)
{
   static UCHAR str[30];
   switch(retcode) {
      WHCASESTRING(str, SQL_ERROR);
      WHCASESTRING(str, SQL_INVALID_HANDLE);
      WHCASESTRING(str, SQL_NEED_DATA);
      WHCASESTRING(str, SQL_NO_DATA_FOUND);
      WHCASESTRING(str, SQL_SUCCESS);
      WHCASESTRING(str, SQL_SUCCESS_WITH_INFO);
      WHCASESTRING(str, SQL_STILL_EXECUTING);
      default: {
         STRCPY(str, "*** invalid ***");
         break;
      }
   }
   return (str);
}

RETCODE GetColAll(SQLHSTMT hstmt)
{
   RETCODE retcode;
   SWORD col, pccol;
   char szMsg[100];
   SPRINTF(szMsg, "GetColAll"CRLF);
   PRINTF(szMsg);
   retcode = SQLNumResultCols (hstmt, &pccol);
   if (retcode == SQL_SUCCESS) {
      for (col=1; col <= pccol; col++) {
         SQLLEN fDesc;
         UCHAR FAR* pHostVar;
         SQLLEN cbHostVar;
         retcode = SQLColAttributes(hstmt, col, SQL_COLUMN_DISPLAY_SIZE,
            NULL, 0, NULL, &fDesc);
         if (retcode == SQL_SUCCESS) {
            if (fDesc >= 0)
               pHostVar = MALLOC((fDesc+1));
            else
               pHostVar = NULL;
            if (pHostVar) {
               retcode = SQLGetData (hstmt, col, SQL_C_CHAR, (PTR)pHostVar,
                  fDesc+1, &cbHostVar);
               if (retcode == SQL_SUCCESS) {
                  if (STRLEN(pHostVar) > sizeof(szMsg))
                     pHostVar[sizeof(szMsg) -1] = '\0';
                  SPRINTF(szMsg, "%s", pHostVar);
                  PRINTF(szMsg);
               }
               else {
                  SPRINTF(szMsg, "%s", cnvret(retcode));
                  PRINTF(szMsg);
               }
               PRINTF(":");
               FREE(pHostVar);
            }
            else {
               SPRINTF(szMsg, "MALLOC Faild"CRLF);
               PRINTF(szMsg);
               return(SQL_ERROR);
            }
         }
      }
      PRINTF(CRLF);
   }
   return(retcode);
}

UCHAR FAR *GetODBCFunctionName(WORD fn)
{
   static UCHAR str[40];
   switch(fn) {
WHCASESTRING(str, SQL_API_SQLALLOCCONNECT     );
WHCASESTRING(str, SQL_API_SQLALLOCENV         );
WHCASESTRING(str, SQL_API_SQLALLOCHANDLE      );
WHCASESTRING(str, SQL_API_SQLALLOCHANDLESTD   );
WHCASESTRING(str, SQL_API_SQLALLOCSTMT        );
WHCASESTRING(str, SQL_API_SQLBINDCOL          );
WHCASESTRING(str, SQL_API_SQLBINDPARAM        );
WHCASESTRING(str, SQL_API_SQLBINDPARAMETER    );
WHCASESTRING(str, SQL_API_SQLBULKOPERATIONS   );
WHCASESTRING(str, SQL_API_SQLCANCEL           );
WHCASESTRING(str, SQL_API_SQLCLOSECURSOR      );
WHCASESTRING(str, SQL_API_SQLCOLATTRIBUTES    );
WHCASESTRING(str, SQL_API_SQLCOLUMNS          );
WHCASESTRING(str, SQL_API_SQLCONNECT          );
WHCASESTRING(str, SQL_API_SQLCOPYDESC         );
WHCASESTRING(str, SQL_API_SQLDATASOURCES      );
WHCASESTRING(str, SQL_API_SQLDESCRIBECOL      );
WHCASESTRING(str, SQL_API_SQLDISCONNECT       );
WHCASESTRING(str, SQL_API_SQLDRIVERS         );
WHCASESTRING(str, SQL_API_SQLENDTRAN         );
WHCASESTRING(str, SQL_API_SQLERROR           );
WHCASESTRING(str, SQL_API_SQLEXECDIRECT      );
WHCASESTRING(str, SQL_API_SQLEXECUTE         );
WHCASESTRING(str, SQL_API_SQLFETCH           );
WHCASESTRING(str, SQL_API_SQLFETCHSCROLL     );
WHCASESTRING(str, SQL_API_SQLFREECONNECT     );
WHCASESTRING(str, SQL_API_SQLFREEENV         );
WHCASESTRING(str, SQL_API_SQLFREEHANDLE      );
WHCASESTRING(str, SQL_API_SQLFREESTMT        );
WHCASESTRING(str, SQL_API_SQLGETCONNECTATTR  );
WHCASESTRING(str, SQL_API_SQLGETCONNECTOPTION );
WHCASESTRING(str, SQL_API_SQLGETCURSORNAME   );
WHCASESTRING(str, SQL_API_SQLGETDATA         );
WHCASESTRING(str, SQL_API_SQLGETDESCFIELD    );
WHCASESTRING(str, SQL_API_SQLGETDESCREC      );
WHCASESTRING(str, SQL_API_SQLGETDIAGFIELD    );
WHCASESTRING(str, SQL_API_SQLGETDIAGREC      );
WHCASESTRING(str, SQL_API_SQLGETENVATTR      );
WHCASESTRING(str, SQL_API_SQLGETFUNCTIONS    );
WHCASESTRING(str, SQL_API_SQLGETINFO         );
WHCASESTRING(str, SQL_API_SQLGETSTMTATTR     );
WHCASESTRING(str, SQL_API_SQLGETSTMTOPTION   );
WHCASESTRING(str, SQL_API_SQLGETTYPEINFO     );
WHCASESTRING(str, SQL_API_SQLNUMRESULTCOLS   );
WHCASESTRING(str, SQL_API_SQLPARAMDATA       );
WHCASESTRING(str, SQL_API_SQLPREPARE         );
WHCASESTRING(str, SQL_API_SQLPUTDATA         );
WHCASESTRING(str, SQL_API_SQLROWCOUNT        );
WHCASESTRING(str, SQL_API_SQLSETCONNECTATTR  );
WHCASESTRING(str, SQL_API_SQLSETCONNECTOPTION );
WHCASESTRING(str, SQL_API_SQLSETCURSORNAME   );
WHCASESTRING(str, SQL_API_SQLSETDESCFIELD    );
WHCASESTRING(str, SQL_API_SQLSETDESCREC      );
WHCASESTRING(str, SQL_API_SQLSETENVATTR      );
WHCASESTRING(str, SQL_API_SQLSETPARAM        );
WHCASESTRING(str, SQL_API_SQLSETSTMTATTR     );
WHCASESTRING(str, SQL_API_SQLSETSTMTOPTION   );
WHCASESTRING(str, SQL_API_SQLSPECIALCOLUMNS  );
WHCASESTRING(str, SQL_API_SQLSTATISTICS      );
WHCASESTRING(str, SQL_API_SQLTABLES          );
WHCASESTRING(str, SQL_API_SQLTRANSACT        );

WHCASESTRING(str, SQL_API_SQLDRIVERCONNECT   );













WHCASESTRING(str, SQL_API_SQLBROWSECONNECT   );
WHCASESTRING(str, SQL_API_SQLCOLUMNPRIVILEGES);

WHCASESTRING(str, SQL_API_SQLDESCRIBEPARAM   );
WHCASESTRING(str, SQL_API_SQLEXTENDEDFETCH   );
WHCASESTRING(str, SQL_API_SQLFOREIGNKEYS     );
WHCASESTRING(str, SQL_API_SQLMORERESULTS     );
WHCASESTRING(str, SQL_API_SQLNATIVESQL       );
WHCASESTRING(str, SQL_API_SQLNUMPARAMS       );
WHCASESTRING(str, SQL_API_SQLPARAMOPTIONS    );
WHCASESTRING(str, SQL_API_SQLPRIMARYKEYS     );
WHCASESTRING(str, SQL_API_SQLPROCEDURECOLUMNS);
WHCASESTRING(str, SQL_API_SQLPROCEDURES      );
WHCASESTRING(str, SQL_API_SQLSETPOS          );
WHCASESTRING(str, SQL_API_SQLSETSCROLLOPTIONS);
WHCASESTRING(str, SQL_API_SQLTABLEPRIVILEGES );
      default: {
         STRCPY(str, "*** invalid ***");
         break;
      }
   }
   return str;
}

#define MAKECOLATRIBUTETYPE(x) UCHAR sz##x[TABLENAME_LEN+1];\
SWORD FAR cb##x;\
SQLLEN FAR f##x

#define SQLCOLATTRIBUTES(x) retcode = SQLColAttributes(hstmt, icol, x, \
sz##x, sizeof(sz##x), &cb##x, &f##x);\
if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)\
     return(1);

#define PRINTCHARVALUE(x,l) SPRINTF(buffer, "%-*s, ", l, sz##x);\
PRINTF(buffer);

#define PRINTNUMERICVALUE(x) SPRINTF(buffer, "%3d,", f##x);\
PRINTF(buffer)

int DisplayColAttributes(SQLHSTMT hstmt)
{
  RETCODE retcode;
  SWORD cols, icol;
  SWORD *pccol = &cols;
  char buffer[512];

  MAKECOLATRIBUTETYPE(SQL_COLUMN_NAME);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_DISPLAY_SIZE);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_TYPE_NAME);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_TYPE);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_LENGTH);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_SCALE);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_PRECISION);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_NULLABLE);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_CASE_SENSITIVE);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_MONEY);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_SEARCHABLE);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_UNSIGNED);
  MAKECOLATRIBUTETYPE(SQL_COLUMN_UPDATABLE);

  retcode = SQLNumResultCols (hstmt, pccol);
  SPRINTF(buffer, " SQLNumResultCols (%d) retcode: %d"CRLF, cols, retcode);
  PRINTF(buffer);
  if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
    SPRINTF(buffer, "COL  TYPE_NAME           TYP LEN SCA PRE DIS NUL CAS MON SEA UNS UPD COLUMNAME"CRLF );
    PRINTF(buffer);
    for (icol = 1; icol <= cols; icol++) {
      SQLCOLATTRIBUTES(SQL_COLUMN_NAME);
      SQLCOLATTRIBUTES(SQL_COLUMN_TYPE);
      SQLCOLATTRIBUTES(SQL_COLUMN_LENGTH);
      SQLCOLATTRIBUTES(SQL_COLUMN_SCALE);
      SQLCOLATTRIBUTES(SQL_COLUMN_PRECISION);
      SQLCOLATTRIBUTES(SQL_COLUMN_DISPLAY_SIZE);
      SQLCOLATTRIBUTES(SQL_COLUMN_NULLABLE);
      SQLCOLATTRIBUTES(SQL_COLUMN_CASE_SENSITIVE);
      SQLCOLATTRIBUTES(SQL_COLUMN_MONEY);
      SQLCOLATTRIBUTES(SQL_COLUMN_SEARCHABLE);
      SQLCOLATTRIBUTES(SQL_COLUMN_UNSIGNED);
      SQLCOLATTRIBUTES(SQL_COLUMN_UPDATABLE);
      SQLCOLATTRIBUTES(SQL_COLUMN_TYPE_NAME);

      SPRINTF(buffer, "%3d, ", icol);
      PRINTF(buffer);

      PRINTCHARVALUE(SQL_COLUMN_TYPE_NAME, 18);
      PRINTNUMERICVALUE(SQL_COLUMN_TYPE);
      PRINTNUMERICVALUE(SQL_COLUMN_LENGTH);
      PRINTNUMERICVALUE(SQL_COLUMN_SCALE);
      PRINTNUMERICVALUE(SQL_COLUMN_PRECISION);
      PRINTNUMERICVALUE(SQL_COLUMN_DISPLAY_SIZE);
      PRINTNUMERICVALUE(SQL_COLUMN_NULLABLE);
      PRINTNUMERICVALUE(SQL_COLUMN_CASE_SENSITIVE);
      PRINTNUMERICVALUE(SQL_COLUMN_MONEY);
      PRINTNUMERICVALUE(SQL_COLUMN_SEARCHABLE);
      PRINTNUMERICVALUE(SQL_COLUMN_UNSIGNED);
      PRINTNUMERICVALUE(SQL_COLUMN_UPDATABLE);
      PRINTCHARVALUE(SQL_COLUMN_NAME, TABLENAME_LEN);

      SPRINTF(buffer, CRLF);
      PRINTF(buffer);
    }
  }
  return 1;
}

UCHAR FAR* MakeLongName(UCHAR FAR *szShortName, UCHAR FAR *szLongName, DWORD cbNumberOfChars)
{
  int i=0;
  if ( szLongName ) {
    if (szShortName) {
      STRCPY(szLongName, szShortName);
      i = STRLEN(szLongName);
    }
    for (; i < cbNumberOfChars; i++)
      szLongName[i] = (UCHAR)('0' + ((i+1)%10));
    szLongName[cbNumberOfChars] = '\0';
  }
  return szLongName;
}
