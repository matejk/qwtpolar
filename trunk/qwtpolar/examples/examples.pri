# -*- mode: sh -*- #############################################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GPL License, Version 2.0
################################################################

QWT_POLAR_ROOT = ../..

include( $${QWT_POLAR_ROOT}/qwtpolar.pri )

TEMPLATE     = app

MOC_DIR      = moc
OBJECTS_DIR  = obj
INCLUDEPATH += $${QWT_POLAR_ROOT}/src
DEPENDPATH  += $${QWT_POLAR_ROOT}/src
INCLUDEPATH += $$QWT_INCLUDEPATH
DEPENDPATH  += $$QWT_INCLUDEPATH

win32 {
	contains(CONFIG, QwtPolarDll) {
    	DEFINES    += QT_DLL QWT_DLL QWT_POLAR_DLL
		QWTLIB = qwt5
		QWTPOLARLIB = qwtpolar$${VER_MAJ}
	}
	else {
		QWTLIB = qwt
		QWTPOLARLIB = qwtpolar
	}

    msvc:LIBS  += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    msvc.net:LIBS  += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    msvc2005:LIBS += $${QWT_POLAR_ROOT}/lib/$${QWTPOLARLIB}.lib
    g++:LIBS   += -L$${QWT_POLAR_ROOT}/lib -lqwtpolar

    msvc:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    msvc.net:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    msvc2005:LIBS  += $${QWT_LIBRARYPATH}/$${QWTLIB}.lib
    g++:LIBS   += -L$${QWT_LIBRARYPATH} -lqwtpolar
}
else {
	LIBS  += -L$${QWT_POLAR_ROOT}/lib -lqwtpolar
	LIBS  += -L$${QWT_LIBRARYPATH} -lqwt 
}
