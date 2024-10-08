include(../../common.pri)
include(../../config.pri)
include(../../defines.pri)
include(../../odbc.pri)

win32 {
        LIBS    += msvcrt.lib
}

TEMPLATE= lib
TARGET  = QuickTest
DESTDIR	= ../lib
CONFIG	+= plugin

RESOURCES       = 

HEADERS = \
	quiktest.h

SOURCES = \
	quiktest.c

