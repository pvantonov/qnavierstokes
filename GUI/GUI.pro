QT += xml webkit opengl
TEMPLATE = app
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BottomHeating/release/ -lBottomHeating
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BottomHeating/debug/ -lBottomHeating

INCLUDEPATH += $$PWD/../BottomHeating
DEPENDPATH += $$PWD/../BottomHeating

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SideHeating/release/ -lSideHeating
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SideHeating/debug/ -lSideHeating

INCLUDEPATH += $$PWD/../SideHeating
DEPENDPATH += $$PWD/../SideHeating

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../OpenGLPainter/release/ -lOpenGLPainter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../OpenGLPainter/debug/ -lOpenGLPainter

INCLUDEPATH += $$PWD/../OpenGLPainter
DEPENDPATH += $$PWD/../OpenGLPainter

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EmfPainter/release/ -lEmfPainter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EmfPainter/debug/ -lEmfPainter

INCLUDEPATH += $$PWD/../EmfPainter
DEPENDPATH += $$PWD/../EmfPainter
