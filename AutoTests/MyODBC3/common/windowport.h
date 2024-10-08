/*
 * Macros to attempt some form of portability
 */

#define PASCAL

#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define FALSE				0
#endif

#define HGLOBAL				void*
#define GPTR				0

#define GlobalAlloc(a,b)	malloc(b)
#define GlobalLock(a)		(a)
#define GlobalUnlock(a)		(a)
#define GlobalFree(a)		free(a)

#define lstrlen				strlen
#define lstrcpy				strcpy
#define lstrcat				strcat
#define _ltoa(i,str,base)	sprintf( str, "%d", i );
#define _timeb				timeb
#define _ftime 				ftime

#include <sys/timeb.h>
