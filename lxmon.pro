QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
TARGET = lxmon
target.path = /home/pi/lxmon
INSTALLS += target

LIBS += -L$$PWD/../../rpi-qt/sysroot/usr/lib/arm-linux-gnueabihf/ -lpthread

INCLUDEPATH += $$PWD/../../rpi-qt/sysroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../rpi-qt/sysroot/usr/lib/arm-linux-gnueabihf

HEADERS += \
    logger.h

DISTFILES += \
    LICENSE \
    README.md
