# -*- mode: sh -*- ###########################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GPL License, Version 2.0
##############################################

include( ../../qwtpolar.pri )

TEMPLATE     = app

MOC_DIR      = moc
OBJECTS_DIR  = obj
INCLUDEPATH += ../../src
DEPENDPATH  += ../../src
INCLUDEPATH += $$QWT_INCLUDEPATH
DEPENDPATH  += $$QWT_INCLUDEPATH


unix:LIBS  += -L../../lib -lqwtpolar
unix:LIBS  += -L$$QWT_LIBRARYPATH -lqwt 

win32:QwtPolarDll {
    DEFINES    += QT_DLL QWT_DLL QWT_POLAR_DLL
}

contains(CONFIG, QwtPolarDll) {
    win32-msvc:LIBS  += ../../lib/qwtpolar0.lib
    win32-msvc:LIBS  += $$QWT_LIBRARYPATH/qwt5.lib
    win32-msvc.net:LIBS  += ../../lib/qwtpolar0.lib
    win32-msvc.net:LIBS  += $$QWT_LIBRARYPATH/qwt5.lib
    win32-msvc2005:LIBS += ../../lib/qwtpolar0.lib
    win32-msvc2005:LIBS += $$QWT_LIBRARYPATH/qwt5.lib
    win32-g++:LIBS   += -L$$QWT_LIBRARYPATH -lqwtpolar
    win32-g++:LIBS   += -L$$QWT_LIBRARYPATH -lqwt
} else {
    win32-msvc:LIBS  += ../../lib/qwtpolar.lib
    win32-msvc:LIBS  += $$QWT_LIBRARYPATH/lib/qwt.lib
    win32-msvc.net:LIBS  += ../../lib/qwtpolar.lib
    win32-msvc.net:LIBS  += $$QWT_LIBRARYPATH/lib/qwt.lib
    win32-msvc2005:LIBS += ../../lib/qwtpolar.lib
    win32-msvc2005:LIBS += $$QWT_LIBRARYPATH/lib/qwt.lib
    win32-g++:LIBS   += -L../../lib -lqwtpolar
    win32-g++:LIBS   += -L$$QWT_LIBRARYPATH/lib -lqwt
}

