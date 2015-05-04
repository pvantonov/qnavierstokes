QT -= gui
TEMPLATE = lib
DESTDIR = $$ROOT_BUILDDIR/app/libs
DLLDESTDIR = $$ROOT_BUILDDIR/app
DEFINES += SIDEHEATING_LIBRARY
SOURCES += sideheatingsolver.cpp
HEADERS += sideheatingsolver.h\
        SideHeating_global.h

INCLUDEPATH += $$ROOT_SRCDIR/SLAESolver/LOS \
    $$ROOT_SRCDIR/MatrixTools/CSRTools \
    $$ROOT_SRCDIR/FEMTools/TriangleFEMTools
DEPENDPATH += $$ROOT_SRCDIR/SLAESolver/LOS \
    $$ROOT_SRCDIR/MatrixTools/CSRTools \
    $$ROOT_SRCDIR/FEMTools/TriangleFEMTools
LIBS += -L$$ROOT_BUILDDIR/app/libs -lLOS \
    -L$$ROOT_BUILDDIR/app/libs -lCSRTools \
    -L$$ROOT_BUILDDIR/app/libs -lTriangleFEMTools
