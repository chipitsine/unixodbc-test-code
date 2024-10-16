/*!**********************************************************************

  module: o3a.c

  -----------------------------------------------------------------------

  responsible:  Holger Bischoff (D027451 - Holger.Bischoff@SAP-AG.de)

  special area: ODBC-Tests

  description: Convert a 'a' to SQL_C_LONG, SQL_C_SHORT, ...
               -> SQLSTATE 22018

  -----------------------------------------------------------------------





    ========== licence begin  GPL
    Copyright (c) 1999-2004 SAP AG

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
    ========== licence end



************************************************************************/

#define PROGNAME "o3bigint"

#include "o3util.c"
#include "o3cnvtab.h"

SQLLEN lenIndi   = 0;
SQLLEN lenIndiChar   = 0;

static valueStruct charValues[] = {
    { "a", SQL_C_CHAR, &lenIndiChar, SQL_NTS },

    { NULL, 0, NULL, 0 }
}; /* numericValues */

UCHAR  charVal[1024];

SDWORD longVal;
SWORD shortVal;
float floatVal;
double doubleVal;

static testTypesStruct testTypeLong[] = {
    { SQL_C_LONG,
      &longVal, sizeof(SWORD), &lenIndi },
    { SQL_C_SHORT,
      &shortVal, sizeof(SWORD), &lenIndi },
    { SQL_C_FLOAT,
      &floatVal, sizeof(SWORD), &lenIndi },
    { SQL_C_DOUBLE,
      &doubleVal, sizeof(SWORD), &lenIndi },
    { 0,
      NULL, 0, NULL }
}; /* TestTypeLong */


static testTabStruct testTab[] = {
    { SQL_CHAR, "SQL_CHAR",
      "(n integer, col CHAR(100))",
      charValues,
      testTypeLong},
    { 0, "", "", NULL, NULL}
}; /* TestTab */


int main(int argc, argv_t argv) {
    RETCODE retcode;
    HENV    henv  = SQL_NULL_HENV;
    HDBC    hdbc  = SQL_NULL_HDBC;

    ProcessCmdArguments(argc,argv);

    StartTest( PROGNAME );

    retcode = FullConnect( &henv, &hdbc );
    if (retcode != SQL_SUCCESS) {
        return 0;
    }; /* if */

    ConversionTest( hdbc, testTab );
    FullDisconnect( &henv, &hdbc );

    EndTest( PROGNAME );
 exit:
    return 0;
}; /* main */

