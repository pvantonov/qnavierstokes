QT -= gui
TEMPLATE = lib
DEFINES += BOTTOMHEATING_LIBRARY
SOURCES += bottomheatingsolver.cpp
HEADERS += bottomheatingsolver.h\
        BottomHeating_global.h

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../GUI/debug/
