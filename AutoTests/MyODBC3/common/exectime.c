/*
*****************************************************************************
**
** Autor        : Harun Eren
**
** Program      : EXECTIME.C
**
** Usage        :
** Option       : --
**
** Purpose      : Auto Test DLL, Zeitmessung und Zeitcheck
**
** Call Example :
**                struct _timeb timebuf1, timebuf2;
**
**                GetTime(&timebuf1);       -> Get start time struct
**                ...
**                GetTime(&timebuf2);       -> Get finish time struct
**                -> Output time
**                CheckTime(timebuf1, timebuf2, "SQLGetInfo", lpSrvr, 0);
**
****************************************************************************
*/

#ifndef WIN32
#include "windowport.h"
#endif
#include "autotest.h"

/* ------------------------------------------------------------------------- */

#define MAX_NAME_LEN 50

/* ---------------------------------------------------------------------har- */
RETCODE GetTime(struct _timeb *timebuf);
long CheckTime (struct _timeb timebuf1, struct _timeb timebuf2,
			LPSTR szFuncName, lpSERVERINFO lpSrvr, SWORD opt);
/* ---------------------------------------------------------------------har- */

//*---------------------------------------------------------------------------------
//| GetTime():
//|
//| Returns:
//|         Time in _timeb struct
//*---------------------------------------------------------------------------------
RETCODE GetTime(timebuf)
 struct _timeb *timebuf;
{
 _ftime(timebuf);

 return(0);
}
//*---------------------------------------------------------------------------------
//| CheckTime():
//|
//| Option     :
//|		0 -> output short time
//|		1 -> output full time
//|		2 -> return msec (millisecond)
//| 		3 -> return dsec (second)
//| Returns:
//|         Output execution time in sec
//*---------------------------------------------------------------------------------
long CheckTime (timebuf1, timebuf2, szFuncName, lpSrvr, opt)
	struct _timeb timebuf1;
	struct _timeb timebuf2;
	LPSTR  szFuncName;
	lpSERVERINFO lpSrvr;
	SWORD opt;
{
 time_t dsec;
 struct tm *today;
 unsigned short msec;
 char exetd[5];
 char exetm[5];
 UCHAR buffer[MAX_NAME_LEN];

 dsec = timebuf2.time - timebuf1.time;
 msec = (1000 - timebuf1.millitm) + timebuf2.millitm;
 if (msec >= 1000) { msec -= 1000; }
 if (timebuf1.millitm > timebuf2.millitm) { dsec -= 1; }

 szLogPrintf(lpSrvr, FALSE,"\t'%s  ->'\r\n",szFuncName);

 if (opt == 1) { /* output full time */
	 today = localtime(& (timebuf1.time) );
	 sprintf(buffer,"\t\t Time started  %.8s.%hu\r\n",
			asctime(today)+11,timebuf1.millitm / 10);
	 szLogPrintf(lpSrvr, FALSE,"%s", buffer);

	 today = localtime(& (timebuf2.time) );
	 sprintf(buffer,"\t\t Time finished %.8s.%hu\r\n",
			asctime(today)+11,timebuf2.millitm / 10);
	 szLogPrintf(lpSrvr, FALSE,"%s", buffer);
	}

 if (dsec < 10) sprintf(exetd,"0%ld",dsec);
	   else sprintf(exetd,"%ld",dsec);
 if ((msec / 10) < 10) sprintf(exetm,"0%hu",msec / 10);
		  else sprintf(exetm,"%hu",msec / 10);

 if ((opt == 0) || (opt == 1)) { /* output short time */
	 sprintf(buffer,"\t\t Total execution time is '%s.%s' sec.\r\n",exetd,exetm);
	 szLogPrintf(lpSrvr, FALSE,"%s", buffer);
	}

 if (opt == 2) return(dsec);
 if (opt == 3) return(msec);
 return 0;
}
/* ************************************************************************* */


