#pragma once
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <vector>

#include <parallelcoordinatesglobals.h>
#include "datastore.h"

// For testing only

#include <time.h>
#include <stdlib.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

using namespace std;

template<class T>
class View3D : public QOpenGLWidget
{

private:

    bool switchedToFullscreen;

    GLuint m_posAttr;
    QOpenGLShaderProgram *m_program;
    QOpenGLVertexArrayObject *m_vaoPtr;
    QOpenGLBuffer *m_vboPtr;

    int minX = 0;
    int maxX = 1000;
    int minY = 0;
    int maxY = 1000;
    int minZ = -1000;
    int maxZ = 1000;

    DataStore<T>* dataStorePtr;

    vector<DataSet<WIDGET_DATA_TYPE>>* dataSetPtr = NULL;

    int xRot;
    int yRot;
    int zRot;
    int distance = 0;

    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);



    void initializeData();

    // For testing purposes only...
    static const int resX = 100;
    static const int resY = 100;
    int testData[resX*resY];



public:
    View3D(QWidget *parent)
           : QOpenGLWidget(parent), switchedToFullscreen(false), m_vaoPtr(NULL), m_vboPtr(NULL) {

        qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
        qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

        // For testing purposes only...
        srand (time(NULL));

        for (int y=0; y<resY; y++)
            for (int x=0; x<resX;x++) {
                testData[x+y*resX] = 0;
            }


        for (int iteration = 0; iteration<500; iteration++)
        for (int y=0; y<resY; y++)
            for (int x=0; x<resX;x++) {
                int randomNr = rand()%3;
                switch (randomNr) {
                    case 0: break; // Do nothing
                    case 1: testData[x+y*resX]++; break;
                    case 2: testData[x+y*resX]--;
                }
            }

        /*
        for (int y=0; y<resY; y++)
            for (int x=0; x<resX;x++) {
                int randomNr = rand()%200;
                testData[x+y*resX] = randomNr;
            }
        */
    }

    void setDataStorePtr(DataStore<T>* dataStorePtr);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

};

