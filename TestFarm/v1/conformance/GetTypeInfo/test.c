#include <tst.h>

/*
  main
  
  The entry point for this program - it all starts here.
*/  
int main(int argc, char *argv[])
{
  HENV      henv    = SQL_NULL_HENV;
  HDBC FAR  hdbc    = SQL_NULL_HDBC;
  HDBC FAR  hstm    = SQL_NULL_HSTMT;
  RETCODE   ret;
  int       row     = 0;
  int       rows    = 49;
  UCHAR FAR *szDSN  = NULL;
  UCHAR FAR *szUID  = NULL;
  UCHAR FAR *szPWD  = NULL;
  UWORD     col     = 0;
  SWORD     cols    = 0;
  char      value[256];
  SDWORD    str_len_or_ind;

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

  ret = TSTConnect(hdbc, szDSN, SQL_NTS, szUID, SQL_NTS, szPWD, SQL_NTS);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTAllocStmt(hdbc, &hstm);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTGetTypeInfo(hstm, SQL_ALL_TYPES);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  ret = TSTNumResultCols(hstm, &cols);
  if (!SQL_SUCCEEDED(ret))
    exit(1);

  while (SQL_SUCCEEDED(TSTFetch(hstm)))
  {
    row++;
/*
    for (col = 1; col <= cols; col++)
    {
      ret = TSTGetData(hstm, col, SQL_C_CHAR, (PTR)value, 255, &str_len_or_ind);
      if (!SQL_SUCCEEDED(ret))
        exit(1);
      if (str_len_or_ind == SQL_NULL_DATA)
        printf("<null>|");
      else if (str_len_or_ind == SQL_NO_TOTAL)
        printf("%s+|", value);
      else if (str_len_or_ind < 0)
        printf("<error>|");
      else
        printf("%s|", value);
    }
    printf("\n");
*/
  }

  ret = TSTFreeStmt(hstm, SQL_DROP);
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

#if 0
  if (row != rows)
  {
    fprintf(stderr, "[%s][%d] %d rows but expected %d\n", __FILE__, __LINE__, row, rows);
    exit(1);
  }
#endif

  exit(0);
}


