#-------------------------------------------------
#
# Project created by QtCreator 2016-09-07T11:53:49
#
#-------------------------------------------------

QT       += core gui quickwidgets datavisualization
QMAKE_MAC_SDK = macosx10.12
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParallelCoordinates 
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    qrangeslider.cpp \
    parallelcoordinateswidget.cpp \
    view3d.cpp \
    datastore.cpp \
    qrangesliderobserver.cpp \
    surfacegraph.cpp

HEADERS  += mainwindow.h \
    qrangeslider.h \
    parallelcoordinateswidget.h \
    parallelcoordinatesglobals.h \
    view3d.h \
    dataset.h \
    datastore.h \
    qrangesliderobserver.h \
    surfacegraph.h

FORMS    += mainwindow.ui

DISTFILES += \
    Form.ui.qml \
    ThreeDSurface.qml

RESOURCES += \
    qml.qrc

