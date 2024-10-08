# #########################################################
# QMAKE PROJECT FILE
# #########################################################
TEMPLATE	= app
TARGET          = test

include( ../../../../../common.pri )
include( ../../../../../config.pri )
include( ../../../../defines.pri )
include( ../../../../odbc.pri )

DEFINES		+= TEST_NAME="\"AllocEnv-ENV\""
INCLUDEPATH     += ../../../common ../../../../common
LIBS		+= -L../../../common -ltst 

# #########################################################
# FILES
# #########################################################
HEADERS		=  
SOURCES		= test.c


