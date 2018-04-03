QT += qml quick quickcontrols2

CONFIG += c++11

SOURCES += main.cpp \
    board.cpp \
    solver.cpp \
    solvestate.cpp \
    orb.cpp \
    combothread.cpp \
    padgui.cpp \
    solvethread.cpp \
    combo.cpp

RESOURCES += \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    board.h \
    solver.h \
    solvestate.h \
    orb.h \
    combothread.h \
    padgui.h \
    settings.h \
    solvethread.h \
    combo.h
