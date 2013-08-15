#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T23:56:50
#
#-------------------------------------------------

QT       += core gui
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MasterMeasure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tabview.cpp \
    chromosome.cpp \
    scaledialog.cpp \
    chromosomeindexdialog.cpp

HEADERS  += mainwindow.h \
    tabview.h \
    chromosome.h \
    scaledialog.h \
    chromosomeindexdialog.h

FORMS    += mainwindow.ui \
    scaledialog.ui \
    chromosomeindexdialog.ui

RESOURCES += \
    icon.qrc

RC_FILE = myapp.rc

HEADERS += tabview.h
