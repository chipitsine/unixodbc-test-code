#
# building
# ========
#
# These tests can be built for UNIX'ism or MS'ism.
#
# UNIX'ism
# --------
#
# Install unixODBC-Core and unixODBC-Dev. Set the following
# environment variables to tell us where unixODBC was
# installed;
#
# export UNIXODBC_DIR=/usr
# export UNIXODBC_LIBDIR=$UNIXODBC_DIR/lib64
#
# Build This
# 
# $ qmake
# $ make
#
# MS'ism
# ------
#
# Install unixODBC-GUI-Qt sources. Build gtrtstQ4 and
# ODBCTestQ4.
# 
# Set the following environment variable to tell us
# where gtrtstQ4 can be found;
#
# SET UNIXODBC_GUI_QT_SRC=C:/unixodbc-gui-qt/trunk
#
# Build This
# 
# > qmake
# > nmake
#
TEMPLATE        = subdirs
SUBDIRS		= \
		Examples

