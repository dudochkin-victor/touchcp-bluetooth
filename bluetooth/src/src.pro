!exists(bluemanager.h):system(qdbusxml2cpp -p bluemanager.h:bluemanager.cpp manager.xml)
!exists(blueadapter.h):system(qdbusxml2cpp -p blueadapter.h:blueadapter.cpp adapter.xml)
!exists(bluedevice.h):system(qdbusxml2cpp -p bluedevice.h:bluedevice.cpp -i devicetypes.h device.xml)
!exists(audiosink.h):system(qdbusxml2cpp -p audiosink.h:audiosink.cpp audiosink.xml)
!exists(audiosource.h):system(qdbusxml2cpp -p audiosource.h:audiosource.cpp audiosrc.xml)
!exists(headset.h):system(qdbusxml2cpp -p headset.h:headset.cpp headset.xml)
include(obex/obex.pri)
TEMPLATE = lib
TARGET = meegobluetooth
LIBS += -lconnman-qt
QT += dbus \
    xmlpatterns \
    xml
CONFIG += meegotouch \
    link_pkgconfig \
    debug
PKGCONFIG += libpulse libpulse-mainloop-glib
OTHER_FILES += McpBluetooth.conf \
    manager.xml \
    device.xml \
    audiosrc.xml \
    audiosink.xml \
    adapter.xml \
    headset.xml
HEADERS += profileactionpage.h \
    nearbydeviceswidget.h \
    discoverwidget.h \
    devicetypes.h \
    devicelistwidget.h \
    bluetoothwidget.h \
    bluetoothvisibilitybutton.h \
    bluetoothtogglewidget.h \
    bluetoothpage.h \
    bluetoothagent.h \
    agentadaptor.h \
    bluemanager.h \
    bluedevice.h \
    blueadapter.h \
    audiosource.h \
    audiosink.h \
    headset.h \
    nearbyitem.h \
    paireditem.h \
    ftptransferwidget.h \
    btprofiles.h \
    pacontrol.h
SOURCES += profileactionpage.cpp \
    nearbydeviceswidget.cpp \
    discoverwidget.cpp \
    devicelistwidget.cpp \
    bluetoothwidget.cpp \
    bluetoothvisibilitybutton.cpp \
    bluetoothtogglewidget.cpp \
    bluetoothpage.cpp \
    bluetoothagent.cpp \
    agentadaptor.cpp \
    bluemanager.cpp \
    bluedevice.cpp \
    blueadapter.cpp \
    audiosource.cpp \
    audiosink.cpp \
    headset.cpp \
    nearbyitem.cpp \
    paireditem.cpp \
    ftptransferwidget.cpp \
    pacontrol.cpp

target.path = $$INSTALL_ROOT/usr/lib
headers.path = $$INSTALL_ROOT/usr/include/meegobluetooth/
headers.files = $$HEADERS
INSTALLS += target \
    headers

CONFIG += create_pc create_prl
QMAKE_PKGCONFIG_DESCRIPTION = MeeGo Touch Bluetooth library
QMAKE_PKGCONFIG_INCDIR = $$headers.path
pkgconfig.path = $$INSTALL_ROOT/usr/lib/pkgconfig
pkgconfig.files = meegobluetooth.pc

INSTALLS += pkgconfig
