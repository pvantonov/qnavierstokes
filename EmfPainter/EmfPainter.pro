QT += widgets
LIBS += -lgdi32
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += EMFPAINTER_LIBRARY
SOURCES += emfpainter.cpp
HEADERS += emfpainter.h\
        EmfPainter_global.h
