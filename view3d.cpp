#include "view3d.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "heightmap.h"

#include "datastore.h"
// #include "datastore.cpp"

// #include "matrix4x4.h"
#include "vmath.h"


#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

/*

#include "./3rdParty/glm/glm/vec3.hpp"
#include "./3rdParty/glm/glm/vec4.hpp"
#include "./3rdParty/glm/glm/mat4x4.hpp"
#include "./3rdParty/glm/glm/gtc/matrix_transform.hpp"
#include "./3rdParty/glm/glm/gtc/type_ptr.hpp"
*/

#include <QDebug>

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
QString View3D<T>::readStringFromResourceFile(QString filenameIncludingPath) {
    QString result = "";

    QFile f(filenameIncludingPath);
    if (!f.open(QFile::ReadOnly | QFile::Text)) { return result; }

    QTextStream in(&f);
    //qDebug() << f.size() << in.readAll();
    result = in.readAll();
    f.close();
    return result;
}


template<class T>
void View3D<T>::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

template<class T>
void View3D<T>::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}


template<class T>
GLuint View3D<T>::compileShaders() {
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;

    GLuint program_id;


    QString debug = readStringFromResourceFile(":/shaders/vertexShader.glsl");

    string str = debug.toStdString();
    str = str;
    debug = debug;

    string vertexShaderStr = readStringFromResourceFile(":/shaders/vertexShader.glsl").toStdString();
    string fragmentShaderStr = readStringFromResourceFile(":/shaders/fragmentShader.glsl").toStdString();

    const GLchar *vertexShader_cStr   = (const GLchar *) vertexShaderStr.c_str();
    const GLchar *fragmentShader_cStr = (const GLchar *) fragmentShaderStr.c_str();

    cout << vertexShader_cStr << endl;

    // Create Vertex Shader
    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertexShader_cStr, NULL);
    glCompileShader(vertex_shader_id);

    // Create Fragment Shader
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragmentShader_cStr, NULL);
    glCompileShader(fragment_shader_id);

    // Print logs of Shaders
    printShaderInfoLog(vertex_shader_id);
    printShaderInfoLog(fragment_shader_id);

    // Create Program, attach shaders and link
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glBindFragDataLocation(program_id, 0, "aColor"); // TODO: check if this is necessary

    glLinkProgram(program_id);

    // Print logs of linker
    printProgramInfoLog(program_id);

    // Free resources as the program has now the shaders...
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

template<class T>
void View3D<T>::initializeGL()
{

    glClearColor(0,0,0,255);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    shaderProgramId = compileShaders();
    matrixUniformId = glGetUniformLocation(shaderProgramId, "matrix");


    /*
    m_program = new QOpenGLShaderProgram(this);

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexShader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentShader.glsl");

    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    */

    initializeData();

   // m_program->release();

//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);

}

