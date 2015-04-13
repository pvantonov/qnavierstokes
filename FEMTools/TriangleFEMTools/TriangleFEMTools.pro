QT -= gui
TEMPLATE = lib
DEFINES += TRIANGLEFEMTOOLS_LIBRARY
SOURCES += trianglefemtools.cpp
HEADERS += trianglefemtools.h\
        TriangleFEMTools_global.h

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../../GUI/debug/
