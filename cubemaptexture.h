#pragma once
#include <QString>
#include <QOpenGLContext>

class CubeMapTexture
{
private:
    GLuint textureId;
    GLuint samplerId;

    bool initialized = false;

public:
    CubeMapTexture();
    void load(QString paths[]);
    void bindTexture(int iTextureUnit);

};

