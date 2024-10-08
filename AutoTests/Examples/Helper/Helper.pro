include(../../common.pri)
include(../../config.pri)
include(../../defines.pri)
include(../../odbc.pri)

TEMPLATE= lib
TARGET  = Helper
DESTDIR	= ./
CONFIG	+= staticlib

HEADERS = \
        helper.h

SOURCES = \
	helper.c

