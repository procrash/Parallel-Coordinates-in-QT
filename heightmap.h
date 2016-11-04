#pragma once

//#include <glm/vec3.hpp>
#include "./3rdParty/glm/glm/vec3.hpp"
#include <glm/mat4x4.hpp> // glm::mat4

#include <vector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QString>

#define BYTE unsigned char
using namespace std;

class HeightMap
{
private:


    bool initialized = false;

    GLuint shaderProgramId;

    GLuint vboVerticesData;
    GLuint vboIndexData;

    vector<BYTE> data;
    vector<BYTE> indices;

    GLuint uiVAO;


    int iRows;
    int iCols;

    int iSize;
    int iCurrentSize;

    GLint matrixUniformId;
    glm::mat4x4 modelViewMatrix;

//    glm::vec3* vVertexData1D;

//    glm::vec3** vVertexData;

    vector< vector< glm::vec3> > vVertexData;

    vector<vector<glm::vec3>> vFinalNormals;

    QString readStringFromResourceFile(QString filenameIncludingPath);
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);

    void loadVertexData();
    void calculateTriangleNormals();
    void createBuffers();

    GLuint compileShaders();
    void printOpenGLErrors();

public:
    HeightMap();
    void initialize();
    void setModelViewMatrix(glm::mat4 matrix);
    void render();
};
