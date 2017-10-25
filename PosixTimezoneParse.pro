QT += core
QT -= gui

TARGET = PosixTimezoneParse
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    TimeZone/timezoneparse.cpp \
    timezoneparseposix.cpp

HEADERS += \
    TimeZone/timezoneparse.h \
    timezoneparseposix.h

RESOURCES += \
    resource.qrc

