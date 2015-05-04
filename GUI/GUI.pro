QT += widgets webkit webkitwidgets opengl
TEMPLATE = app
VERSION = 1.5.0
TARGET = qnavierstokes
DESTDIR = $$ROOT_BUILDDIR/app

SOURCES += main.cpp \
    mainwindow.cpp \
    settingswindow.cpp \
    aboutwindow.cpp \
    helpwindow.cpp
HEADERS += mainwindow.h \
    settings.h \
    settingswindow.h \
    paintaim.h \
    aboutwindow.h \
    helpwindow.h
FORMS += mainwindow.ui \
    settingswindow.ui \
    aboutwindow.ui

INCLUDEPATH += $$ROOT_SRCDIR/BottomHeating \
    $$ROOT_SRCDIR/SideHeating \
    $$ROOT_SRCDIR/OpenGLPainter \
    $$ROOT_SRCDIR/EmfPainter
DEPENDPATH += $$ROOT_SRCDIR/BottomHeating \
    $$ROOT_SRCDIR/SideHeating \
    $$ROOT_SRCDIR/OpenGLPainter \
    $$ROOT_SRCDIR/EmfPainter
LIBS += -L$$ROOT_BUILDDIR/app/libs -lBottomHeating \
    -L$$ROOT_BUILDDIR/app/libs -lSideHeating \
    -L$$ROOT_BUILDDIR/app/libs -lOpenGLPainter \
    -L$$ROOT_BUILDDIR/app/libs -lEmfPainter
