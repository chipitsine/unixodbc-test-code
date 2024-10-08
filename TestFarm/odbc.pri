# #########################################################
#
# \brief	These settings are used when building 
#		unixODBC tests using the qmake utility.
#
#               Edit these settings before using qmake to
#		generate the Makefiles.
#
# \note         The plan is to generate this file using a 
#               configure.bat/configure.exe at some point in 
#               the future.
#
# \sa		*.pri
#               *.pro
#
# #########################################################

# #########################################################
# WIN32
# #########################################################
win32 {
        LIBS            += odbc32.lib 
}

# #########################################################
# MAC/UNIX common
# #########################################################
unix {
	INCLUDEPATH	+= $(UNIXODBC_DIR)/include
	LIBS		+= -L$(UNIXODBC_DIR)/lib
}

# #########################################################
# MAC/UNIX specific
# #########################################################
mac {
        LIBS	        += -liodbc
} else:unix {
	LIBS		+= -lodbc
}



