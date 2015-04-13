QT += widgets
LIBS += -lgdi32
TEMPLATE = lib
DEFINES += EMFPAINTER_LIBRARY
SOURCES += emfpainter.cpp
HEADERS += emfpainter.h\
        EmfPainter_global.h

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../GUI/debug/
