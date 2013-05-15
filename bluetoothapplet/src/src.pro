load(meegotouch_defines)
include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)

INCLUDEPATH += ../../bluetooth/src
LIBS += -L../../bluetooth/src -lmeegobluetooth
TEMPLATE = lib
CONFIG += plugin \
    gui \
    meegotouch \
    silent \
    debug \
    duicontrolpanel
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = bluetoothapplet.h \
    bluetoothbrief.h
SOURCES = bluetoothapplet.cpp \
    bluetoothbrief.cpp \

css.files = bluetoothapplet.css
DESTDIR = ../lib
TARGET = $$qtLibraryTarget(bluetoothapplet)
desktop.files += *.desktop
desktop.path = $$DCP_DESKTOP_DIR
target.path += $$DCP_APPLET_DIR
css.path += $$DCP_THEME_DIR/style
dbusconfig.files = McpBluetooth.conf
dbusconfig.path = $$INSTALL_ROOT/etc/dbus-1/system.d/

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)

INSTALLS += target \
    css \
    desktop \
    dbusconfig
