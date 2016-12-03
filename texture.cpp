#include "texture.h"
#include <QImage>
#include <iostream>

Texture::Texture() : initialized(false)
{

}

void Texture::load(QString path) {
    if (initialized) {
        glDeleteTextures(1, &textureId);
        glDeleteSamplers(1, &samplerId);
    }

    cout << "Loading Textures..." << endl;

    initialized = true;

    QImage img(path);

    int width = img.width();
    int height = img.height();
    int bpp = img.depth();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    cout << bpp << endl;

    int iFormat = bpp == 24 ? GL_RGB : bpp == 8 ? GL_LUMINANCE : 0;
    int iInternalFormat = bpp == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) img.bits());

    glGenerateMipmap(GL_TEXTURE_2D);
    glGenSamplers(1, &samplerId);
}

void Texture::setFiltering(int a_tfMagnification, int a_tfMinification)
{
   // Set magnification filter
   if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
      glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
      glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Set minification filter
   if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
      glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
      glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
      glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
      glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
      glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

//   GL_NEAREST_MIPMAP_LINEAR


   // tfMinification = a_tfMinification;
   // tfMagnification = a_tfMagnification;
}

void Texture::bindTexture(int iTextureUnit)
{
   //  cout << "Binding Textures" << endl;
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindSampler(iTextureUnit, samplerId);
}
