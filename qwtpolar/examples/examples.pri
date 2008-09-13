# -*- mode: sh -*- ##############################################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
#################################################################

QWT_POLAR_ROOT = ../..

include( $${QWT_POLAR_ROOT}/qwtpolar.pri )

SUFFIX_STR =
VVERSION = $$[QT_VERSION]
isEmpty(VVERSION) {

    # Qt 3
    debug {
        SUFFIX_STR = $${DEBUG_SUFFIX}
    }
    else {
        SUFFIX_STR = $${RELEASE_SUFFIX}
    }
}
else {
    CONFIG(debug, debug|release) {
        SUFFIX_STR = $${DEBUG_SUFFIX}
    }
    else {
        SUFFIX_STR = $${RELEASE_SUFFIX}
    }
}

TEMPLATE     = app

MOC_DIR      = moc
OBJECTS_DIR  = obj$${SUFFIX_STR}
INCLUDEPATH += $${QWT_POLAR_ROOT}/src
DEPENDPATH  += $${QWT_POLAR_ROOT}/src
INCLUDEPATH += $$QWT_INCLUDEPATH
DEPENDPATH  += $$QWT_INCLUDEPATH
DESTDIR      = $${QWT_POLAR_ROOT}/examples/bin$${SUFFIX_STR}

QWTLIB = qwt$${SUFFIX_STR}
QWTPOLARLIB = qwtpolar$${SUFFIX_STR}
win32 {
	contains(CONFIG, QwtPolarDll) {
    	DEFINES    += QT_DLL QWT_DLL QWT_POLAR_DLL
		QWTLIB = $${QWTLIB}5
		QWTPOLARLIB = $${QWTPOLARLIB}$${VER_MAJ}
	}

    win32-msvc:LIBS  += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    win32-msvc.net:LIBS  += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    win32-msvc2002:LIBS += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    win32-msvc2003:LIBS += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    win32-msvc2005:LIBS += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    win32-msvc2008:LIBS += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    win32-g++:LIBS   += -L$${QWT_POLAR_ROOT}/lib -l$${QWTPOLARLIB}

    win32-msvc:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc.net:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2002:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2003:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2005:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2008:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-g++:LIBS   += -L$${QWT_LIBRARYPATH} -l$${QWTLIB}
}
else {
	LIBS  += -L$${QWT_POLAR_ROOT}/lib -l$${QWTPOLARLIB}
	LIBS  += -L$${QWT_LIBRARYPATH} -l$${QWTLIB} 
}
