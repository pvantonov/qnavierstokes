QT -= gui
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += TRIANGLEFEMTOOLS_LIBRARY
SOURCES += trianglefemtools.cpp
HEADERS += trianglefemtools.h\
        TriangleFEMTools_global.h
