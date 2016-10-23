#pragma once

//#include <glm/vec3.hpp>
#include "./3rdParty/glm/glm/vec3.hpp"
#include <vector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>

#define BYTE unsigned char
using namespace std;

class HeightMap
{
private:
    GLuint vboVerticesData;
    GLuint vboIndexData;

    vector<BYTE> data;
    vector<GLuint> indices;

    GLuint uiVAO;


    int iRows;
    int iCols;

    int iSize;
    int iCurrentSize;

    glm::vec3* vVertexData1D;

    glm::vec3** vVertexData;
    vector<vector<glm::vec3>> vFinalNormals;


    void loadVertexData();
    void calculateTriangleNormals();
    void createBuffers();

public:
    HeightMap();
    void render();
};
