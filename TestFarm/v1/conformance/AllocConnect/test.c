#define ODBCVER	0x0100

#include <tst.h>

/*
  main
  
  The entry point for this program - it all starts here.
*/  
int main()
{
  HENV     henv = SQL_NULL_HENV;
  HDBC FAR hdbc = SQL_NULL_HDBC;
  RETCODE  ret;

  ret = TSTAllocEnv(&henv);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTAllocConnect(henv, &hdbc);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTFreeConnect(hdbc);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTFreeEnv(henv);
  if (!SQL_SUCCEEDED(ret))
    exit(1);
	
  exit(0);
}


