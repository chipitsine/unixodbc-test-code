/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : SQLTOOLS.C
**
** Usage        :
** Option       : --
**
**Purpose      : Auto Test DLL, einige SQL-Tools
**
** Call Example :
**		  1.) get_pfSqlType(pfSqlType, buffer);
**		  2.) get_pfNullable(pfSqlType, buffer);
**
****************************************************************************
*/
#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"

/* ------------------------------------------------------------------------- */
#define MAX_NAME_LEN 50
#define MAX_COL_NAME_LEN 18
/* ---------------------------------------------------------------------har- */

//*---------------------------------------------------------------------------------
//| Get_pfSqlType:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE Get_pfSqlType(SWORD pfSqlType, UCHAR FAR * buffer)
{
 switch (pfSqlType) {
	case SQL_BIGINT :
			lstrcpy(buffer,"SQL_BIGINT");
			break;
	case SQL_BINARY :
			lstrcpy(buffer,"SQL_BINARY");
			break;
	case SQL_BIT    : lstrcpy(buffer,"SQL_BIT");
			  break;
	case SQL_CHAR   :
			lstrcpy(buffer,"SQL_CHAR");
			break;
	case SQL_DATE   :
			lstrcpy(buffer,"SQL_DATE");
			break;
	case SQL_DECIMAL :
			lstrcpy(buffer,"SQL_DECIMAL");
			break;
	case SQL_DOUBLE :
			lstrcpy(buffer,"SQL_DOUBLE");
			break;
	case SQL_FLOAT :
			lstrcpy(buffer,"SQL_FLOAT");
			break;
	case SQL_INTEGER :
			lstrcpy(buffer,"SQL_INTEGER");
			break;
	case SQL_LONGVARBINARY :
			lstrcpy(buffer,"SQL_LONGVARBINARY");
			break;
	case SQL_LONGVARCHAR :
			lstrcpy(buffer,"SQL_LONGVARCHAR");
			break;
	case SQL_NUMERIC :
			lstrcpy(buffer,"SQL_NUMERIC");
			break;
	case SQL_REAL :
			lstrcpy(buffer,"SQL_REAL");
			break;
	case SQL_SMALLINT :
			lstrcpy(buffer,"SQL_SMALLINT");
			break;
	case SQL_TIME :
			lstrcpy(buffer,"SQL_TIME");
			break;
	case SQL_TIMESTAMP :
			lstrcpy(buffer,"SQL_TIMESTAMP");
			break;
	case SQL_TINYINT :
			lstrcpy(buffer,"SQL_TINYINT");
			break;
	case SQL_VARBINARY :
			lstrcpy(buffer,"SQL_VARBINARY");
			break;
	case SQL_VARCHAR :
			lstrcpy(buffer,"SQL_VARCHAR");
			break;
	}
  return(0);
}
//*---------------------------------------------------------------------------------
//| Get_pfNullable:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE Get_pfNullable(SWORD pfNullable, UCHAR FAR * buffer)
{
 switch (pfNullable) {
	case SQL_NO_NULLS :
			lstrcpy(buffer,"SQL_NO_NULLS");
			break;
	case SQL_NULLABLE :
			lstrcpy(buffer,"SQL_NULLABLE");
			break;
	case SQL_NULLABLE_UNKNOWN :
			lstrcpy(buffer,"SQL_NULL_UNKNOWN");
			break;
	}
  return(0);
}
//*---------------------------------------------------------------------------------
//| Get_BoolValue:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE Get_BoolValue(SWORD pfDesc, UCHAR FAR * buffer)
{
 switch (pfDesc) {
	case TRUE :
			lstrcpy(buffer,"TRUE");
			break;
	case FALSE :
			lstrcpy(buffer,"FALSE");
			break;
	default    :
			lstrcpy(buffer,"<NULL>");
			break;
	}
  return(0);
}
//*---------------------------------------------------------------------------------
//| Get_Searchable:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE Get_Searchable(SWORD pfDesc, UCHAR FAR * buffer)
{
 switch (pfDesc) {
	case SQL_UNSEARCHABLE :
			lstrcpy(buffer,"SQL_UNSEARCHABLE");
			break;
	case SQL_LIKE_ONLY :
			lstrcpy(buffer,"SQL_LIKE_ONLY");
			break;
	case SQL_ALL_EXCEPT_LIKE :
			lstrcpy(buffer,"SQL_ALL_EXCEPT_LIKE");
			break;
	case SQL_SEARCHABLE :
			lstrcpy(buffer,"SQL_SEARCHABLE");
			break;
	default    :
			lstrcpy(buffer,"<NULL>");
			break;
	}
  return(0);
}
//*---------------------------------------------------------------------------------
//| Get_Updatable:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE Get_Updatable(SWORD pfDesc, UCHAR FAR * buffer)
{
 switch (pfDesc) {
	case SQL_ATTR_READONLY :
			lstrcpy(buffer,"SQL_ATTR_READONLY");
			break;
	case SQL_ATTR_WRITE :
			lstrcpy(buffer,"SQL_ATTR_WRITE");
			break;
	case SQL_ATTR_READWRITE_UNKNOWN :
			lstrcpy(buffer,"SQL_ATTR_READWRITE_UNKNOWN");
			break;
	default    :
			lstrcpy(buffer,"<NULL>");
			break;
	}
  return(0);
}
//*---------------------------------------------------------------------------------
//| Get_Updatable:
//|
//|
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE lst_ColumnNames(HENV FAR * phenv, HDBC FAR * phdbc,
		     HSTMT FAR * phstmt, lpSERVERINFO lpSrvr, SWORD outcol)
{
	RETCODE                 rc;
	SWORD                   sErr=0;
/* ------------------------------------------------------------------------- */
	UWORD			icol;
	UCHAR                   szColName[MAX_NAME_LEN];
	SWORD                   cbColNameMax;
	SWORD                   pcbColName;
	SWORD			pfSqlType;
	UDWORD			pcbColDef;
	SWORD			pibScale;
	SWORD			pfNullable;
/* ------------------------------------------------------------------------- */
	szLogPrintf(lpSrvr, FALSE,"\tColumns->|");

	icol = 1; cbColNameMax = MAX_COL_NAME_LEN;
	while (icol <= outcol) {
		rc = SQLDescribeCol(*phstmt, icol, szColName, cbColNameMax, &pcbColName,
				    &pfSqlType, &pcbColDef, &pibScale, &pfNullable);

		szLogPrintf(lpSrvr, FALSE,"%s|",szColName);
		icol++;
	}
	szLogPrintf(lpSrvr, FALSE,"\r\n");

return(rc);
}
/* ------------------------------------------------------------------------- */

/* ************************************************************************* */

/*
   --------------------------------------------------------------------------
  | PrintString:
  |
  |
  | Returns:
   --------------------------------------------------------------------------
*/
#define MAX_OUTPUT	256
int PrintString(UCHAR *StrLong, DWORD Strlen, UCHAR *StrFirst,
		      lpSERVERINFO lpSrvr)
{
 SQLRETURN rc;
 UCHAR str[MAX_OUTPUT+1];
 UWORD i,j;

 rc=SQL_SUCCESS;

 szLogPrintf(lpSrvr, FALSE,"\t%s (%d): '", StrFirst, Strlen);
 i=0;
 while (i < Strlen) {
   for (j=0; j <= MAX_OUTPUT; j++) {
	str[j]=StrLong[i];
	if (i == Strlen) break; else i++;
   }
   str[j]='\0';
   szLogPrintf(lpSrvr, FALSE,"%s", str);
 }
 szLogPrintf(lpSrvr, FALSE,"'\r\n");

 return (0);
}
