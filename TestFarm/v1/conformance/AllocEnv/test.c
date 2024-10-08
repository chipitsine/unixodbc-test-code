#include <tst.h>

/*
  main
  
  The entry point for this program - it all starts here.
*/  
int main()
{
  HENV    henv = SQL_NULL_HENV;
  RETCODE ret;

  ret = TSTAllocEnv(&henv);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTFreeEnv(henv);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  exit(0);
}


