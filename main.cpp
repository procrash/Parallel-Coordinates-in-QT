#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "parallelcoordinatesglobals.h"

#include <QSurfaceFormat>>

using namespace std;

int main(int argc, char *argv[])
{    

    QSurfaceFormat format;
    format.setDepthBufferSize( 24 );
    format.setMajorVersion( 4 );
    format.setMinorVersion( 3 );
    format.setSamples( 4 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    //setFormat(format);
    QSurfaceFormat::setDefaultFormat(format);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

