# -*- mode: sh -*- ##############################################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
#################################################################

# qmake project file for building the qwtpolar libraries

QWT_POLAR_ROOT = ..

include( $${QWT_POLAR_ROOT}/qwtpolarconfig.pri )
include( $${QWT_POLAR_ROOT}/qwtpolarbuild.pri )

TEMPLATE          = lib
TARGET            = $$qtLibraryTarget(qwtpolar)

contains(CONFIG, QwtPolarDll ) {

    CONFIG += dll
	win32|symbian: DEFINES += QT_DLL QWT_DLL QWT_POLAR_DLL QWT_POLAR_MAKEDLL
}
else {
    CONFIG += staticlib
}

HEADERS += \
    qwt_polar_global.h \
    qwt_polar.h \
    qwt_polar_point.h \
    qwt_polar_fitter.h \
    qwt_polar_item.h \
    qwt_polar_panner.h \
    qwt_polar_magnifier.h \
    qwt_polar_grid.h \
    qwt_polar_curve.h \
    qwt_polar_spectrogram.h \
    qwt_polar_marker.h \
    qwt_polar_itemdict.h \
    qwt_polar_canvas.h \
    qwt_polar_layout.h \
    qwt_polar_plot.h

SOURCES += \
    qwt_polar_point.cpp \
    qwt_polar_fitter.cpp \
    qwt_polar_item.cpp \
    qwt_polar_panner.cpp \
    qwt_polar_magnifier.cpp \
    qwt_polar_grid.cpp \
    qwt_polar_curve.cpp \
    qwt_polar_spectrogram.cpp \
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

target.path    = $${QWT_POLAR_INSTALL_LIBS}
headers.path   = $${QWT_POLAR_INSTALL_HEADERS}
doc.path       = $${QWT_POLAR_INSTALL_DOCS}

INSTALLS       = target headers doc
