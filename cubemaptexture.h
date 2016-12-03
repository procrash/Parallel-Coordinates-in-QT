#pragma once
#include <QString>
#include <QOpenGLContext>

class CubeMapTexture
{
public:
     GLuint textureId;
private:

    GLuint samplerId;

    bool initialized = false;

public:
    CubeMapTexture();
    void load(QString paths[]);
    void bindTexture(int iTextureUnit);
    void printOpenGLErrors();
};

