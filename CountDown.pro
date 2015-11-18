#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T12:37:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CountDown
TEMPLATE = app


SOURCES += main.cpp \
    widget.cpp \
    settingdialog.cpp

HEADERS += widget.h \
    settingdialog.h

FORMS   += widget.ui \
    settingdialog.ui

RESOURCES += \
    res/res.qrc

TRANSLATIONS += res/Translation/lan.zh_CN.ts

RC_FILE += \
    res/fileinfo.rc
