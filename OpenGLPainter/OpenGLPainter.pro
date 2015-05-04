QT += opengl
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += OPENGLPAINTER_LIBRARY
SOURCES += openglpainter.cpp
HEADERS += openglpainter.h\
        OpenGLPainter_global.h

INCLUDEPATH += $$ROOT_SRCDIR/ColorScale
DEPENDPATH += $$ROOT_SRCDIR/ColorScale
LIBS += -L$$ROOT_BUILDDIR/app/libs -lColorScale
