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

LIBS += -lexiv2
LIBS += -lmarblewidget
LIBS += -lX11

#for stdC++11 threads
QMAKE_CXXFLAGS = -std=c++11
LIBS += -pthread

SOURCES += main.cpp\
        mainwindow.cpp \
    xml_handler.cpp \
    config_parser.cpp \
    image_analyzer.cpp \
    tag_handler.cpp \
    divfilesystemmodel.cpp

HEADERS  += mainwindow.h \
    xml_handler.h \
    config_parser.h \
    image_analyzer.h \
    tag_handler.h \
    divfilesystemmodel.h \
    Descriptors.h

FORMS    += mainwindow.ui