template<class T>
void View3D<T>::initializeData()
{

    if (verticesPositionsPtr!=NULL) {
        free(verticesPositionsPtr);
    }

    if (verticesColorsPtr!=NULL) {
        free(verticesColorsPtr);
    }

    //  3 Floats for Coordinates  * 2 Points per Triangle Strip
    // *3 (RGB Color Information) * 2 Points

    const size_t spaceVerticesPositions = nrOfPoints*3*2*sizeof(GLfloat);

    verticesPositionsPtr = (GLfloat*) malloc(spaceVerticesPositions);
    memset(verticesPositionsPtr, 0, spaceVerticesPositions);

    const size_t spaceVerticesColors = nrOfPoints * 3 * sizeof(GLfloat);

    verticesColorsPtr = (GLfloat*) malloc(spaceVerticesColors);
    memset(verticesColorsPtr, 0, spaceVerticesColors);

    const int xDist = 5;
    const int yDist = 5;



    // vector<DataSet<T>>* data = dataStorePtr->getDataSetPtr();

    // this->dataStorePtr->getDataSetPtr();



    int idx = 0;
    for (int y=0; y<nrOfPointsY; y++)
    for (int x=0; x<nrOfPointsX; x++) {


        // Coordinates
        verticesPositionsPtr[idx]   = ((GLfloat) x*xDist-nrOfPointsX*xDist/2)/(GLfloat)nrOfPointsX;
        verticesPositionsPtr[idx+1] = ((GLfloat) y*yDist-nrOfPointsY*yDist/2)/(GLfloat)nrOfPointsY;
        verticesPositionsPtr[idx+2] = 0; //perlin.Get2D(x,y)/90000;
        // cout << perlin.Get2D(x,y) << endl;

        // Coordinates
        verticesPositionsPtr[idx+3] = ((GLfloat) x*xDist-nrOfPointsX*xDist/2)/(GLfloat)nrOfPointsX;
        verticesPositionsPtr[idx+4] = ((GLfloat) (y+1)*yDist-nrOfPointsY*yDist/2)/(GLfloat)nrOfPointsY;
        verticesPositionsPtr[idx+5] = 0; //perlin.Get2D(x,(y+1))/90000;;

        idx+=6;
    };

    idx = 0;
    int colorIdx =0;

    // Set Colors for Vertices...
    for (int y=0; y<nrOfPointsY; y++)
    for (int x=0; x<nrOfPointsX; x++) {
        if (colorIdx%3==0) {
            verticesColorsPtr[idx]   = 1.0f;
            verticesColorsPtr[idx+1] = 0.0f;
            verticesColorsPtr[idx+2] = 0;
        } else if (colorIdx%3==1) {
            verticesColorsPtr[idx]   = 0;
            verticesColorsPtr[idx+1] = 1.0f;
            verticesColorsPtr[idx+2] = 0;
        } else if (colorIdx%3==2) {
            verticesColorsPtr[idx]   = 0;
            verticesColorsPtr[idx+1] = 0;
            verticesColorsPtr[idx+2] = 1.0f;
        }
        colorIdx++;
        idx+=3;
    }

    // cout << sizeof(vertices) << endl;
    // memcpy(verticesPtr, vertices, sizeof(vertices));


    // Create Vertex Position Buffer...
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, spaceVerticesPositions, verticesPositionsPtr, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


   //  m_program->setAttributeBuffer(0,GL_FLOAT, 0, 3, 0);


    // Create Vertex Color Buffer...
    glGenBuffers(1, &vertexBufferColorsId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColorsId);
    glBufferData(GL_ARRAY_BUFFER, spaceVerticesColors, verticesColorsPtr, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // m_program->setAttributeBuffer(1,GL_FLOAT, 0, 3, 0);


    /*
    // Create Index Buffer...
    GLushort indices[] = { 0,1,2, 0,3,4 };
    glGenBuffers(1, &indicesBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */

    // glEnableVertexAttribArray(1);


    /*
    glEnableVertexAttribArray(0);
    m_program->setAttributeBuffer(0,GL_FLOAT, 0, 3, 6);

    glEnableVertexAttribArray(1);
    m_program->setAttributeBuffer(1,GL_FLOAT, 3, 3, 6);

    */

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);



}

template<class T>
glm::mat4 View3D<T>::camera(float Translate, glm::vec2 const & Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

template<class T>
void View3D<T>::paintGL()
{

    glBindVertexArray(vertexArrayId);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(shaderProgramId);


    float translate = 2.0f+distance/20.0f;


    glm::mat4 mvp = camera(translate, glm::vec2((float)(xRot/16)*M_PI/180,(float)(yRot/16)*M_PI/180)); //glm::vec2((float)(xRot),(float)(yRot)));
    glUniformMatrix4fv(matrixUniformId, 1, GL_FALSE,  glm::value_ptr(mvp));



    for (int i=0; i<nrOfPointsY;i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*nrOfPointsX, i*nrOfPointsX+nrOfPointsX);
    }



    heightMap.render();

    glUseProgram(0);
    glBindVertexArray(0);
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
void View3D<T>::setTranslationX(int distance) {

}

template<class T>
void View3D<T>::setTranslationY(int distance) {

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

        /*
        xDistance+=dx/100;
        this->update();
        */
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
    // TODO: Implement keyPress events also in main application
    // as this works only in fullscreen here!!!
    if (keyEvent->key() == Qt::Key_Escape) {
        if(isFullScreen())
        {
            setWindowFlags(Qt::Widget);
            showNormal();
        }
    }

    if (keyEvent->key() == Qt::Key_A) {
        xDistance-=0.05;
        this->update();
    }
    if (keyEvent->key() == Qt::Key_D) {
        xDistance+=0.05;
        this->update();
    }

    if (keyEvent->key() == Qt::Key_W) {
        yDistance+=0.05;
        this->update();
    }
    if (keyEvent->key() == Qt::Key_S) {
        yDistance-=0.05;
        this->update();
    }

}

