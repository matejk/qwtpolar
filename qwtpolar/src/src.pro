# -*- mode: sh -*- ##############################################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
#################################################################

QWT_POLAR_ROOT = ..

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

TARGET            = qwtpolar$${SUFFIX_STR}
TEMPLATE          = lib

MOC_DIR           = moc
OBJECTS_DIR       = obj$${SUFFIX_STR}
DESTDIR           = $${QWT_POLAR_ROOT}/lib

contains(CONFIG, QwtPolarDll ) {
    CONFIG += dll
}
else {
    CONFIG += staticlib
}

INCLUDEPATH += $$QWT_INCLUDEPATH
DEPENDPATH  += $$QWT_INCLUDEPATH

HEADERS += \
    qwt_polar_global.h \
    qwt_polar.h \
    qwt_polar_point.h \
    qwt_polar_item.h \
    qwt_polar_panner.h \
    qwt_polar_magnifier.h \
    qwt_polar_grid.h \
    qwt_polar_curve.h \
    qwt_polar_marker.h \
    qwt_polar_itemdict.h \
    qwt_polar_canvas.h \
    qwt_polar_layout.h \
    qwt_polar_plot.h

SOURCES += \
    qwt_polar_point.cpp \
    qwt_polar_item.cpp \
    qwt_polar_panner.cpp \
    qwt_polar_magnifier.cpp \
    qwt_polar_grid.cpp \
    qwt_polar_curve.cpp \
    qwt_polar_marker.cpp \
    qwt_polar_itemdict.cpp \
    qwt_polar_canvas.cpp \
    qwt_polar_layout.cpp \
    qwt_polar_plot.cpp

# Install directives

headers.files  = $$HEADERS
doc.files      = $${QWT_POLAR_ROOT}/doc/html
unix {
    doc.files      += $${QWT_POLAR_ROOT}/doc/man
}

INSTALLS       = target headers doc

QWTLIB     = qwt$${SUFFIX_STR}
win32 {
    contains(CONFIG, QwtPolarDll) {
    	DEFINES    += QT_DLL QWT_DLL QWT_POLAR_DLL QWT_POLAR_MAKEDLL
		QWTLIB     = $${QWTLIB}$${QWT_VERSION_MAJ}
	}
	else {
	}

    win32-msvc:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc.net:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2002:LIBS += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2003:LIBS += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2005:LIBS += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-msvc2008:LIBS += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    win32-g++:LIBS   += -L$${QWT_LIBRARYPATH} -l$${QWTLIB}
}
else {
	LIBS   += -L$${QWT_LIBRARYPATH} -l$${QWTLIB}
}
