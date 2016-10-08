#include "view3d.h"
#include <iostream>
#include <vector>
#include "datastore.h"

using namespace std;

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

template<class T>
QSize View3D<T>::minimumSizeHint() const
{
    return QSize(50, 50);
}


template<class T>
QSize View3D<T>::sizeHint() const
{
    return QSize(600, 600);
}

template<class T>
void View3D<T>::initializeGL()
{



     glClearColor(0,0,0,255);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
  //  glEnable(GL_LIGHTING);
  //  glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    /*
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
*/
}

template<class T>
void View3D<T>::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(500.0, 500.0, -1+distance);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    //glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,0,1);

    int xDist = 10;
    int yDist = 10;

    if (this->dataSetPtr!=NULL) {
        glPointSize(3.0f);
        glBegin(GL_POINTS);
        for (uint64_t i=0; i<dataSetPtr->size(); i++) {
            for (int j=0; j<nrOfDimensions; j++) {
                // glVertex3f(i*xDist, j*yDist, dataSetPtr->at(i).dimVal[j] / );
            }
        }
        glEnd();
    }
    //    glEnd();
}

template<class T>
void View3D<T>::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(minX, maxX, minY, maxY, minZ, maxZ);
    glMatrixMode(GL_MODELVIEW);

}

template<class T>
void View3D<T>::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        update();
    }
}

template<class T>
void View3D<T>::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        update();

    }
}

template<class T>
void View3D<T>::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        update();
    }
}

template<class T>
void View3D<T>::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

template<class T>
void View3D<T>::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

template<class T>
void View3D<T>::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    distance += 10*numSteps;
    event->accept();

    update();
}

template<class T>
void View3D<T>::setDataStorePtr(DataStore<T>* dataStorePtr){
    this->dataStorePtr = dataStorePtr;
}

