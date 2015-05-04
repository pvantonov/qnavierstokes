QT -= gui
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += BOTTOMHEATING_LIBRARY
SOURCES += bottomheatingsolver.cpp
HEADERS += bottomheatingsolver.h\
        BottomHeating_global.h
