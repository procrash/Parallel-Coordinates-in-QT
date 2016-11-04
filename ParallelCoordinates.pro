#-------------------------------------------------
#
# Project created by QtCreator 2016-09-07T11:53:49
#
#-------------------------------------------------

QT       += core gui
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
    parallelcoordinatesworker.cpp \
    parallelcoordinatesworkerobserver.cpp \
    heightmap.cpp \
    texture.cpp

HEADERS  += mainwindow.h \
    qrangeslider.h \
    parallelcoordinateswidget.h \
    parallelcoordinatesglobals.h \
    view3d.h \
    dataset.h \
    datastore.h \
    qrangesliderobserver.h \
    parallelcoordinatesworker.h \
    parallelcoordinatesworkerobserver.h \
    vmath.h \
    heightmap.h \
    texture.h

FORMS    += mainwindow.ui

DISTFILES += \
    3dVisualization/fragmentShader.glsl \
    3dVisualization/vertexShader.glsl \
    heightmap.jpg

RESOURCES += \
    3dvisualization.qrc

INCLUDEPATH += 3rdParty/glm
