QT += dbus
INCLUDEPATH += ../wifi
!exists(bluemanager.h):system(qdbusxml2cpp -p bluemanager.h:bluemanager.cpp manager.xml)
!exists(blueadapter.h):system(qdbusxml2cpp -p blueadapter.h:blueadapter.cpp adapter.xml)
!exists(bluedevice.h):system(qdbusxml2cpp -p bluedevice.h:bluedevice.cpp -i devicetypes.h device.xml)
!exists(audiosink.h):system(qdbusxml2cpp -p audiosink.h:audiosink.cpp audiosink.xml)
!exists(audiosource.h):system(qdbusxml2cpp -p audiosource.h:audiosource.cpp audiosrc.xml)

SOURCES += bluetooth/bluetoothpage.cpp \
    bluetooth/nearbydeviceswidget.cpp \
    bluetooth/bluemanager.cpp \
    bluetooth/devicelistwidget.cpp \
    bluetooth/bluedevice.cpp \
    bluetooth/bluetoothwidget.cpp \
    bluetooth/bluetoothtogglewidget.cpp \
    bluetooth/blueadapter.cpp \
    bluetooth/bluetoothagent.cpp \
    bluetooth/agentadaptor.cpp \
    bluetooth/bluetoothvisibilitybutton.cpp \
    bluetooth/profileactionpage.cpp \
    bluetooth/discoverwidget.cpp \
    bluetooth/audiosink.cpp \
    bluetooth/audiosource.cpp
HEADERS += bluetooth/bluetoothpage.h \
    bluetooth/nearbydeviceswidget.h \
    bluetooth/bluemanager.h \
    bluetooth/devicelistwidget.h \
    bluetooth/bluedevice.h \
    bluetooth/bluetoothwidget.h \
    bluetooth/bluetoothtogglewidget.h \
    bluetooth/blueadapter.h \
    bluetooth/bluetoothagent.h \
    bluetooth/agentadaptor.h \
    bluetooth/bluetoothvisibilitybutton.h \
    bluetooth/devicetypes.h \
    bluetooth/profileactionpage.h \
    bluetooth/discoverwidget.h \
    bluetooth/audiosink.h \
    bluetooth/audiosource.h
OTHER_FILES += McpBluetooth.conf
dbusconfig.files = bluetooth/McpBluetooth.conf
dbusconfig.path = $$INSTALL_ROOT/etc/dbus-1/system.d/
INSTALLS += dbusconfig
