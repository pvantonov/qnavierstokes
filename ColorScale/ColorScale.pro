TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += COLORSCALE_LIBRARY
SOURCES += colorscale.cpp
HEADERS += colorscale.h\
        ColorScale_global.h
