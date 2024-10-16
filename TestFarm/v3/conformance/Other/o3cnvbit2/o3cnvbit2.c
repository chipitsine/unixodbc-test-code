/*!***************************************************************************

  module:        o3cnvbit2.c

  -------------------------------------------------------------------------

  responsible:   D024584

  special area:  conversion from C types to SQL_BIT

  description:

  [optional tags]:

  -------------------------------------------------------------------------


\if EMIT_LICENCE


    ========== licence begin  GPL
    Copyright (c) 2001-2004 SAP AG

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

\endif
*****************************************************************************/

/*===========================================================================*
 *  INCLUDES                                                                 *
 *===========================================================================*/
#define _ODBC
#include "uniutil.c"

/*===========================================================================*
 *  DEFINES                                                                  *
 *===========================================================================*/
#define TABNAME "O3CNVBIT2"
#define COLDEF "col1 boolean, col2 boolean"

/*===========================================================================*
 *  FUNCTIONS                                                                *
 *===========================================================================*/
int main(int argc, argv_t argv) {
	SQLHENV henv   = SQL_NULL_HENV;
	SQLHENV *phenv = &henv;
	SQLHDBC hdbc   = SQL_NULL_HDBC;
	SQLHDBC *phdbc = &hdbc;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLHSTMT *phstmt = &hstmt;
	SDWORD pcbValue;
	RETCODE rc;

        int id, line;

        SQLCHAR bit, vBit1, vBit2;
        SQLCHAR buffer[512], insert[512], select[512];

        SQLCHAR vChar1[10], vChar2[10], vBin[10];
        SQLWCHAR vWchar1[10], vWchar2[10];
        SQLINTEGER vInt1, vInt2;
        SQLINTEGER vLong1, vLong2;
        short vShort1, vShort2;
        SQLREAL vFloat1, vFloat2;
        SQLDOUBLE vDouble1, vDouble2;
        SQL_NUMERIC_STRUCT vNumeric1, vNumeric2;

        char vTiny;
        /*
        __int64 vBig;
        */

	BOOL	okflag;

        ProcessCmdArguments(argc,argv);

	/* Connection to database and creation table rutine */
	FullConnect(phenv, phdbc);
        rc = SQLAllocStmt(hdbc, phstmt);
        SPRINTF(buffer," SQLAllocStmt retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);
	DropTestTable(hstmt, TABNAME);
	CreateTestTable(hstmt, TABNAME, COLDEF);

	/* Prepare for insert data in to the table */
	STRCPY(insert, "insert into ");
	STRCAT(insert, TABNAME);
	STRCAT(insert, " values (?, ?)");
	SPRINTF(buffer,"Insert Stmt = '%s'"CRLF, insert);
	PRINTF(buffer);
	rc = SQLPrepare(hstmt, insert, SQL_NTS);
	SPRINTF(buffer," SQLPrepare retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

	/* Insert SQL_C_CHAR data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                              SQL_BIT, 0, 0, vChar1, sizeof(vChar1), NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_CHAR retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
                              SQL_BIT, 0, 0, vChar2, sizeof(vChar2), NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_CHAR retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        strcpy (vChar1, "0");
        strcpy (vChar2, "1");
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);


	/* Insert SQL_C_WCHAR data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR,
                              SQL_BIT, 0, 0, vWchar1, sizeof(vWchar1), NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_WCHAR retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR,
                              SQL_BIT, 0, 0, vWchar2, sizeof(vWchar2), NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_WCHAR retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

	StringToWString(vWchar1, sizeof(vWchar1), NULL, TRUE, "0", 2, NULL);
	StringToWString(vWchar2, sizeof(vWchar2), NULL, TRUE, "1", 2, NULL);
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);


	/* Insert SQL_C_SHORT data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT,
                              SQL_BIT, 0, 0, &vShort1, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_SHORT retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT,
                              SQL_BIT, 0, 0, &vShort2, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_SHORT retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        vShort1 = 0;
        vShort2 = 1;
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);



	/* Insert SQL_C_LONG data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
                              SQL_BIT, 0, 0, &vLong1, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_LONG retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG,
                              SQL_BIT, 0, 0, &vLong2, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_LONG retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        vLong1 = 0;
        vLong2 = 1;
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);



	/* Insert SQL_C_FLOAT data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_FLOAT,
                              SQL_BIT, 0, 0, &vFloat1, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_FLOAT retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT,
                              SQL_BIT, 0, 0, &vFloat2, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_FLOAT retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        vFloat1 = 0.;
        vFloat2 = 1.;
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);


	/* Insert SQL_C_DOUBLE data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE,
                              SQL_BIT, 0, 0, &vDouble1, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_DOUBLE retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE,
                              SQL_BIT, 0, 0, &vDouble2, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_DOUBLE retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        vDouble1 = 0.;
        vDouble2 = 1.;
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

	/* Insert SQL_C_NUMERIC data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_NUMERIC,
                              SQL_BIT, 0, 0, &vNumeric1, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_NUMERIC retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_NUMERIC,
                              SQL_BIT, 0, 0, &vNumeric2, SQL_NTS, NULL);
	SPRINTF(buffer," SQLBindParameter SQL_C_NUMERIC retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        vNumeric1.precision = 1;
        vNumeric1.scale = 0;
        vNumeric1.sign = 0;
        MEMSET (vNumeric1.val, 0, sizeof (vNumeric1.val));
        vNumeric1.val[0] = 0;

        vNumeric2.precision = 1;
        vNumeric2.scale = 0;
        vNumeric2.sign = 0;
        MEMSET (vNumeric2.val, 0, sizeof (vNumeric2.val));
        vNumeric2.val[0] = 1;

	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);



	/* Select data from the table */
        rc = SQLFreeStmt (hstmt, SQL_CLOSE);

	STRCPY(select, "select * from ");
        STRCAT(select, TABNAME);
	SPRINTF(buffer,"Select Stmt= '%s'"CRLF, select);
	PRINTF(buffer);

	rc = SQLExecDirect(hstmt, select, SQL_NTS);
	SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        line = 0;
        okflag = TRUE;
        do {
            line++;
            rc = SQLGetData(hstmt, 1, SQL_C_BIT, &vBit1, sizeof (vBit1), NULL);
            SPRINTF (buffer, "%d. Line\tSQLGetData rc1: %d\t", line, rc);
            PRINTF(buffer);
            rc = SQLGetData(hstmt, 2, SQL_C_BIT, &vBit2, sizeof (vBit2), NULL);
            SPRINTF (buffer, "SQLGetData rc2: %d\t Bit1: %d\tBit2: %d\n", rc, vBit1, vBit2);
            PRINTF(buffer);

            okflag = okflag && (vBit1 == SQL_FALSE) && (vBit2 == SQL_TRUE);
            rc = SQLFetch(hstmt);
        } while (rc == SQL_SUCCESS && okflag == TRUE);

	ErrorCheck(SQL_HANDLE_STMT, hstmt);

	if (okflag == TRUE)
		PRINTF("Test state: OK\n\n");
	else
		PRINTF("Test state: Failure\n\n");
	FullDisconnect(phenv, phdbc);
	return 0;
}
