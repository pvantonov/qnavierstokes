QT -= gui
TEMPLATE = lib
DEFINES += GAUSS_LIBRARY
SOURCES += gauss.cpp
HEADERS += gauss.h\
        Gauss_global.h

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../../GUI/debug/
