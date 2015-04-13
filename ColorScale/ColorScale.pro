TEMPLATE = lib
DEFINES += COLORSCALE_LIBRARY
SOURCES += colorscale.cpp
HEADERS += colorscale.h\
        ColorScale_global.h

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../GUI/debug/
