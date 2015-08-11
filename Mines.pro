#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T09:44:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mines
TEMPLATE = app


SOURCES += main.cpp\
        gui_main.cpp \
    tile.cpp \
    customgame.cpp

HEADERS  += gui_main.h \
    minebutton.h \
    lcdtimer.h \
    tile.h \
    customgame.h

FORMS    += gui_main.ui \
    customgame.ui

RESOURCES += \
    resources.qrc
