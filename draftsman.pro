#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T22:38:39
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = draftsman
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    optionswindow.cpp \
    aboutwindow.cpp \
    qcustomplot.cpp \
    drawdata.cpp

HEADERS  += mainwindow.h \
    optionswindow.h \
    aboutwindow.h \
    qcustomplot.h \
    drawdata.h

FORMS    += mainwindow.ui \
    optionswindow.ui \
    aboutwindow.ui

OTHER_FILES +=
