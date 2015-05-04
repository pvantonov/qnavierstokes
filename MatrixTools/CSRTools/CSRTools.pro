QT -= gui
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += CSRTOOLS_LIBRARY
SOURCES += csrtools.cpp
HEADERS += csrtools.h\
        CSRTools_global.h
