#-------------------------------------------------
#
# Project created by QtCreator 2013-01-27T20:05:57
#
#-------------------------------------------------

QT       += core gui
QT      += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dynamic_Image_Viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xml_handler.cpp

HEADERS  += mainwindow.h \
    xml_handler.h

FORMS    += mainwindow.ui
