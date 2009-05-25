# -*- mode: sh -*- ##############################################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
#################################################################

######################################################################
# Install paths
######################################################################

VER_MAJ      = 0
VER_MIN      = 1
VER_PAT      = 0
VERSION      = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

unix {
    INSTALLBASE    = /usr/local/qwtpolar-$$VERSION-svn
}

win32 {
    INSTALLBASE    = C:/QwtPolar-$$VERSION-svn
}

target.path    = $$INSTALLBASE/lib
headers.path   = $$INSTALLBASE/include
doc.path       = $$INSTALLBASE/doc

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt     # Also for Qtopia Core!
CONFIG           += warn_on
CONFIG           += thread
CONFIG           += silent

######################################################################
# release/debug mode
# The designer plugin is always built in release mode.
# If want to change this, you have to edit designer/designer.pro.
######################################################################

VVERSION = $$[QT_VERSION]
isEmpty(VVERSION) {

    # Qt 3
    CONFIG           += release     # release/debug
}
else {
    # Qt 4
    win32 {
        # On Windows you can't mix release and debug libraries.
        # The designer is built in release mode. If you like to use it
        # you need a release version. For your own application development you
        # might need a debug version. 

        # CONFIG           += debug_and_release
        # CONFIG           += build_all
        CONFIG           += release     # release/debug/debug_and_release
    }
    else {
        CONFIG           += release     # release/debug
    }
}

######################################################################
# If you want to have different names for the debug and release
# versions you can add a suffix rules below.
######################################################################

DEBUG_SUFFIX        =
RELEASE_SUFFIX      =

win32 {
    DEBUG_SUFFIX      = d
}

######################################################################
# Qwt paths
# Add the paths to the Qwt include files and Qwt libraries
# Note, that these need to bo absolute path names.
######################################################################

QWT_INCLUDEPATH = /path/to/qwt-5.2/include
QWT_LIBRARYPATH = /path/to/qwt-5.2/lib

!exists ( $${QWT_INCLUDEPATH} ) {
    error( "You need to define a valid path to the Qwt includes in qwtpolar.pri" )
}
!exists ( $${QWT_LIBRARYPATH} ) {
    error( "You need to define a valid path to the Qwt libs in qwtpolar.pri" )
}
win32 {
	QWT_VERSION_MAJ = 5
}

######################################################################
# Build the static/shared libraries.
# If QwtPolarDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

CONFIG           += QwtPolarDll

######################################################################
# If you want to build the QwtPolar designer plugin,
# enable the line below.
# Otherwise you have to build it from the designer directory.
######################################################################

CONFIG     += QwtPolarDesigner

######################################################################
# If you want to auto build the examples, enable the line below
# Otherwise you have to build them from the examples directory.
######################################################################

# CONFIG     += QwtPolarExamples
