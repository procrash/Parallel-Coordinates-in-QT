#include "cubemaptexture.h"
#include <QOpenGLBuffer>
#include <QImage>
#include <iostream>

CubeMapTexture::CubeMapTexture()
{

}

void CubeMapTexture::load(QString paths[]) {

    if (initialized) {
        glDeleteTextures(1, &textureId);
        glDeleteSamplers(1, &samplerId);
    }

    std::cout << "Loading Textures..." << std::endl;

    // initialized = true;


    GLuint textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);


    int width,height;
    unsigned char* image;
    for(GLuint i = 0; i < 6; i++)
    {
        QImage img(paths[i]);
        int width = img.width();
        int height = img.height();
        int bpp = img.depth();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.bits());
    }


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenSamplers(1, &samplerId);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CubeMapTexture::bindTexture(int iTextureUnit) {
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glBindSampler(iTextureUnit, samplerId);
}
