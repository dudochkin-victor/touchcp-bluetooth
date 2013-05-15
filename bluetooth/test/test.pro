QT += dbus
CONFIG += meegotouch \
    debug
INCLUDEPATH += ../src
LIBS += -L../src \
    -lmeegobluetooth
SOURCES += main.cpp \
    bluetoothtransferpage.cpp
HEADERS += bluetoothtransferpage.h
