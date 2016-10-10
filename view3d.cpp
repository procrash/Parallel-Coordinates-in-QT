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
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);


    m_program = new QOpenGLShaderProgram(this);

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexShader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentShader.glsl");

    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");


    initializeData();

   // m_program->release();

//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);



}

template<class T>
void View3D<T>::initializeData()
{
    if (m_vboPtr!=NULL) {
        m_vboPtr->destroy();
        delete(m_vboPtr);
    }
    m_vboPtr = new QOpenGLBuffer();

    if (m_vaoPtr!=NULL) {
        m_vaoPtr->destroy();
        delete(m_vaoPtr);
    }
    m_vaoPtr = new QOpenGLVertexArrayObject();

    if (verticesPtr!=NULL) {
        free(verticesPtr);
    }




    srand (time(NULL));
    verticesPtr = (GLfloat*) malloc(nrOfPoints*3*sizeof(GLfloat));


    GLfloat vertices[] = {
          +0.0f, +1.0f, 0.0f,
          -1.0f, -1.0f,   0.0f,
          +1.0f, -1.0f,   0.0f
       };

    memcpy(verticesPtr, vertices, sizeof(vertices));

    /*
    int idx = 0;
    for (int y=0; y<500; y++)
    for (int x=0; x<500; x++) {
        verticesPtr[idx]   =((GLfloat) x)/(GLfloat)500.0f; // 1/((rand()%1000)+1);
        verticesPtr[idx+1] =((GLfloat) y)/(GLfloat)500.0f; // 1/((rand()%1000)+1);
        verticesPtr[idx+2] =0; // 1/((rand()%1000)+1);
        idx+=3;
    };
    */


    m_vboPtr->create();
    m_vboPtr->bind();
    m_vboPtr->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vboPtr->allocate(verticesPtr, nrOfPoints*3*sizeof(GL_FLOAT));

    m_vaoPtr->create();
    m_vaoPtr->bind();
    glEnableVertexAttribArray(0);
    m_program->setAttributeBuffer(0,GL_FLOAT, 0, 3, 0);

    m_vboPtr->release();
    m_vaoPtr->release();
}

template<class T>
void View3D<T>::paintGL()
{

    if (switchedToFullscreen) {
        initializeData();
    }




    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_vaoPtr->bind();
    m_program->bind();


    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 1.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    m_program->setUniformValue(m_matrixUniform, matrix);

    // glPointSize(10.0f);
    //glDrawArrays(GL_POINT, 0, nrOfPoints);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawArrays(GL_LINES, 0,3);


    m_program->release();
    m_vaoPtr->release();

    //m_vao.destroy();
    //m_vbo.destroy();


}

/*
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

       //  glEnd();
        //}


    // if (this->dataSetPtr!=NULL) {
    //    glPointSize(3.0f);
    //    glBegin(GL_POINTS);
    //    for (uint64_t i=0; i<dataSetPtr->size(); i++) {
    //        for (int j=0; j<nrOfDimensions; j++) {
    //            glVertex3f(i*xDist, j*yDist, 0 /dataSetPtr->at(i).dimVal[j] / );
    //        }
    //    }
    //    glEnd();
    // }


}
*/

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
        switchedToFullscreen = true;
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

