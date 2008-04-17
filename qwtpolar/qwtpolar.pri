######################################################################
# Install paths
######################################################################

VERSION      = 0.0.0

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

######################################################################
# release/debug mode
# The designer plugin is always built in release mode.
# If want to change this, you have to edit designer/designer.pro.
######################################################################

CONFIG           += debug     # release/debug

######################################################################
# Qwt paths
# Add the paths to the Qwt include files and Qwt libraries
######################################################################

QWT_INCLUDEPATH = .../qwt-5.1/include
QWT_LIBRARYPATH = .../qwt-5.1/lib

######################################################################
# Build the static/shared libraries.
# If QwtPolarDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

CONFIG           += QwtPolarDll

######################################################################
# If you want to auto build the examples, enable the line below
# Otherwise you have to build them from the examples directory.
######################################################################

CONFIG     += QwtPolarExamples
