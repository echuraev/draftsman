#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T22:38:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draftsman
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    optionswindow.cpp \
    aboutwindow.cpp \
    drawgraphics.cpp

HEADERS  += mainwindow.h \
    optionswindow.h \
    aboutwindow.h \
    drawgraphics.h

FORMS    += mainwindow.ui \
    optionswindow.ui \
    aboutwindow.ui
