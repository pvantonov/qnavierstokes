QT -= gui
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += GAUSS_LIBRARY
SOURCES += gauss.cpp
HEADERS += gauss.h\
        Gauss_global.h
