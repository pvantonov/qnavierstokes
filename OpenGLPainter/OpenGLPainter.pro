QT       += opengl
TEMPLATE = lib
DEFINES += OPENGLPAINTER_LIBRARY
SOURCES += openglpainter.cpp
HEADERS += openglpainter.h\
        OpenGLPainter_global.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ColorScale/release/ -lColorScale
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ColorScale/debug/ -lColorScale

INCLUDEPATH += $$PWD/../ColorScale
DEPENDPATH += $$PWD/../ColorScale
