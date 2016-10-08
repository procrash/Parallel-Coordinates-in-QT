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
   // glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);


    /*
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
*/
}

template<class T>
void View3D<T>::paintGL()
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(minX-distance, maxX+distance, minY-distance, maxY+distance, minZ+distance, maxZ-distance);

//    glOrtho(minX, maxX, minY, maxY, minZ, maxZ);
    glMatrixMode(GL_MODELVIEW);


    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //glEnable ( GL_LIGHTING ) ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();



    glTranslatef(500, 500, -1);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glTranslatef(-500, -500, -1);


    //glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,0,1);

    int xDist = 10;
    int yDist = 10;
    int zDist = 10;

    glPointSize(3.0f);

//    if (dataStorePtr!=NULL) {

    int nrPointsX = 100;
    int nrPointsY = 100;

    for (int y = 0; y<nrPointsY-1; y++) {
      glBegin(GL_TRIANGLE_STRIP);
      for (int x = 0; x<nrPointsX; x++) {
        int z1 = testData[y*nrPointsX+x];
        int z2 = testData[(y+1)*nrPointsX+x];

        //cout << z1 << endl;
        //cout << z2 << endl;
        glVertex3f(x*xDist,  y*yDist, z1);
        glVertex3f(x*xDist,  (y+1)*yDist, z2);
      }
      glEnd();
    }

        /*
        int i = 0;
        for (int x = 0; x<100; x++) {
            for (int z=0; z<100; z++) {

                vector<DataSet<WIDGET_DATA_TYPE>>* dataSets = dataStorePtr->getDataSet();

                if (i<dataSets->size()-1) i++; else i=0;
                DataSet<WIDGET_DATA_TYPE> data = dataSets->at(i);


                WIDGET_DATA_TYPE val;

                val = data.dimVal[1];
                double dVal = (double)val;

                WIDGET_DATA_TYPE min = dataStorePtr->getMinValPtr()[1];
                WIDGET_DATA_TYPE max = dataStorePtr->getMaxValPtr()[1];

                dVal/=dVal*500/(max-min);

                glVertex3f(x*xDist,  z*zDist, dVal);

            }
        }
        */
       //  glEnd();
        //}

    /*
    if (this->dataSetPtr!=NULL) {
        glPointSize(3.0f);
        glBegin(GL_POINTS);
        for (uint64_t i=0; i<dataSetPtr->size(); i++) {
            for (int j=0; j<nrOfDimensions; j++) {
                glVertex3f(i*xDist, j*yDist, 0 /dataSetPtr->at(i).dimVal[j] / );
            }
        }
        glEnd();
    }
    */

}

template<class T>
void View3D<T>::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    cout << "MaxZ: " << maxZ << endl;

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

template<class T>
void View3D<T>::mouseDoubleClickEvent(QMouseEvent *e) {
    if(isFullScreen())
    {
        setWindowFlags(Qt::Widget);
        showNormal();
    } else {
        setWindowFlags(Qt::Window);
        showFullScreen();
    }
}


template<class T>
void View3D<T>::keyPressEvent(QKeyEvent *keyEvent) {
    if (keyEvent->key() == Qt::Key_Escape) {
        if(isFullScreen())
        {
            setWindowFlags(Qt::Widget);
            showNormal();
        }
    }
}

