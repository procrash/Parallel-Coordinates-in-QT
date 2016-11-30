#pragma once
#include <QOpenGLContext>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <QString>
#include "cubemaptexture.h"


class Skybox
{
private:
    bool initialized = false;

    std::vector<glm::vec3> vVertexData;


    GLuint vboVerticesData;
    GLuint uiVAO;
    GLuint shaderProgramId;
    GLint matrixUniformId;

    glm::mat4x4 modelViewMatrix;

    CubeMapTexture cmTexture;

public:
    Skybox();
    void initialize();
    void loadVertexData();
    void loadTextures();
    void createBuffers();
    GLuint compileShaders();
    void render();

    void setModelViewMatrix(glm::mat4 matrix);

    void printProgramInfoLog(GLuint obj);
    void printShaderInfoLog(GLuint obj);
    QString readStringFromResourceFile(QString filenameIncludingPath);

};

