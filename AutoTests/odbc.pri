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
#               the future (on MS platforms).
#
# \sa		*.pri
#               *.pro
#
# #########################################################

#
# gtrtst
#
# We use a GUI specific version of gtrtst. This could be the legacy
# implementation from MS (gtrtst32) or a modern version derived from 
# unixODBC work. The application used to run the tests must be built
# for the same GUI/toolkit as the chosen gtrtst. 
#
# Example 1 - build AutoTest libraries for use with MS ODBC Test application
#
# - link the AutoTest libraries against gtrtst32.dll
#
# Example 2 - build AutoTest libraries for use with unixODBC-GUI-Qt::ODBCTestQ4 
#
# - link the AutoTest libraries against unixODBC-GUI-Qt::gtrtstQ4
#

#
# You need autotest.h. You can use the MS version or you can 
# use the one already in these sources. 
#
INCLUDEPATH     += ./include

# #########################################################
# WIN32
# #########################################################
win32 {
#
#       We use MS's Driver Manager, Installer library, etc.
#
        LIBS            += odbc32.lib odbccp32.lib
        LIBS            += user32.lib
#
#	We build for unixODBC-GUI-Qt::ODBCTestQ4.
#
        LIBS            += $(UNIXODBC_GUI_QT_SRC)/lib/gtrtstQ4.lib
}

# #########################################################
# MAC/UNIX common
# #########################################################
unix {
}

# #########################################################
# MAC/UNIX specific
# #########################################################
mac {
        LIBS	        += -liodbc -liodbcinst
} else:unix {
#
#       We use unixODBC's Driver Manager, Installer library, etc.
#
	INCLUDEPATH	+= $(UNIXODBC_DIR)/include
	LIBS		+= -L$(UNIXODBC_LIBDIR)
	LIBS		+= -lodbc -lodbcinst
#
#	We build for unixODBC-GUI-Qt::ODBCTestQ4.
#
        LIBS            += -lgtrtstQ4
}

