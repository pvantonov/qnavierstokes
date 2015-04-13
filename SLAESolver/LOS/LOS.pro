QT -= gui
TEMPLATE = lib
DEFINES += LOS_LIBRARY
SOURCES += los.cpp
HEADERS += los.h\
        LOS_global.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../MatrixTools/CSRTools/release/ -lCSRTools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../MatrixTools/CSRTools/debug/ -lCSRTools

INCLUDEPATH += $$PWD/../../MatrixTools/CSRTools
DEPENDPATH += $$PWD/../../MatrixTools/CSRTools

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Gauss/release/ -lGauss
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Gauss/debug/ -lGauss

INCLUDEPATH += $$PWD/../Gauss
DEPENDPATH += $$PWD/../Gauss

win32:CONFIG(release, debug|release): DESTDIR += $$OUT_PWD/../../GUI/release/
else:win32:CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../../GUI/debug/
