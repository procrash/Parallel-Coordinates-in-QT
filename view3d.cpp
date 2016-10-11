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
    /*
    if (vertexBuffer!=NULL) {
        vertexBuffer->destroy();
        delete(vertexBuffer);
    }
    vertexBuffer = new QOpenGLBuffer();

    if (m_vaoPtr!=NULL) {
        m_vaoPtr->destroy();
        delete(m_vaoPtr);
    }
    m_vaoPtr = new QOpenGLVertexArrayObject();
    */

    if (verticesPtr!=NULL) {
        free(verticesPtr);
    }




    //  3 Floats for Coordinates  * 2 Points per Triangle Strip
    // *3 (RGB Color Information) * 2 Points

    const size_t space = nrOfPoints*3*2*3*2*sizeof(GLfloat);

    verticesPtr = (GLfloat*) malloc(space);


    const int xDist = 20;
    const int yDist = 20;

    int idx = 0;
    for (int y=0; y<500; y++)
    for (int x=0; x<500; x++) {
        // Coordinates
        verticesPtr[idx]   = ((GLfloat) x*xDist)/(GLfloat)500.0f;
        verticesPtr[idx+1] = ((GLfloat) y*yDist)/(GLfloat)500.0f;
        verticesPtr[idx+2] = 0;


        // Color
        verticesPtr[idx+3] = 1.0f;
        verticesPtr[idx+4] = 0;
        verticesPtr[idx+5] = 0;


        // Coordinates
        verticesPtr[idx+6] = ((GLfloat) x*xDist)/(GLfloat)500.0f;
        verticesPtr[idx+7] = ((GLfloat) (y+1)*yDist)/(GLfloat)500.0f;
        verticesPtr[idx+8] = 0;


        // Color
        verticesPtr[idx+9]  = 1.0f;
        verticesPtr[idx+10] = 0;
        verticesPtr[idx+11] = 0;

        idx+=12;
    };


    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, space, verticesPtr, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6, 0);

    GLushort indices[] = { 0, 1, 2 };
    glGenBuffers(1, &indicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GLfloat, false, 0, 6);

    /*
    glEnableVertexAttribArray(0);
    m_program->setAttributeBuffer(0,GL_FLOAT, 0, 3, 6);

    glEnableVertexAttribArray(1);
    m_program->setAttributeBuffer(1,GL_FLOAT, 3, 3, 6);

    */
}

template<class T>
void View3D<T>::paintGL()
{


    glBindVertexArray(vertexBufferId);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 1.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2+distance/20);

    matrix.rotate(xRot / 16.0, 1.0, 0.0, 0.0);
    matrix.rotate(yRot / 16.0, 0.0, 1.0, 0.0);
    matrix.rotate(zRot / 16.0, 0.0, 0.0, 1.0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 500);


    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, 500*500);



    m_program->release();
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
    distance += event->delta();
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

