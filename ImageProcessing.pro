#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T18:02:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    customview.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    customview.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui
