/*!***************************************************************************

  module:        o3cnvbit.c

  -------------------------------------------------------------------------

  responsible:   D024584

  special area:  conversion from SQL_BIT C types.

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
#include "tst.h"
#include "uniutil.h"

/*===========================================================================*
 *  DEFINES                                                                  *
 *===========================================================================*/
#define TABNAME "O3CNVBIT"
#define COLDEF "id int, col boolean"

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

        int id;

        SQLCHAR bit;
        SQLCHAR buffer[512], insert[512], select[512];

        SQLCHAR vChar[10], vBin[10];
        SQLWCHAR vCharw[10];
        SQLINTEGER vInt;
        SQLREAL vFloat;
        SQLDOUBLE vDouble;
        SQL_NUMERIC_STRUCT vNum;
        short vShort;
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

	/* Insert C_CHAR data */
	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
                              SQL_INTEGER, 0, 0, &id, 0, NULL);
	SPRINTF(buffer," SQLBindParameter (id) retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);
	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BIT,
                              SQL_BIT, 0, 0, &bit, 0, NULL);
	SPRINTF(buffer," SQLBindParameter (bit) retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        id = 1;
        bit = TRUE;
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);

        id = 2;
        bit = FALSE;
	rc = SQLExecute(hstmt);
	SPRINTF(buffer," SQLExecute retcode: %d"CRLF, rc);
	PRINTF(buffer);
	ErrorCheck(SQL_HANDLE_STMT, hstmt);



	/* Select date from the table */
	STRCPY(select, "select * from ");
        STRCAT(select, TABNAME);
	SPRINTF(buffer,"Select Stmt= '%s'"CRLF, select);
	PRINTF(buffer);

	okflag = TRUE;

        /* CHAR */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_CHAR, vChar, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_CHAR rc: %d\t %s\t", rc, vChar);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_CHAR, vChar, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_CHAR rc: %d\t %s\n", rc, vChar);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);

        /* WCHAR */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_WCHAR, vCharw, sizeof (vChar), NULL);
        WStringToString (vChar, sizeof (vChar), NULL, TRUE, vCharw, WStrLen (vCharw, sizeof(vCharw)), NULL);
        SPRINTF (buffer, "SQL_C_WCHAR rc: %d\t %s\t", rc, vChar);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_WCHAR, vCharw, sizeof (vChar), NULL);
        WStringToString (vChar, sizeof (vChar), NULL, TRUE, vCharw, WStrLen (vCharw, sizeof(vCharw)), NULL);
        SPRINTF (buffer, "SQL_C_WCHAR rc: %d\t %s\n", rc, vChar);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);


        /* LONG */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_LONG, &vInt, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_LONG rc: %d\t %d\t", rc, vInt);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_LONG, &vInt, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_LONG rc: %d\t %d\n", rc, vInt);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);


        /* BIGINT */
        /*
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_SBIGINT, &vBig, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_BIGINT rc: %d\t %d\t", rc, vBig);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_SBIGINT, &vBig, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_BIGINT rc: %d\t %d\n", rc, vBig);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);
        */


        /* TINYINT */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_TINYINT, &vTiny, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_TINYINT rc: %d\t %d\t", rc, vTiny);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_TINYINT, &vTiny, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_TINYINT rc: %d\t %d\n", rc, vTiny);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);



        /* SHORT */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_SHORT, &vShort, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_SHORT rc: %d\t %d\t", rc, vShort);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_SHORT, &vShort, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_SHORT rc: %d\t %d\n", rc, vShort);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);


        /* FLOAT */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_FLOAT, &vFloat, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_FLOAT rc: %d\t %g\t", rc, vFloat);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_FLOAT, &vFloat, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_FLOAT rc: %d\t %g\n", rc, vFloat);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);


        /* DOUBLE */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_DOUBLE, &vDouble, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_DOUBLE rc: %d\t %g\t", rc, vDouble);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_DOUBLE, &vDouble, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_DOUBLE rc: %d\t %g\n", rc, vDouble);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);


        /* NUMERIC */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_NUMERIC, &vNum, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_NUMERIC rc: %d\t prec: %d  scale: %d  sign: %d  val: %d\n",
                 rc, vNum.precision, vNum.scale, vNum.sign, vNum.val[0]|vNum.val[1]);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_NUMERIC, &vNum, sizeof (vChar), NULL);
        SPRINTF (buffer, "SQL_C_NUMERIC rc: %d\t prec: %d  scale: %d  sign: %d  val: %d\n",
                 rc, vNum.precision, vNum.scale, vNum.sign, vNum.val[0]|vNum.val[1]);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);


        /* BINARY */
        rc = SQLExecDirect (hstmt, select, SQL_NTS);
        SPRINTF(buffer," SQLExecDirect retcode: %d"CRLF, rc);
        PRINTF(buffer);
        ErrorCheck(SQL_HANDLE_STMT, hstmt);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_BINARY, vBin, sizeof (vBin), NULL);
        SPRINTF (buffer, "SQL_C_BINARY rc: %d\t %d\t", rc, vBin[0]);
        PRINTF(buffer);

        rc = SQLFetch(hstmt);
        rc = SQLGetData(hstmt, 2, SQL_C_BINARY, vBin, sizeof (vBin), NULL);
        SPRINTF (buffer, "SQL_C_BINARY rc: %d\t %d\n", rc, vBin[0]);
        PRINTF(buffer);

        rc = SQLFreeStmt (hstmt, SQL_CLOSE);



	if (okflag == TRUE)
		PRINTF("Test state: OK\n\n");
	else
		PRINTF("Test state: Failure\n\n");
	FullDisconnect(phenv, phdbc);
	return 0;
}
