include(../../common.pri)
include(../../config.pri)
include(../../defines.pri)
include(../../odbc.pri)

TEMPLATE= lib
TARGET  = BoundParameters
DESTDIR	= ../lib
CONFIG	+= plugin

INCLUDEPATH     += ../Helper
LIBS	        += -L../Helper -lHelper

HEADERS =

SOURCES = \
	boundparam.c \
	cboundtimestampparam.c

