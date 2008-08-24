# -*- mode: sh -*- ###########################
# QwtPolar Widget Library
# Copyright (C) 2008   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
##############################################

QWTPOLAR_ROOT = ..

include ( $${QWTPOLAR_ROOT}/qwtpolar.pri )

contains(CONFIG, QwtPolarDesigner) {

	CONFIG    += warn_on

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

	TEMPLATE        = lib
	MOC_DIR         = moc
	OBJECTS_DIR     = obj$${SUFFIX_STR}
	DESTDIR         = plugins/designer
	INCLUDEPATH    += $${QWTPOLAR_ROOT}/src 
	DEPENDPATH     += $${QWTPOLAR_ROOT}/src 

	INCLUDEPATH += $$QWT_INCLUDEPATH
	DEPENDPATH  += $$QWT_INCLUDEPATH

    LIBNAME         = qwtpolar$${SUFFIX_STR}
	contains(CONFIG, QwtDll) {
		win32 {
			DEFINES += QT_DLL QWT_DLL QWTPOLAR_DLL
			LIBNAME = $${LIBNAME}$${VER_MAJ}
		}
	}

	unix:LIBS      += -L$${QWTPOLAR_ROOT}/lib -l$${LIBNAME}
	win32-msvc:LIBS  += $${QWTPOLAR_ROOT}/lib/$${LIBNAME}.lib
	win32-msvc.net:LIBS  += $${QWTPOLAR_ROOT}/lib/$${LIBNAME}.lib
	win32-msvc2005:LIBS += $${QWTPOLAR_ROOT}/lib/$${LIBNAME}.lib
	win32-g++:LIBS   += -L$${QWTPOLAR_ROOT}/lib -l$${LIBNAME}

	# isEmpty(QT_VERSION) does not work with Qt-4.1.0/MinGW

	VVERSION = $$[QT_VERSION]
	isEmpty(VVERSION) {
		# Qt 3 
		TARGET    = qwtpolarplugin$${SUFFIX_STR}
		CONFIG   += qt plugin

		UI_DIR    = ui

		HEADERS  += qwt_polar_plugin.h
		SOURCES  += qwt_polar_plugin.cpp

		target.path = $(QTDIR)/plugins/designer
		INSTALLS += target

		IMAGES  += \
			pixmaps/qwt_polar_plot.png

	} else {

		# Qt 4

		TARGET    = qwt_polar_designer_plugin$${SUFFIX_STR}
		CONFIG    += qt designer plugin 

		RCC_DIR   = resources

		HEADERS += \
			qwt_polar_designer_plugin.h

		SOURCES += \
			qwt_polar_designer_plugin.cpp

		RESOURCES += \
			qwt_polar_designer_plugin.qrc

		target.path = $$[QT_INSTALL_PLUGINS]/designer
		INSTALLS += target
	}
}
else {
	TEMPLATE        = subdirs # do nothing
}
