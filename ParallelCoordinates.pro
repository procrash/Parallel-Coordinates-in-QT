#-------------------------------------------------
#
# Project created by QtCreator 2016-09-07T11:53:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParallelCoordinates 
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qrangeslider.cpp \
    parallelcoordinateswidget.cpp \
    view3d.cpp \
    datastore.cpp \
    textboxfocusobservable.cpp

HEADERS  += mainwindow.h \
    qrangeslider.h \
    parallelcoordinateswidget.h \
    parallelcoordinatesglobals.h \
    mainwindow.h \
    view3d.h \
    dataset.h \
    datastore.h \
    textboxfocusedobserver.h \
    textboxfocusedobservable.h

FORMS    += mainwindow.ui \
    mainwindow.ui

