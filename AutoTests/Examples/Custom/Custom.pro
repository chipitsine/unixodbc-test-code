include(../../common.pri)
include(../../config.pri)
include(../../defines.pri)
include(../../odbc.pri)

TEMPLATE= lib
TARGET  = Custom
DESTDIR	= ../lib
CONFIG	+= plugin

HEADERS = \
	custom.h

SOURCES = \
	custom.c


