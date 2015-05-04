QT -= gui
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += LOS_LIBRARY
SOURCES += los.cpp
HEADERS += los.h\
        LOS_global.h

INCLUDEPATH += $$ROOT_SRCDIR/MatrixTools/CSRTools \
    $$ROOT_SRCDIR/Gauss
DEPENDPATH += $$ROOT_SRCDIR/MatrixTools/CSRTools \
    $$ROOT_SRCDIR/Gauss
LIBS += -L$$ROOT_BUILDDIR/app/libs -lCSRTools \
    -L$$ROOT_BUILDDIR/app/libs -lGauss
