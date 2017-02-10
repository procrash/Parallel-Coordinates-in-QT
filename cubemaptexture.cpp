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
        // glDeleteSamplers(1, &samplerId);
    }


    // glBindSampler(0, 0);

    std::cout << "Loading Textures..." << std::endl;

    // initialized = true;

    glActiveTexture(GL_TEXTURE0);

    GLuint textureId;

    glGenTextures(1, &textureId);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    /*
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
   // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);


    int width,height;
    unsigned char* image;
    for(GLuint i = 0; i < 6; i++)
    {
        QImage img(paths[i]);
        int width = img.width();
        int height = img.height();
        int bpp = img.depth();

        GLenum format = GL_RGB;

        if (img.format()== QImage::Format_RGBA8888) {
            format = GL_RGBA;
        } else
        if (img.format()==QImage::Format_RGB888) {
            format = GL_RGB;
        }

        std::cout << img.format() << std::endl;


        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, img.bits());
    }

    */

    // setup texture
        unsigned int width = 1, height = 1;
        unsigned char xpos[] = { 0xFF, 0x00, 0x00, 0xFF };    // red
        unsigned char xneg[] = { 0x00, 0xFF, 0xFF, 0xFF };    // cyan
        unsigned char ypos[] = { 0x00, 0xFF, 0x00, 0xFF };    // green
        unsigned char yneg[] = { 0xFF, 0x00, 0xFF, 0xFF };    // magenta
        unsigned char zpos[] = { 0x00, 0x00, 0xFF, 0xFF };    // blue
        unsigned char zneg[] = { 0xFF, 0xFF, 0x00, 0xFF };    // yellow
      //  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xpos);

        /*
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xpos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xneg);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ypos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, yneg);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zpos);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zneg);
*/

//        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

         // glGenSamplers(1, &samplerId);

         glBindTexture(GL_TEXTURE_2D, 0);




   // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);







}

void CubeMapTexture::bindTexture(int iTextureUnit) {
    glBindSampler(0, 0);

    std::cout << "Binding Textures Cubemap" << std::endl;
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // glBindSampler(iTextureUnit, samplerId);
}

void CubeMapTexture::printOpenGLErrors() {

    GLenum error;

    do {
        error = glGetError();
        switch (error) {
         case GL_INVALID_ENUM : std::cout << "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag." << std::endl; break;
         case GL_INVALID_VALUE : std::cout << "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag." << std::endl; break;
         case GL_INVALID_OPERATION : std::cout << "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag." << std::endl; break;
         case GL_INVALID_FRAMEBUFFER_OPERATION: std::cout << "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag." << std::endl; break;
         case GL_OUT_OF_MEMORY: std::cout << "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded." << std::endl; break;
         case GL_STACK_UNDERFLOW: std::cout << "An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl; break;
         case GL_STACK_OVERFLOW : std::cout << "An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl; break;
        }
    }
    while (error!=GL_NO_ERROR);

   // assert(glGetError()==GL_NO_ERROR);
}
