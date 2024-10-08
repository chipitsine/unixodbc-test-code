#include <tst.h>

/*
  main
  
  The entry point for this program - it all starts here.
*/  
int main(int argc, char *argv[])
{
  HENV     henv = SQL_NULL_HENV;
  HDBC FAR hdbc = SQL_NULL_HDBC;
  RETCODE  ret;

  UCHAR FAR *szDSN = NULL;
  UCHAR FAR *szUID = NULL;
  UCHAR FAR *szPWD = NULL;

  if (argc >= 2 && argv[1][0])
    szDSN = argv[1];

  if (argc >= 3 && argv[2][0])
    szUID = argv[2];

  if (argc >= 4 && argv[3][0])
    szPWD = argv[3];

  ret = TSTAllocEnv(&henv);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTAllocConnect(henv, &hdbc);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

/*
printf("(%s)\n(%s)\n(%s)\n", szDSN, szUID, szPWD);
*/

  ret = TSTConnect(hdbc, szDSN, SQL_NTS, szUID, SQL_NTS, szPWD, SQL_NTS);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTDisconnect(hdbc);
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


