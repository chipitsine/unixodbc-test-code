# #########################################################
# COMMON
# #########################################################
TEMPLATE	= lib
TARGET		= tst
CONFIG		+= staticlib thread warn_on debug
LANGUAGE	= C
OBJECTS_DIR	= .tmp
DESTDIR		= ./
INCLUDEPATH	+= /usr/local/include  ../../common
LIBS		+= 

# #########################################################
# FILES
# #########################################################
HEADERS		=	\
			tst.h \
                        o3util.h \
                        uniutil.h
SOURCES		=	\
			tst.c \
                        o3util.c \
                        o3utdesc.c \
                        uniutil.c
