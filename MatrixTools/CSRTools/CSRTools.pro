QT -= gui
TEMPLATE = lib
DEFINES += CSRTOOLS_LIBRARY
SOURCES += csrtools.cpp
HEADERS += csrtools.h\
        CSRTools_global.h

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../../GUI/debug/
