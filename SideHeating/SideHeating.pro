QT -= gui
TEMPLATE = lib
DEFINES += SIDEHEATING_LIBRARY
SOURCES += sideheatingsolver.cpp
HEADERS += sideheatingsolver.h\
        SideHeating_global.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SLAESolver/LOS/release/ -lLOS
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SLAESolver/LOS/debug/ -lLOS

INCLUDEPATH += $$PWD/../SLAESolver/LOS
DEPENDPATH += $$PWD/../SLAESolver/LOS

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MatrixTools/CSRTools/release/ -lCSRTools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MatrixTools/CSRTools/debug/ -lCSRTools

INCLUDEPATH += $$PWD/../MatrixTools/CSRTools
DEPENDPATH += $$PWD/../MatrixTools/CSRTools

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FEMTools/TriangleFEMTools/release/ -lTriangleFEMTools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FEMTools/TriangleFEMTools/debug/ -lTriangleFEMTools

INCLUDEPATH += $$PWD/../FEMTools/TriangleFEMTools
DEPENDPATH += $$PWD/../FEMTools/TriangleFEMTools
