#include <tst.h>

/*
  main
  
  The entry point for this program - it all starts here.
*/  
int main()
{
  SQLHENV   env = NULL;
  SQLHDBC   dbc = NULL;
  SQLRETURN ret;

  ret = TSTAllocHandle(SQL_HANDLE_ENV, NULL, &env);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_UINTEGER);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTAllocHandle(SQL_HANDLE_DBC, env, &dbc);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTFreeHandle(SQL_HANDLE_DBC, dbc);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTFreeHandle(SQL_HANDLE_ENV, env);
  if (!SQL_SUCCEEDED(ret))
    exit(1);


  exit(0);
}


